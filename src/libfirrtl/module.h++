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

#ifndef LIBFIRRTL__MODULE_HXX
#define LIBFIRRTL__MODULE_HXX

#include "util/withptr.h++"
#include "port.h++"
#include <string>
#include <vector>

namespace libfirrtl {
    /* Stores a single module */
    class module: public util::withptr<module> {
    private:
        std::string _name;

        /* The ports are stored  */
        std::vector<input_port::ptr> _inputs;
        std::vector<output_port::ptr> _outputs;

    public:
        /* FIXME: Modules need to have more than just names. */
        module(const decltype(_name)& name);

    public:
        const decltype(_name)& name(void) const { return _name; }
        const decltype(_inputs)& inputs(void) const { return _inputs; }
        const decltype(_outputs)& outputs(void) const { return _outputs; }
    };
}

#endif
