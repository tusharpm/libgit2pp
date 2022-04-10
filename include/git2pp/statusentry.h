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

#ifndef LIBGIT2PP_STATUS_ENTRY_H
#define LIBGIT2PP_STATUS_ENTRY_H

#include "libgit2pp_config.h"

#include "git2.h"

namespace LibGit2pp
{

class Status;
class DiffDelta;

/**
 * @brief Wrapper class for git_status_entry.
 *
 * Represents a status entry in a Git repository, that is a Git status linked to a file name.
 * Actually the status entry encompasses two file names, to take renames into account.
 *
 * @ingroup LibGit2pp
 * @{
 */
class LIBGIT2PP_EXPORT StatusEntry
{
public:

    explicit StatusEntry(const git_status_entry *entry);

    StatusEntry(const StatusEntry& other);

    ~StatusEntry();

    /**
     * Return the status of the entry
     */
    Status status() const;

    /**
     * Returns the diff between HEAD and index.
     */
    DiffDelta headToIndex() const;

    /**
     * Returns the diff between index and working directory.
     */
    DiffDelta indexToWorkdir() const;

private:
    const git_status_entry* d;
};

/**@}*/
}

#endif // LIBGIT2PP_STATUS_ENTRY_H
