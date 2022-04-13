/******************************************************************************
 * This file is part of the libgit2pp library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#include "git2pp/oid.h"
#include <algorithm>
#include "git2pp/exception.h"

namespace LibGit2pp
{

OId::OId(const git_oid* oid)
    : d(GIT_OID_RAWSZ, 0)
{
    if (oid != 0) {
        git_oid_cpy(data(), oid);
    }
}

OId::OId(const OId& other)
    : d(other.d)
{
}

OId::~OId()
{
}

bool OId::isValid() const
{
    return ( !d.empty() &&
             (d.size() <= GIT_OID_RAWSZ) &&
             (d != QByteArray(GIT_OID_RAWSZ, 0))
             );
}

void OId::fromHex(const std::string& hex)
{
    int len = std::min(hex.size(), std::size_t(GIT_OID_HEXSZ));
    qGitThrow(git_oid_fromstrn(data(), hex.data(), len));
    d.resize(len / 2);
}

void OId::fromString(const std::string& string)
{
    fromHex(string);
}


void OId::fromRawData(const QByteArray& raw)
{
    qGitThrow(raw.size() < GIT_OID_HEXSZ);
    d = raw;
}

OId OId::stringToOid(const std::string& string)
{
    OId oid;
    oid.fromHex(string);
    return oid;
}

OId OId::rawDataToOid(const QByteArray& raw)
{
    OId oid;
    oid.d = raw;
    return oid;
}

std::string OId::format() const
{
    std::string ba(GIT_OID_HEXSZ, '\0');
    git_oid_fmt(ba.data(), constData());
    return ba;
}

std::string OId::pathFormat() const
{
    std::string ba(GIT_OID_HEXSZ+1, '\0');
    git_oid_pathfmt(ba.data(), constData());
    return ba;
}

git_oid* OId::data()
{
    return reinterpret_cast<git_oid*>(d.data());
}

const git_oid* OId::constData() const
{
    return reinterpret_cast<const git_oid*>(d.data());
}

bool operator ==(const OId &oid1, const OId &oid2)
{
    return git_oid_cmp(oid1.constData(), oid2.constData()) == 0;
}

bool operator !=(const OId &oid1, const OId &oid2)
{
    return !(oid1 == oid2);
}

int OId::length() const
{
    return d.size() * 2;
}

} // LibGit2pp