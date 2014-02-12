/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

#pragma once

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

#include <stdbool.h>
#include <inttypes.h>

#include "util.h"
#include "list.h"
#include "hashmap.h"
#include "sd-event.h"
#include "sd-bus.h"

typedef struct PackageManager PackageManager;

struct PackageManager {
        sd_event *event;
        sd_bus *bus;

        Hashmap *packages;
};

PackageManager *manager_new(void);
void manager_free(PackageManager *m);

extern const sd_bus_vtable manager_vtable[];

int manager_startup(PackageManager *m);
int manager_run(PackageManager *m);