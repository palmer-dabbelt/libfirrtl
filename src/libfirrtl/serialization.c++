/*
 * Copyright (c) 2015, The Regents of the University of California
 * (Regents).  All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Regents nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 * SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 * OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 * HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
 * MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "parseable.h++"
#include "serialization.h++"
#include "util/expat.h++"
#include <iostream>
#include <stack>
#include <unordered_map>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

using namespace libfirrtl;

circuit::ptr libfirrtl::parse_xml(const std::string filename)
{
    std::vector<module::ptr> modules;

    std::stack<std::shared_ptr<parseable>> parse_stack;

    parseable_circuit::ptr p_circuit = nullptr;
    std::unordered_map<std::string, parseable_module::ptr> p_modules;
    parseable_module::ptr p_module = nullptr;

    /* Here we have the default element handlers, which are called
     * when there's no stack at all. */
    auto start_element_handler =
        [&](const std::string& name, const std::vector<util::expat_attr>& attrs)
        {
#ifdef PRINT_PARSE_STACK
            std::cerr
            << "start_element_handler\n"
            << "  " << name << "\n"
            << "  attrs:\n";

            for (const auto& attr: attrs)
                std::cerr << "    " << attr.key << "=" << attr.val << "\n";
#endif

            if (name.compare("firrtl") == 0) {
                if (parse_stack.size() != 0)
                    abort();
                return;
            }

            if (name.compare("circuit") == 0) {
                if (parse_stack.size() != 0)
                    abort();
                if (attrs.size() != 0)
                    abort();

                parse_stack.push(std::make_shared<parseable_circuit>());
                return;
            }

            if (name.compare("module") == 0) {
                util::option<std::string> name;
                for (const auto& attr: attrs) {
                    if (attr.key.compare("name") == 0)
                        name = attr.val;
                    else
                        abort();
                }

                auto m = std::make_shared<parseable_module>(name.value());

                if (parse_stack.size() != 0) {
                    switch (parse_stack.top()->get_type()) {
                    case parseable_type::CIRCUIT:
                        parse_stack.top()->add_child(m);
                        break;
                    case parseable_type::MODULE:
                    case parseable_type::PORTS:
                    case parseable_type::INPUT:
                        abort();
                        break;
                    }
                }

                p_module = m;
                parse_stack.push(m);
                return;
            }

            if (name.compare("ports") == 0) {
                auto m = std::make_shared<parsable_ports>();

                if (parse_stack.size() != 0) {
                    switch (parse_stack.top()->get_type()) {
                    case parseable_type::MODULE:
                        parse_stack.top()->add_child(m);
                        parse_stack.push(m);
                        return;
                    case parseable_type::CIRCUIT:
                    case parseable_type::PORTS:
                    case parseable_type::INPUT:
                        abort();
                        break;
                    }
                }

                return;
            }

            if (name.compare("statements") == 0) {
                return;
            }

            if (name.compare("input") == 0) {
                util::option<std::string> name;
                for (const auto& attr: attrs) {
                    if (attr.key.compare("name") == 0)
                        name = attr.val;
                    else
                        abort();
                }

                auto m = std::make_shared<parseable_input>(name.value());

                if (parse_stack.size() != 0) {
                    switch (parse_stack.top()->get_type()) {
                    case parseable_type::PORTS:
                        parse_stack.top()->add_child(m);
                        break;
                    case parseable_type::CIRCUIT:
                    case parseable_type::MODULE:
                    case parseable_type::INPUT:
                        abort();
                        break;
                    }
                }

                parse_stack.push(m);
                return;
            }

            if (name.compare("uint") == 0) {
                return;
            }

            fprintf(stderr, "Unable to handle FIRRTL XML node of type '%s'\n",
                    name.c_str());
            abort();
        };

    auto end_element_handler =
        [&](const std::string& name)
        {
#ifdef PRINT_PARSE_STACK
            std::cerr << "end_element_handler\n"
                      << "  " << name << "\n";
#endif

            if (name.compare("firrtl") == 0) {
                return;
            }

            if (name.compare("circuit") == 0) {
                auto top_uc = parse_stack.top();
                assert(top_uc->get_type() == parseable_type::CIRCUIT);
                parse_stack.pop();

                if (parse_stack.size() != 0)
                    abort();
                if (p_circuit != nullptr)
                    abort();

                auto top = std::dynamic_pointer_cast<parseable_circuit>(top_uc);
                if (top == NULL)
                    abort();
                p_circuit = top;

                return;
            }

            if (name.compare("module") == 0) {
                auto top_uc = parse_stack.top();
                assert(top_uc->get_type() == parseable_type::MODULE);
                parse_stack.pop();

                auto top = std::dynamic_pointer_cast<parseable_module>(top_uc);
                if (top == NULL)
                    abort();

                /* We only want to add naked modules to the list. */
                if (parse_stack.size() == 0) {
                    auto m = std::make_shared<module>(top->name());
                    modules.push_back(m);
                }

                p_module = nullptr;
                return;
            }

            if (name.compare("ports") == 0) {
                auto top_uc = parse_stack.top();
                assert(top_uc->get_type() == parseable_type::PORTS);
                parse_stack.pop();

                return;
            }

            if (name.compare("statements") == 0) {
                return;
            }

            if (name.compare("input") == 0) {
                auto top_uc = parse_stack.top();
                assert(top_uc->get_type() == parseable_type::INPUT);
                parse_stack.pop();

                return;
            }

            if (name.compare("uint") == 0) {
                return;
            }

            fprintf(stderr, "Unable to handle FIRRTL XML node of type '%s'\n",
                    name.c_str());
            abort();
        };

    {
        auto parser = std::make_shared<libfirrtl::util::expat>();

        parser->set_start_element_handler(start_element_handler);
        parser->set_end_element_handler  (end_element_handler);

        parser->read_file(filename);
    }

    return std::make_shared<circuit>(modules);
}

void libfirrtl::write_xml(const circuit::const_ptr& circuit,
                          const std::string filename)
{
    auto file = fopen(filename.c_str(), "w");

    fprintf(file, "<firrtl>\n");

    fprintf(file, "  <circuit>\n");
    for (const auto& module: circuit->modules())
        fprintf(file, "    <module name=\"%s\"/>\n", module->name().c_str());
    fprintf(file, "  </circuit>\n");

    for (const auto& module: circuit->modules()) {
        fprintf(file, "  <module name=\"%s\">\n", module->name().c_str());

        fprintf(file, "    <ports>\n");
        for (const auto& input: module->inputs()) {
            fprintf(file, "      <input name=\"%s\">\n",
                    input->name().c_str());
            fprintf(file, "      </input>\n");
        }
        fprintf(file, "    </ports>\n");

        fprintf(file, "    <statements>\n");
        fprintf(file, "    </statements>\n");

        fprintf(file, "  </module>\n");
    }

    fprintf(file, "</firrtl>\n");

    fclose(file);
}
