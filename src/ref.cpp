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

#include "git2pp/ref.h"

#include "git2pp/oid.h"
#include "git2pp/repository.h"
#include "git2pp/exception.h"
#include "private/pathcodec.h"

namespace LibGit2pp
{

Reference::Reference(git_reference *ref)
    : d(ref, git_reference_free)
{
}

Reference::Reference(const Reference& other)
    : d(other.d)
{
}

Reference::~Reference()
{
}

OId Reference::target() const
{
    return OId(git_reference_target(d.data()));
}

std::string Reference::symbolicTarget() const
{
    return std::string::fromUtf8(git_reference_symbolic_target(d.data()));
}

bool Reference::isDirect() const
{
    return git_reference_type(d.data()) == GIT_REF_OID;
}

bool Reference::isSymbolic() const
{
    return git_reference_type(d.data()) == GIT_REF_SYMBOLIC;
}

std::string Reference::name() const
{
    return git_reference_name(d.data());
}

Reference Reference::resolve() const
{
    git_reference *ref;
    qGitThrow(git_reference_resolve(&ref, d.data()));
    return Reference(ref);
}

Repository Reference::owner() const
{
    return Repository(git_reference_owner(d.data()));
}

void Reference::setSymbolicTarget(const std::string& target, const std::string &message)
{
    git_reference* rp;
    qGitThrow(git_reference_symbolic_set_target(&rp, data(), PathCodec::toLibGit2(target), message.toUtf8()));
    d = ptr_type(rp, git_reference_free);
}

void Reference::setTarget(const OId& oid, const std::string &message)
{
    git_reference* rp;
    qGitThrow(git_reference_set_target(&rp, data(), oid.constData(), message.toUtf8()));
    d = ptr_type(rp, git_reference_free);
}

bool Reference::isNull() const
{
    return data() == 0;
}

git_reference* Reference::data() const
{
    return d.data();
}

const git_reference* Reference::constData() const
{
    return d.data();
}

} // namespace LibGit2pp
