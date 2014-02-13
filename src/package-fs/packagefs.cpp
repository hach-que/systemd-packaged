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

        // TODO: uid / gid?
        packagefs->fs = new AppLib::FS(path, 0, 0);

        return packagefs;
}

extern "C" void packagefs_getattr(
        PackageFS *packagefs,
        const char* path,
        struct stat* stbufOut)
{
        if (packagefs == (PackageFS*)0)
        {
                ((AppLib::FS*)packagefs->fs)->getattr(
                        std::string(path),
                        *stbufOut);
        }
}

extern "C" void packagefs_readdir(
        PackageFS *packagefs,
        const char* path)
{
        if (packagefs == (PackageFS*)0)
        {
                std::vector<std::string> entries = ((AppLib::FS*)packagefs->fs)->readdir(std::string(path));

                for (int i = 0; i < entries.size(); i++)
                {
                        printf("%s", entries[i].c_str());
                }
        }
}

extern "C" void packagefs_close(PackageFS *packagefs) {
        if (packagefs == (PackageFS*)0)
        {
                return;
        }

        delete (AppLib::FS*)(packagefs->fs);
        free(packagefs);
}