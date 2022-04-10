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

#ifndef LIBGIT2PP_DATABASE_H
#define LIBGIT2PP_DATABASE_H

#include "libgit2pp_config.h"

#include "databasebackend.h"
#include "oid.h"

#include <string>

namespace LibGit2pp
{
    /**
     * @brief Wrapper class for git_odb.
     * Represents a Git object database containing unique sha1 object ids.
     *
     * @ingroup LibGit2pp
     * @{
     */
    class LIBGIT2PP_EXPORT Database
    {
        public:
            /**
             * Create a new object database with no backends.
             *
             * Before the ODB can be used for read/writing, a custom database
             * backend must be manually added using `addBackend()`
             *
             */
            explicit Database( git_odb *odb = 0);

            Database( const Database& other );

            ~Database();

        public:
            /**
            * Create a new object database and automatically add
            * the two default backends:
            *
            * - backendLoose: read and write loose object files
            * from disk, assuming `objects_dir` as the Objects folder
            *
            * - backendPack: read objects from packfiles,
            * assuming `objectsDir` as the Objects folder which
            * contains a 'pack/' folder with the corresponding data
            *
            * @param objectsDir path of the backends' "objects" directory.
            * @return GIT_SUCCESS if the database opened; otherwise an error
            * code describing why the open was not possible.
            */
            int open(const std::string& objectsDir);

            /**
             * Close an open object database.
             */
            void close();

            /**
             * Add a custom backend to an existing Object DB
             *
             * Read <odb_backends.h> for more information.
             *
             * @param backend pointer to a databaseBackend instance
             * @return 0 on sucess; error code otherwise
             */
            int addBackend(DatabaseBackend *backend, int priority);

            /**
            * Add a custom backend to an existing Object DB; this
            * backend will work as an alternate.
            *
            * Alternate backends are always checked for objects *after*
            * all the main backends have been exhausted.
            *
            * Writing is disabled on alternate backends.
            *
            * Read <odb_backends.h> for more information.
            *
            * @param backend pointer to a databaseBackend instance
            * @return 0 on sucess; error code otherwise
            */
            int addAlternate(DatabaseBackend *backend, int priority);

            /**
             * Determine if the given object can be found in the object database.
             *
             * @param db database to be searched for the given object.
             * @param id the object to search for.
             * @return
             * - true, if the object was found
             * - false, otherwise
             */
            int exists(Database *db, const OId& id);

            git_odb* data() const;
            const git_odb* constData() const;

        private:
            git_odb *m_database;
    };

    /**@}*/
}

#endif // LIBGIT2PP_DATABASE_H
