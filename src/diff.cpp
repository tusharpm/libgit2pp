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

#include "git2pp/diff.h"
#include "git2pp/diffdelta.h"

namespace LibGit2pp
{

Diff::Diff(git_diff *diff) :
    d(diff, git_diff_free)
{
}

size_t Diff::numDeltas() const
{
    if (d) {
        return git_diff_num_deltas(d.get());
    }
    return 0;
}

DiffDelta Diff::delta(size_t index) const
{
    const git_diff_delta *delta = 0;
    if (d) {
        delta = git_diff_get_delta(d.get(), index);
    }
    return DiffDelta(delta);
}

}
