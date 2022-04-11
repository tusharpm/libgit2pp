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

#ifndef LIBGIT2PP_CHECKOUTOPTIONS_H
#define LIBGIT2PP_CHECKOUTOPTIONS_H

#include "git2.h"
#include <list>
#include <memory>
#include "libgit2pp_config.h"
#include "libgit2pp_types.h"

namespace LibGit2pp
{
    /**
     * Options that specify how a checkout operation is performed.
     *
     * @ingroup LibGit2pp
     * @{
     */
    class LIBGIT2PP_EXPORT CheckoutOptions
    {
    public:
        /**
         * A checkout strategy.
         */
        enum Strategy {
            None,         ///< No actual updates
            Safe,         ///< Allow safe updates that cannot overwrite uncommitted data
            Force         ///< Allow all updates to force working directory to look like index
        };

        /**
         * Options specifying details about how a checkout is performed.
         */
        enum Flag {
            AllowConflicts = 1u << 0,    ///< Allow checkout to make safe updates even if conflicts are found
            RemoveUntracked = 1u << 1,   ///< Remove untracked files not in index (that are not ignored)
            RemoveIgnored = 1u << 2,     ///< Remove ignored files not in index
            UpdateOnly = 1u << 3,        ///< Only update existing files, don't create new ones
            DontUpdateIndex = 1u << 4,   ///< Normally checkout updates index entries as it goes; this stops that
            NoRefresh = 1u << 5,         ///< Don't refresh index/config/etc before doing checkout
            SkipUnmerged = 1u << 6,      ///< Allow checkout to skip unmerged files
            UnmergedUseOurs = 1u << 7,   ///< For unmerged files, checkout stage 2 from index
            UnmergedUseTheirs = 1u << 8, ///< For unmerged files, checkout stage 3 from index
            RecreateMissing = 1u << 9    ///< Allow checkout to recreate missing files
        };
        using Flags = FlagsFromEnumeration<Flag>;

        /**
         * Constructs a new CheckoutOptions.
         * @param strategy The checkout strategy to use.
         * @param flags Details about the checkout process.
         */
        CheckoutOptions(Strategy strategy = None, Flags flags = Flags());

        /**
         * Set the target directory where the files will be checked out.
         * The default is to use the working directory.
         */
        void setTargetDirectory(const std::string &dir);

        /**
         * Sets the paths which are checked out.
         * These can be exact path names or wildcard matchers like `*.c`.
         * The wildcard syntax is that accepted by the POSIX \c fnmatch function.
         * @param paths The paths to checkout.
         */
        void setPaths(const std::list<std::string> &paths);

        const git_checkout_options* data() const;

    private:
        class Private;
        std::shared_ptr<Private> d_ptr;
    };

    /** @} */

}

#endif // LIBGIT2PP_CHECKOUTOPTIONS_H
