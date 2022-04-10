/******************************************************************************
 * This file is part of the libgit2pp library
 * Copyright (C) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef LIBGIT2PP_LIB_CONFIG_H
#define LIBGIT2PP_LIB_CONFIG_H

#include <QtCore/QtGlobal>

/** @defgroup LibGit2pp The Qt Library for Git revision control featuring libgit2
 * Qt wrapper classes for the LibGit2 library.
 */

#ifndef LIBGIT2PP_EXPORT
#if defined(MAKE_LIBGIT2PP_LIB)
#define LIBGIT2PP_EXPORT Q_DECL_EXPORT
#else
#define LIBGIT2PP_EXPORT Q_DECL_IMPORT
#endif
#endif

#ifndef LIBGIT2PP_EXPORT_DEPRECATED
#define LIBGIT2PP_EXPORT_DEPRECATED Q_DECL_DEPRECATED LIBGIT2PP_EXPORT
#endif


#ifndef LIBGIT2PP_FUNC_NAME
#define LIBGIT2PP_FUNC_NAME __func__
#endif

#endif // LIBGIT2PP_LIB_CONFIG_H
