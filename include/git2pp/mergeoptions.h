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

#ifndef LIBGIT2PP_MERGEOPTIONS_H
#define LIBGIT2PP_MERGEOPTIONS_H

#include "git2.h"

#include "libgit2pp_config.h"
#include "libgit2pp_flags.h"

namespace LibGit2pp
{
    /**
     * Options that specify how a merge operation is performed.
     *
     * @ingroup LibGit2pp
     * @{
     */
    class LIBGIT2PP_EXPORT MergeOptions
    {
    public:
        /**
         * Strategy to handle conflicting regions when merging a file.
         */
        enum FavorType {
            Normal,       ///< Record a conflict in the index, inserting conflict markers in the working directory file.
            Ours,         ///< Use "ours" side version for the conflicting regions.
            Theirs,       ///< Use "theirs" side version for the conflicting regions.
            Union         ///< Combine lines from both "ours" and "theirs" side for the conflicting regions.
        };

        /**
         * Options specifying details about how a checkout is performed.
         */
        enum Flag {
            FindRenames = 1u << 0   ///< Enables merging between a renamed and modified file
        };
        using Flags = FlagsFromEnumeration<Flag>;

        /**
         * Construct a new MergeOptions.
         * @param flags Set details about the merge process.
         * @param favor How conflicting regions in files should be handled.
         */
        MergeOptions(FavorType favor = Normal, Flags flags = Flags());

        const git_merge_options* data() const;

    private:
        git_merge_options d;
    };

    /** @} */

}

#endif // LIBGIT2PP_MERGEOPTIONS_H
