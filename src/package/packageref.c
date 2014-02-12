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

#include "sd-daemon.h"

#include "packageref.h"

PackageRef *packageref_new(void) {
        PackageRef *m;

        m = new0(PackageRef, 1);
        if (!m)
                return NULL;

        m->name = NULL;

        if (!m->name) {
                packageref_free(m);
                return NULL;
        }

        return m;
}

void packageref_free(PackageRef *m) {
        if (m->name != NULL)
        {
                free(m->name);
        }

        free(m);
}