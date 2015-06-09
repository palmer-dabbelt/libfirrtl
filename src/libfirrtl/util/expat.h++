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

#ifndef LIBFIRRTL__UTIL__EXPAT_HXX
#define LIBFIRRTL__UTIL__EXPAT_HXX

#include "withptr.h++"
#include "option.h++"
#include <expat.h>
#include <functional>
#include <string>
#include <vector>

namespace libfirrtl {
    namespace util {
        /* The error codes that can come out of expat. */
        class expat_error: public withptr<expat_error> {
        private:
            option<XML_Error> _code;

        public:
            expat_error(void)
                : _code()
                  {}

            expat_error(XML_Error code)
                : _code(code)
                {}

        public:
            bool has_error(void) { return _code.valid(); }
            XML_Error code(void) { return _code.value(); }
            const std::string name(void) { return XML_ErrorString(code()); }
        };

        struct expat_attr {
            const std::string key;
            const std::string val;

            expat_attr(const std::string& _key, const std::string& _val)
                : key(_key),
                  val(_val)
                {}
        };

        /* Wraps up Expat in a way such that it can use C++11 lambdas
         * for callbacks, and so the user doesn't have to
         * allocate/free anything.*/
        class expat: public withptr<expat> {
        public:
            typedef std::function<void(const std::string&,
                                       const std::vector<expat_attr>&)>
            start_element_handler_t;

            typedef std::function<void(const std::string&)>
            end_element_handler_t;

        private:
            XML_Parser _parser;

            option<start_element_handler_t> _start_element_handler;
            option<end_element_handler_t>   _end_element_handler;

        public:
            /* Creates a new parser, reading the encoding from the
             * file. */
            expat(void);

            ~expat(void);

        public:
            /* Mirrors how Expat registers callbacks. */
            void set_start_element_handler(start_element_handler_t h)
                { _start_element_handler = h; }
            void set_end_element_handler  (end_element_handler_t   h)
                { _end_element_handler = h; }

        public:
            /* Reads a file into the parser, but doesn't finalize the
             * parser (that will happen when */
             expat_error::ptr read_file(const std::string filename);

        private:
            /* Wrapper functions for expat. */
            static void expat_start_element_handler(void *self,
                                                    const char *name,
                                                    const char **attrs);
            static void expat_end_element_handler(void *self,
                                                  const char *name);
        };
    }
}

#endif
