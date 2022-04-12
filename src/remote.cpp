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

#include "git2pp/remote.h"
#include "git2pp/exception.h"
#include "private/remotecallbacks.h"
#include "private/strarray.h"

#include "git2.h"

namespace LibGit2pp {

struct Remote::Private : public internal::RemoteListener
{
    Private(Remote &parent, git_remote *remote, const Credentials &credentials) :
        m_data(remote, git_remote_free),
        m_parent(parent),
        m_callbacks(this, credentials)
    {
    }

    int progress(int percent)
    {
        m_parent.transferProgress(percent);
        return 0;
    }

    void push(const std::list<std::string> &refSpecs)
    {
        std::list<QByteArray> baRefSpecs;
        for (const std::string &ref : refSpecs) {
            baRefSpecs.append(ref.toLatin1());
        }
        internal::StrArray refspecs(baRefSpecs);

        git_push_options opts = GIT_PUSH_OPTIONS_INIT;
        opts.callbacks = m_callbacks.rawCallbacks();
        qGitThrow(git_remote_push(m_data.get(), &refspecs.data(), &opts));
    }

    std::shared_ptr<git_remote> m_data;

private:
    Remote &m_parent;
    internal::RemoteCallbacks m_callbacks;
};


Remote::Remote(git_remote *remote, const Credentials &credentials) :
    d_ptr(new Private(*this, remote, credentials))
{
}

std::string Remote::url() const
{
    return std::string::fromLatin1(git_remote_url(data()));
}

void Remote::push(const std::list<std::string> &refSpecs)
{
    d_ptr->push(refSpecs);
}

git_remote* Remote::data() const
{
    return d_ptr->m_data.get();
}

}
