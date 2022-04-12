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

#include "pathcodec.h"

namespace LibGit2pp
{
namespace internal
{

// POSIX emulation in libgit2 assumes all i/o to be UTF-8 encoded
QByteArray PathCodec::toLibGit2(const std::string &path)
{
    return {path.begin(), path.end()};
}

std::string PathCodec::fromLibGit2(const QByteArray &path)
{
    return path.data();
}

std::string PathCodec::fromLibGit2(const char *path)
{
    return fromLibGit2(QByteArray(path));
}

}
}
