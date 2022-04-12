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

#include "git2pp/config.h"

#include "git2pp/exception.h"
#include "git2pp/repository.h"
#include "private/buffer.h"
#include "private/pathcodec.h"

namespace LibGit2pp
{

Config::Config(git_config *cfg)
    : d(cfg)
{
    if (d == 0)
        git_config_new(&d);
}

Config::Config(const Config &other)
    : d(other.d)
{
}

Config::~Config()
{
    git_config_free(d);
}

Config Config::fromGlobalConfig()
{
    git_config * def;
    git_config * cfg;
    git_config_open_default(&def);
    if ( git_config_open_global(&cfg, def) == GIT_OK )
        return Config(cfg);

    return Config();
}

std::string Config::findGlobal()
{
    internal::Buffer buffer;
    qGitThrow(git_config_find_global(buffer.data()));

    return buffer.asPath();
}

std::string Config::findSystem()
{
    internal::Buffer buffer;
    qGitThrow(git_config_find_system(buffer.data()));

    return buffer.asPath();
}

bool Config::append(const std::string &path, git_config_level_t level, const Repository &repo, bool force)
{
    return GIT_OK == git_config_add_file_ondisk(d, internal::PathCodec::toLibGit2(path).data(), level, repo.constData(), force);
}

std::optional<std::string> Config::value(const std::string &key) const
{
    const char * result = 0;
    if (git_config_get_string(&result, d, key.c_str()) == GIT_OK)
        return std::string(result);

    return {};
}

void Config::setValue(const std::string &key, const std::string &value)
{
    qGitThrow(git_config_set_string(d, key.c_str(), value.c_str()) );
}


} // LibGit2pp

