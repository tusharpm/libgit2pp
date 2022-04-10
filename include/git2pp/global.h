/******************************************************************************
 * This file is part of the libgit2pp library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef LIBGIT2PP_GLOBAL_H
#define LIBGIT2PP_GLOBAL_H

#include "libgit2pp_config.h"

namespace LibGit2pp
{
    /**
     * This method should be called once before using any other parts of LibGit2pp.
     *
     * This method can be called more than once - the subsequent calls have no extra
     * effect. But note that the \c shutdownLibGit2pp() method should be called (at least)
     * as many times as \c initLibGit2pp() has been called.
     *
     * @return true when the initialization happens.
     */
    LIBGIT2PP_EXPORT bool initLibGit2pp();

    /**
     * This method should be called when done with LibGit2pp.
     *
     * This method must be called (at least) as many times as \c initLibGit2pp() has been
     * called. Only after the same amount of calls have been made to this method the
     * library actually gets uninitialized.
     *
     * @return true when the shutdown happens.
     */
    LIBGIT2PP_EXPORT bool shutdownLibGit2pp();
}

#endif // LIBGIT2PP_GLOBAL_H
