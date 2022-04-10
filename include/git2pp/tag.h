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

#ifndef LIBGIT2PP_TAG_H
#define LIBGIT2PP_TAG_H

#include "object.h"

namespace LibGit2pp
{
    class OId;
    class Signature;

    /**
     * @brief Wrapper class for git_tag.
     * Represents a Git tag object.
     *
     * @ingroup LibGit2pp
     * @{
     */
    class LIBGIT2PP_EXPORT Tag : public Object
    {
        public:

            /**
             * Creates a Tag that points to tag. The pointer object becomes managed by
             * this Tag, and must not be passed to another Tag or closed outside this
             * object.
             */
            explicit Tag(git_tag *tag = 0);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            Tag(const Tag& other);

            /**
             * Destroys the object.
             */
            ~Tag();

            /**
             * Get the id of a tag.
             * @return object identity for the tag.
             */
            OId oid() const;

            /**
             * Get the tagged object of a tag
             *
             * @throws LibGit2pp::Exception
             */
            Object target() const;

            /**
             * Get the name of a tag
             * @return name of the tag
             */
            const std::string name() const;

            /**
             * Get the tagger (author) of a tag
             * @return reference to the tag's author
             */
            Signature tagger() const;

            /**
             * Get the message of a tag
             * @return message of the tag
             */
            const std::string message();

            git_tag* data() const;
            const git_tag* constData() const;
    };

    /**@}*/
}

#endif // LIBGIT2PP_TAG_H
