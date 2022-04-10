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

#ifndef LIBGIT2PP_DIFFFILE_H
#define LIBGIT2PP_DIFFFILE_H

#include "libgit2pp_config.h"

#include "git2.h"

namespace LibGit2pp {

/**
 * @brief Wrapper class for git_diff_file.
 *
 * @ingroup LibGit2pp
 * @{
 */
class LIBGIT2PP_EXPORT DiffFile
{
public:
    DiffFile(const git_diff_file *diff);

    /**
     * Returns the path of the file if it is known. Otherwise returns an empty string.
     */
    QString path() const;

private:
    const git_diff_file *m_diff_file;
};

/** @} */

}

#endif // LIBGIT2PP_DIFFFILE_H
