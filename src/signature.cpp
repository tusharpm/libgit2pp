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

#include "git2pp/signature.h"
#include "git2pp/exception.h"

namespace LibGit2pp
{

class Signature::Private {
    static void do_not_free(git_signature*) {}
public:
    Private(const git_signature *signature, bool own) :
        signature(const_cast<git_signature*>(signature), own ? git_signature_free : do_not_free)
    {
    }

    std::shared_ptr<git_signature> signature;
};

Signature::Signature(const std::string& name, const std::string& email, ZonedTime dateTime)
{
    git_signature *sig = 0;
    qGitThrow(git_signature_new(&sig, name.c_str(), email.c_str(), dateTime.time, dateTime.offset));
    d_ptr = std::make_shared<Private>(sig, true);
}

Signature::Signature(const std::string& name, const std::string& email)
{
    git_signature *sig = 0;
    qGitThrow(git_signature_now(&sig, name.c_str(), email.c_str()));
    d_ptr = std::make_shared<Private>(sig, true);
}

Signature::Signature(const git_signature *signature) :
    d_ptr(std::make_shared<Private>(signature, false))
{
}

std::string Signature::name() const
{
    std::string ret;
    if (d_ptr->signature) {
        ret = d_ptr->signature->name;
    }
    return ret;
}

std::string Signature::email() const
{
    std::string ret;
    if (d_ptr->signature) {
        ret = d_ptr->signature->email;
    }
    return ret;
}

ZonedTime Signature::when() const
{
    ZonedTime dt{};
    if (d_ptr->signature) {
        dt = d_ptr->signature.when;
    }
    return dt;
}

const git_signature *Signature::data() const
{
    return d_ptr->signature.get();
}

} // namespace LibGit2pp
