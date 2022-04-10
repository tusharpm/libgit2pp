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

#include "git2pp/rebase.h"
#include "git2pp/oid.h"
#include "git2pp/exception.h"

namespace LibGit2pp
{

struct Rebase::Private {
    Private(git_rebase *rebase)
        : mRebase(rebase, git_rebase_free)
    {
    }

    void abort()
    {
        qGitThrow(git_rebase_abort(data()));
    }

    OId commit(const Signature &committer, const Signature &author, const std::string &message)
    {
        git_oid oid;
        qGitThrow(git_rebase_commit(&oid, data(), author.data(), committer.data(), NULL, message.empty() ? NULL : message.c_str()));
        return OId(&oid);
    }

    void finish(const Signature &signature)
    {
        qGitThrow(git_rebase_finish(data(), signature.data()));
    }

    bool next()
    {
        git_rebase_operation *op;
        int error = git_rebase_next(&op, data());
        if (error == GIT_ITEROVER) {
            return false;
        } else {
            qGitThrow(error);
            return true;
        }
    }

    git_rebase *data() const
    {
        return mRebase.get();
    }

    const git_rebase *constData() const
    {
        return mRebase.get();
    }

private:
    std::shared_ptr<git_rebase> mRebase;
};

Rebase::Rebase(git_rebase *rebase)
    : d_ptr(new Private(rebase))
{
}

void Rebase::abort()
{
    return d_ptr->abort();
}

OId Rebase::commit(const Signature &committer, const Signature &author, const std::string &message)
{
    return d_ptr->commit(committer, author, message);
}

void Rebase::finish(const Signature &signature)
{
    d_ptr->finish(signature);
}

bool Rebase::next()
{
    return d_ptr->next();
}

git_rebase *Rebase::data() const
{
    return d_ptr->data();
}

git_rebase *Rebase::constData() const
{
    return d_ptr->data();
}
}
