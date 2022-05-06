/******************************************************************************
 * This file is part of the libgit2pp library
 * Copyright (c) 2012 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef LIBGIT2PP_CONFIG_H
#define LIBGIT2PP_CONFIG_H

#include <optional>

#include "git2.h"

#include "libgit2pp_config.h"

#include "repository.h"

namespace LibGit2pp
{
    /**
      * @brief Represents the git configuration file.
      *
      * @ingroup LibGit2pp
      * @{
      */
    class LIBGIT2PP_EXPORT Config
    {
    public:
        /**
          * Default constructor to create a new configuration object.
          *
          * @param cfg when given, the instance is used instead of creating a new one
          */
        Config(git_config *cfg = 0);
        Config(const Config &other);
        virtual ~Config();

        /**
          * Creates a new configuration object and adds the global Git configuration when found.
          * Otherwise an empty configuration object is created.
          *
          * @return the new instance
          */
        static Config fromGlobalConfig();

        /**
          * Appends a config file with the given access priority.
          *
          * @param path the absolute path to the config file
          * @param priority the access priority; values with higher priority are accessed first
          * @param repo optional repository to allow parsing of conditional includes
          * @param force replace config file at the given priority level
          *
          * @return true on success
          */
        bool append(const std::string& path, git_config_level_t level, const Repository &repo = Repository(), bool force = false);

        /**
          * Reads a single value from the configuration.
          *
          * @return the value as QVariant or an empty QVariant instance
          */
        std::optional<std::string> value(const std::string &key) const;

        /**
          * Writes a value in the configuration with the highest priority.
          *
          * @param key the name of the value to write
          * @param value the value
          */
        void setValue(const std::string &key, const std::string &value);

        /**
          * Remove a value from the configuration.
          *
          * @param key the name for the value to remove
          */
        void remove(const std::string &key);

    public:
        /**
          * Searches for the global configuration file located in $HOME.
          * @see git_config_find_global
          */
        static std::string findGlobal();

        /**
          * Searches for the system configuration file.
          * @see git_config_find_system
          */
        static std::string findSystem();

    private:
        git_config *    d; //!< internal pointer to the libgit2 config instance
    };

    /**@}*/
}

#endif // LIBGIT2PP_CONFIG_H
