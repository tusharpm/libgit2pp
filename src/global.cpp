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

#include "git2pp/global.h"

#include "git2.h"

#include <atomic>

namespace LibGit2pp
{

namespace internal {
const int Uninitialized = 0;
const int Initialized = 1;

std::atomic_int LibInitialized(Uninitialized);
}

using namespace internal;

bool initLibGit2pp() {
    bool ret = false;
    if (LibInitialized.fetch_add(Initialized, std::memory_order_relaxed) == Uninitialized) {
        git_libgit2_init();
        ret = true;
    }
    return ret;
}

bool shutdownLibGit2pp() {
    bool ret = false;
    if (LibInitialized.load() > Uninitialized) {
        if (LibInitialized.fetch_add(-Initialized, std::memory_order_relaxed) == Initialized) {
            git_libgit2_shutdown();
            ret = true;
        }
    }
    return ret;
}

}
