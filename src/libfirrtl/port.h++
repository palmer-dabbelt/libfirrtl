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

#ifndef LIBFIRRTL__PORT_HXX
#define LIBFIRRTL__PORT_HXX

#include "util/withptr.h++"
#include "util/option.h++"

namespace libfirrtl {
    enum class port_direction {
        INPUT,
        OUTPUT
    };

    /* A super-class for all port types, which can be either inputs or
     * outputs. */
    class port {
    private:
        const std::string _name;
        const util::option<unsigned> _width;

    public:
        port(const std::string& name)
            : _name(name)
            {}

    public:
        /* Accessor methods. */
        const decltype(_name)& name(void) const { return _name; }

    public:
        virtual enum port_direction direction(void) = 0;
    };

    /* This template simply fills in the correct direction for a port,
     * so we get some amount of runtime configuration.  This sort of
     * approximatess a scala match statement, but it's a bit
     * clunky. */
    template<enum port_direction dir>
    class port_t: public port {
    public:
        port_t(const std::string& name)
            : port(name)
            {}

        enum port_direction direction(void) { return dir; }
    };

    /* Ports can be either inputs or outputs.  These types just
     * provide some convenient names for them. */
    class input_port:
        public port_t<port_direction::INPUT>,
        public util::withptr<input_port> {
    };

    class output_port:
        public port_t<port_direction::OUTPUT>,
        public util::withptr<output_port> {
    };
}

#endif
