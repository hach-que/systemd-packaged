/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

/***
  This file is part of systemd.

  Copyright 2014 James Rhodes

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include <stdlib.h>

#include "packagefs.h"
#include "fs.h"
#include "lowlevel/util.h"

extern "C" PackageFS *packagefs_new(const char *path) {
        PackageFS *packagefs;

        packagefs = (PackageFS*)malloc(sizeof(PackageFS));
        if (!packagefs)
        {
                return (PackageFS*)0;
        }

        if (!AppLib::LowLevel::Util::createPackage(
                path,
                "",
                "",
                "",
                ""))
        {
                return (PackageFS*)0;
        }

        return packagefs_open(path);
}

extern "C" PackageFS *packagefs_open(const char *path) {
        PackageFS *packagefs;

        packagefs = (PackageFS*)malloc(sizeof(PackageFS));
        if (!packagefs)
        {
                return (PackageFS*)0;
        }

        return packagefs;
}

extern "C" void packagefs_close(PackageFS *packagefs) {
        free(packagefs);
}