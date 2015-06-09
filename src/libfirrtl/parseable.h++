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

#ifndef LIBFIRRTL__PARSABLE_HXX
#define LIBFIRRTL__PARSABLE_HXX

#include "util/withptr.h++"
#include <vector>

namespace libfirrtl {
    enum class parseable_type {
        CIRCUIT,
        MODULE,
    };

    class parseable {
    private:
        std::vector<std::shared_ptr<parseable>> _children;

    public:
        virtual enum parseable_type get_type(void) = 0;

    public:
        void add_child(const std::shared_ptr<parseable> child)
            { _children.push_back(child); }
    };

    template<enum parseable_type type>
    class parseable_t: public parseable {
        virtual enum parseable_type get_type(void) { return type; }
    };

    class parseable_circuit:
        public parseable_t<parseable_type::CIRCUIT>,
        public util::withptr<parseable_circuit>
    {
    };

    class parseable_module:
        public parseable_t<parseable_type::MODULE>,
        public util::withptr<parseable_module>
    {
    private:
        std::string _name;

    public:
        parseable_module(const std::string& name)
            : _name(name)
            {}

    public:
        const decltype(_name)& name(void) { return _name; }
    };
}

#endif
