/******************************************************************************
 * This file is part of the libgit2pp library
 * Copyright (C) 2013 Leonardo Giordani
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

#ifndef LIBGIT2PP_STATUS_LIST_H
#define LIBGIT2PP_STATUS_LIST_H

#include <memory>

#include "git2.h"

#include "libgit2pp_config.h"
#include "statusentry.h"

namespace LibGit2pp
{
/**
 * @brief Wrapper class for git_status_list.
 *
 * Represents a list of status entries in a Git repository. This is not a simple QList of StatusEntry,
 * it wraps the underlying libgit2 functions.
 *
 * @ingroup LibGit2pp
 * @{
 */
class LIBGIT2PP_EXPORT StatusList
{
public:
    explicit StatusList(git_status_list *status_list = 0);

    StatusList(const StatusList& other);

    ~StatusList();

    /**
     * Returns the number of entries in the status list.
     */
    size_t entryCount();

    /**
     * Returns the entry with the given index.
     */
    const StatusEntry entryByIndex(size_t idx);

    git_status_list* data() const;
    const git_status_list* constData() const;

private:
    typedef std::shared_ptr<git_status_list> ptr_type;
    ptr_type d;

};


/**@}*/
}

#endif // LIBGIT2PP_STATUS_LIST_H
