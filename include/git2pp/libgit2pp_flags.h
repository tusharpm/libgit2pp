/******************************************************************************
 * This file is part of the libgit2pp library
 * Copyright (C) 2022 Tushar Maheshwari <tushar27192@gmail.com>
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

#ifndef LIBGIT2PP_LIB_FLAGS_H
#define LIBGIT2PP_LIB_FLAGS_H

namespace LibGit2pp
{

template <typename T>
class FlagsFromEnumeration
{
public:
    FlagsFromEnumeration() : data{0} {}
    FlagsFromEnumeration(T value) : data{value} {}

    operator T() const {
        return static_cast<T>(data);
    }

private:
    std::underlying_type_t<T> data;
};

}

#endif // LIBGIT2PP_LIB_FLAGS_H
