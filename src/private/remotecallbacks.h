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

#ifndef LIBGIT2PP_REMOTECALLBACKS_H
#define LIBGIT2PP_REMOTECALLBACKS_H

#include "git2pp/credentials.h"
#include "git2.h"

namespace LibGit2pp
{
namespace internal
{

class RemoteListener
{
public:
    virtual ~RemoteListener();

    /**
     * Return a value less than 0 for stopping the transfer.
     */
    virtual int progress(int percent) = 0;
};


class RemoteCallbacks
{
public:
    /**
     * Doesn't take ownership of \a listener.
     */
    RemoteCallbacks(RemoteListener *listener, const Credentials &credentials);

    git_remote_callbacks rawCallbacks() const;

private:
    static int transferProgressCallback(const git_transfer_progress* stats, void* data);
    static int acquireCredentialsCallback(git_cred **cred, const char *url, const char *usernameFromUrl, unsigned int allowedTypes, void *data);

    RemoteListener *m_listener;
    Credentials m_credentials;
    int m_transferProgress;
};

}
}

#endif // LIBGIT2PP_REMOTECALLBACKS_H
