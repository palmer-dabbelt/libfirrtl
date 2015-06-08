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

#include <libfirrtl/serialization.h++>
#include <libfirrtl/util/option.h++>
#include <iostream>

int main(int argc, char **argv)
{
    libfirrtl::util::option<std::string> input;
    libfirrtl::util::option<std::string> output;

    if (argc == 1) {
        std::cout << "firrtl-canon --input <infile> --output <outfile>\n"
                  << "  Canonicalizes a FIRRTL file\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        if (std::string("--input").compare(argv[i]) == 0) {
            input = argv[i+1];
            i++;
        } else if (std::string("--output").compare(argv[i]) == 0) {
            output = argv[i+1];
            i++;
        } else {
            std::cerr << "Unknown argument " << argv[i] << "\n";
            abort();
        }
    }

    auto circuit = libfirrtl::parse_xml(input.value());
    libfirrtl::write_xml(circuit, output.value());
}
