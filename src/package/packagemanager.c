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

#include "def.h"
#include "bus-util.h"

#include "packagemanager.h"
#include "packageref.h"

PackageManager *manager_new(void) {
        PackageManager *m;
        int r;

        m = new0(PackageManager, 1);
        if (!m)
                return NULL;

        m->packages = hashmap_new(string_hash_func, string_compare_func);

        if (!m->packages) {
                manager_free(m);
                return NULL;
        }

        r = sd_event_default(&m->event);
        if (r < 0) {
                manager_free(m);
                return NULL;
        }

        sd_event_set_watchdog(m->event, true);

        return m;
}

void manager_free(PackageManager *m) {
        PackageRef *entry;

        assert(m);

        while ((entry = hashmap_first(m->packages)))
                packageref_free(entry);

        hashmap_free(m->packages);

        sd_bus_unref(m->bus);
        sd_event_unref(m->event);

        free(m);
}

static int manager_connect_bus(PackageManager *m) {
        _cleanup_bus_error_free_ sd_bus_error error = SD_BUS_ERROR_NULL;
        int r;

        assert(m);
        assert(!m->bus);

        r = sd_bus_default_system(&m->bus);
        if (r < 0) {
                log_error("Failed to connect to system bus: %s", strerror(-r));
                return r;
        }

        r = sd_bus_add_object_vtable(m->bus, "/org/freedesktop/package1", "org.freedesktop.package1.Manager", manager_vtable, m);
        if (r < 0) {
                log_error("Failed to add manager object vtable: %s", strerror(-r));
                return r;
        }

        r = sd_bus_request_name(m->bus, "org.freedesktop.package1", 0);
        if (r < 0) {
                log_error("Failed to register name: %s", strerror(-r));
                return r;
        }

        r = sd_bus_attach_event(m->bus, m->event, 0);
        if (r < 0) {
                log_error("Failed to attach bus to event loop: %s", strerror(-r));
                return r;
        }

        return 0;
}

int manager_startup(PackageManager *m) {
        int r;

        /* Connect to the bus */
        r = manager_connect_bus(m);
        if (r < 0)
                return r;

        /* TODO: Load packages */

        return 0;
}

static bool check_idle(void* userdata) {
        /* TODO: GC */

        return false;
}

int manager_run(PackageManager *m) {
        assert(m);

        return bus_event_loop_with_idle(
                m->event,
                m->bus,
                "org.freedesktop.package1",
                DEFAULT_EXIT_USEC,
                check_idle, m);
}