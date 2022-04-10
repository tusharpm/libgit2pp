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

#ifndef LIBGIT2PP_DIFF_H
#define LIBGIT2PP_DIFF_H

#include <memory>

#include "git2.h"

#include "libgit2pp_config.h"

namespace LibGit2pp
{

class DiffDelta;

/**
 * This class represents a diff.
 */
class LIBGIT2PP_EXPORT Diff
{
public:
    Diff(git_diff *diff = 0);

    /**
     * @brief Get the number of \c DiffDelta objects in this \c Diff.
     */
    size_t numDeltas() const;

    /**
     * @brief Get the \c DiffDelta for the specified index.
     *
     * If the given \a index is not on a valid range then a \c NULL \c DiffDelta
     * object is returned.
     *
     * @param index an index from the interval 0 <= index < numDeltas().
     * @return The \c DiffDelta member.
     */
    DiffDelta delta(size_t index) const;

public:
    QSharedPointer<git_diff> d;
};

}

#endif // LIBGIT2PP_DIFF_H
