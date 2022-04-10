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

#include "git2pp/statuslist.h"

namespace LibGit2pp
{

StatusList::StatusList(git_status_list *status_list)
    : d(status_list, git_status_list_free)
{
}

StatusList::StatusList(const StatusList &other)
    : d(other.d)
{
}

StatusList::~StatusList()
{
}

size_t StatusList::entryCount()
{
    return git_status_list_entrycount(data());
}

const StatusEntry StatusList::entryByIndex(size_t idx)
{
    return StatusEntry(git_status_byindex(data(), idx));
}

git_status_list* StatusList::data() const
{
    return d.get();
}

const git_status_list* StatusList::constData() const
{
    return d.get();
}

}
