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

#ifndef LIBGIT2PP_REMOTE_H
#define LIBGIT2PP_REMOTE_H

#include "libgit2pp_config.h"
#include "credentials.h"

#include <memory>

struct git_remote;

namespace LibGit2pp {

/**
 * @brief Represents a git remote
 *
 * @ingroup LibGit2pp
 * @{
 */
class LIBGIT2PP_EXPORT Remote : public QObject
{
    Q_OBJECT

public:
    /**
     * @param remote The raw remote pointer. This needs to be initialized beforehand.
     *        This object takes ownership of this pointer and frees it when this object is destructed.
     * @param credentials Credentials to be used with this remote if any.
     * @param parent The parent of this QObject.
     */
    explicit Remote(git_remote *remote, const Credentials &credentials = Credentials(), QObject *parent = 0);

    /**
     * Gets the URL specified for this remote.
     */
    std::string url() const;

    /**
     * Perform a push on this Remote.
     * @param refSpecs The refspecs to use for pushing. If left empty the configured refspecs will be used.
     * @throws LibGit2pp::Exception
     */
    void push(const QList<std::string> &refSpecs = QList<std::string>());

    git_remote* data() const;

signals:
    void transferProgress(int);

private:
    Q_DISABLE_COPY(Remote)

    struct Private;
    std::shared_ptr<Private> d_ptr;
};

/** @} */

}

#endif // LIBGIT2PP_REMOTE_H
