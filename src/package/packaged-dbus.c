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

#include <stdio.h>

#include "util.h"
#include "sd-event.h"
#include "sd-bus.h"
#include "bus-errors.h"

#include "packagemanager.h"
#include "packageref.h"

static int method_get_package(sd_bus *bus, sd_bus_message *message, void *userdata, sd_bus_error *error) {
        _cleanup_free_ char *p = NULL;
        PackageManager *m = userdata;
        PackageRef *package;
        const char *name;
        int r;

        printf("got message\n");

        assert(bus);
        assert(message);
        assert(m);

        r = sd_bus_message_read(message, "s", &name);
        if (r < 0)
                return r;

        package = hashmap_get(m->packages, name);
        if (!package)
                return sd_bus_error_setf(error, BUS_ERROR_NO_SUCH_MACHINE, "No package '%s' known", name);

        // TODO: Something useful with that.

        /*
        p = machine_bus_path(machine);
        if (!p)
                return -ENOMEM;
        */

        return sd_bus_reply_method_return(message, "s", name);
}

const sd_bus_vtable manager_vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("GetPackage", "s", "s", method_get_package, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};