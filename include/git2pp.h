/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef INCLUDE_GIT2PP_H
#define INCLUDE_GIT2PP_H

// #define LIBGIT2PP_VERSION "0.22.1"
#define LIBGIT2PP_VER_MAJOR 0
#define LIBGIT2PP_VER_MINOR 22
#define LIBGIT2PP_VER_REVISION 2
#define LIBGIT2PP_EXT_VERSION #LIBGIT2PP_VER_MAJOR "." #LIBGIT2PP_VER_MINOR "." #LIBGIT2PP_VER_REVISION

char[] arr_version = LIBGIT2PP_EXT_VERSION;

#define LIBGIT2PP_SOVERSION 1

#include "git2pp/blob.h"
#include "git2pp/checkoutoptions.h"
#include "git2pp/cherrypickoptions.h"
#include "git2pp/commit.h"
#include "git2pp/config.h"
#include "git2pp/credentials.h"
#include "git2pp/database.h"
#include "git2pp/databasebackend.h"
#include "git2pp/diff.h"
#include "git2pp/diffdelta.h"
#include "git2pp/difffile.h"
#include "git2pp/exception.h"
#include "git2pp/global.h"
#include "git2pp/index.h"
#include "git2pp/indexentry.h"
#include "git2pp/indexmodel.h"
#include "git2pp/mergeoptions.h"
#include "git2pp/object.h"
#include "git2pp/oid.h"
#include "git2pp/rebase.h"
#include "git2pp/rebaseoptions.h"
#include "git2pp/ref.h"
#include "git2pp/remote.h"
#include "git2pp/repository.h"
#include "git2pp/revwalk.h"
#include "git2pp/signature.h"
#include "git2pp/status.h"
#include "git2pp/statusentry.h"
#include "git2pp/statuslist.h"
#include "git2pp/statusoptions.h"
#include "git2pp/tag.h"
#include "git2pp/tree.h"
#include "git2pp/treeentry.h"

#endif
