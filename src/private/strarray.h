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

#ifndef LIBGIT2PP_STRARRAY_H
#define LIBGIT2PP_STRARRAY_H

#include <vector>
#include "git2.h"
#include "git2pp/libgit2pp_types.h"

namespace LibGit2pp {
namespace internal {

class StrArray
{
public:
    explicit StrArray(const std::vector<QByteArray> &list = {});
    StrArray(const StrArray &other) = delete;
    StrArray(StrArray &&other);
    ~StrArray();

    StrArray &operator=(const StrArray &rhs) = delete;
    StrArray &operator=(StrArray &&rhs);

    size_t count() const;

    const git_strarray& data() const;

private:
    std::vector<QByteArray> m_strings;
    git_strarray m_data{nullptr, 0};
};

}
}

#endif // LIBGIT2PP_STRARRAY_H
