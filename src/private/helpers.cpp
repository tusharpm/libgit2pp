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

#include "helpers.h"

namespace LibGit2pp
{

bool starts_with(const std::string_view haystack, const std::string_view needle)
{
    if (haystack.length() < needle.length()) {
        return false;
    }

    return haystack.compare(0, needle.length(), needle) == 0;
}

std::string join(const std::vector<std::string>& list, const std::string_view delimiter)
{
    bool first = true;
    std::string result;

    for (const std::string& str : list) {
        if (first) {
            first = false;
        } else {
            result += delimiter;
        }
        result += str;
    }

    return result;
}

}