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

#ifndef LIBGIT2PP_CREDENTIALS_H
#define LIBGIT2PP_CREDENTIALS_H

#include <memory>

#include "libgit2pp_config.h"

struct git_cred;

namespace LibGit2pp
{

class CredentialsPrivate;

/**
 * @brief Credentials are used to authenticate communication
 *
 * There can be different types of credentials. What types of credentials are needed depends
 * on the communication method. Credentials can be for example a user name/password combination
 * or a public key/private key pair for SSH. The user must choose a correct type of \c Credentials
 * or otherwise the authentication can fail.
 *
 * @ingroup LibGit2pp
 * @{
 */
class LIBGIT2PP_EXPORT Credentials
{
public:
    /**
     * An empty Credentials. This does not authenticate any communication.
     */
    Credentials();

    bool isEmpty() const;

    /**
     * @brief Creates \c Credentials suitable for public key SSH authentication.
     *
     * @param privateKeyPath path to the private key file
     * @param publicKeyPath path to the public key file
     * @param userName the user name to use in the authentication
     * @param passphrase the passphrase for the private key
     * @return a new Credentials object
     */
    static Credentials ssh(const std::string &privateKeyPath, const std::string &publicKeyPath = std::string(), const QByteArray &userName = QByteArray(), const QByteArray &passphrase = QByteArray());

private:
    std::shared_ptr<CredentialsPrivate> d_ptr;
    Credentials(CredentialsPrivate &p);
};

/** @} */

}

#endif // LIBGIT2PP_CREDENTIALS_H
