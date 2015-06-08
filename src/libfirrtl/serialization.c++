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

#include "serialization.h++"
#include <iostream>
#include <expat.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

using namespace libfirrtl;

circuit::ptr libfirrtl::parse_xml(const std::string filename)
{
    auto parser = XML_ParserCreate(NULL);

    auto file = fopen(filename.c_str(), "r");
    char buffer[BUFFER_SIZE];
    ssize_t readed;
    while ((readed = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        auto out = XML_Parse(parser, buffer, readed, 0);
        switch (out) {
        case XML_STATUS_ERROR:
        {
            auto code = XML_GetErrorCode(parser);
            auto str = XML_ErrorString(code);
            std::cerr << "XML error: " << str << "\n";
            abort();
            break;
        }
        case XML_STATUS_OK:
            break;
        case XML_STATUS_SUSPENDED:
            std::cerr << "XML suspended\n";
            abort();
            break;

        default:
            std::cerr << "Unknown XML status " << out << "\n";
            abort();
        }
    }
    XML_Parse(parser, NULL, 0, 1);

    fclose(file);
    XML_ParserFree(parser);

    return std::make_shared<circuit>(
        std::vector<module::ptr>{
            std::make_shared<module>("Top")
                }
        );
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
        fprintf(file, "    </ports>\n");
        fprintf(file, "    <statements>\n");
        fprintf(file, "    </statements>\n");

        fprintf(file, "  </module>\n");
    }

    fprintf(file, "</firrtl>\n");

    fclose(file);
}
