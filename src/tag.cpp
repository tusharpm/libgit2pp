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

#include "git2pp/tag.h"
#include "git2pp/oid.h"
#include "git2pp/signature.h"
#include "git2pp/exception.h"

namespace LibGit2pp
{

Tag::Tag(git_tag *tag)
    : Object(reinterpret_cast<git_object*>(tag))
{
}

Tag::Tag(const Tag& other)
    : Object(other)
{
}

Tag::~Tag()
{
}

OId Tag::oid() const
{
    return OId(git_tag_id(data()));
}

Object Tag::target() const
{
    git_object *obj;
    qGitThrow(git_tag_target(&obj, data()));
    return Object(obj);
}

const std::string Tag::name() const
{
    return git_tag_name(data());
}

Signature Tag::tagger() const
{
    return Signature(git_tag_tagger(data()));
}

const std::string Tag::message()
{
    return git_tag_message(data());
}

git_tag* Tag::data() const
{
    return reinterpret_cast<git_tag*>(Object::data());
}

const git_tag* Tag::constData() const
{
    return reinterpret_cast<git_tag*>(Object::data());
}

} // namespace LibGit2pp
