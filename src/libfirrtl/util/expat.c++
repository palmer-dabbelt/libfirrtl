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

#include "expat.h++"
#include <iostream>
using namespace libfirrtl::util;

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

expat::expat(void)
    : _parser(XML_ParserCreate(NULL))
{
    XML_SetUserData(_parser, this);
    XML_SetStartElementHandler(_parser, &expat_start_element_handler);
    XML_SetEndElementHandler(_parser, &expat_end_element_handler);
}

expat::~expat(void)
{
    XML_Parse(_parser, NULL, 0, 1);
    XML_ParserFree(_parser);
}

expat_error::ptr expat::read_file(const std::string filename)
{
    auto file = fopen(filename.c_str(), "r");
    char buffer[BUFFER_SIZE];
    ssize_t readed;
    while ((readed = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        auto out = XML_Parse(_parser, buffer, readed, 0);
        switch (out) {
        case XML_STATUS_ERROR:
        {
            auto code = XML_GetErrorCode(_parser);
            fclose(file);
            return std::make_shared<expat_error>(code);
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
    fclose(file);

    return std::make_shared<expat_error>();
}

void expat::expat_start_element_handler(void *self_uc,
                                        const char *name_c,
                                        const char **attrs_c)
{
    auto self = (expat *)self_uc;
    if (self->_start_element_handler.valid() == false)
        return;

    auto name = std::string(name_c);
    auto attrs = std::vector<expat_attr>();
    for (auto p = attrs_c; *p != NULL; p += 2) {
        attrs.push_back(expat_attr(p[0], p[1]));
    }

    self->_start_element_handler.value()(name, attrs);
}

void expat::expat_end_element_handler(void *self_uc,
                                        const char *name_c)
{
    auto self = (expat *)self_uc;
    if (self->_end_element_handler.valid() == false)
        return;

    auto name = std::string(name_c);

    self->_end_element_handler.value()(name);
}
