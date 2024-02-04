#include <dy/kdycode.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//! functions that must be provided by OS implementations 

typedef u0 *os_handle_t;

// find library by name on disk, return 0 on error
extern os_handle_t kDyPlatLoadLib(c_str lib_name);

// unload a previously loaded library
extern u0 kDyPlatUnloadLib(os_handle_t);

// locate a function fun_name in the os handle, return 0 on error
extern kDyfun kDyPlatGetDyFun(os_handle_t, c_str fun_name);

// the extension for dynamic libraries (.dll, .so, etc)
extern c_str kdyplat_libext;

//! common generic functionality

u0 kDyBindLib(kDylib *self, c_str dylib_name) {
    self->name = dylib_name;
}

b8 kDyBindFun(kDylib *self, c_str dyfun_name, u0 (**dyfun)(u0)) {
    if (!self) return kfalse;
    if (!dyfun_name) return kfalse;
    if (!dyfun) return kfalse;

    if (self->num_bindings == KMAX_DYFUNCS) return kfalse;

    self->bindings[self->num_bindings].dyfun = dyfun;
    self->bindings[self->num_bindings].dyfun_name = dyfun_name;
    self->num_bindings += 1;

    return ktrue;
}

static u0 try_callback(os_handle_t os_handle, c_str cb_name) {
    kDyfun cb = kDyPlatGetDyFun(os_handle, cb_name);
    if (cb) {
        cb();
    } else {
        printf("callback %s not found\n", cb_name);
    }
}

static b8 timestamp_get(c_str timestamp, time_t *t) {
    FILE *f = fopen(timestamp, "rb");
    if (f) {
        time_t now;
        fread(&now, sizeof(now), 1, f);
        fclose(f);
        if (t) {
            *t = now;
        }
        return ktrue;
    }
    return kfalse;
}


b8 kDyRequestReload(kDylib *self) {
    if (!self) { return kfalse; }

    c_str stamp_file = "timestamp.bin";
    time_t stamp;
    if (!timestamp_get(stamp_file, &stamp)) {
        printf("Hot code reload: No timestamp file...\n");
        return kfalse;
    }

    if (self->os_handle != NULL) {
        // check if the timestamp is newer than
        // the last load time
        // if it is, we must reload
        if (difftime(stamp, self->stamp) > 0) {
            kDyCleanup(self);
        } else {
            return ktrue;
        }
    }

    printf("Reload request fulfilled\n");

    self->stamp = stamp;
    self->os_handle = kDyPlatLoadLib(self->name);
    if (self->os_handle == NULL) { return kfalse; }

    try_callback(self->os_handle, "dylib_setup");

    for (int i = 0; i < self->num_bindings; ++i) {
        c_str dyfun_name = self->bindings[i].dyfun_name;
        kDyfun *dyfun = self->bindings[i].dyfun;

        kDyfun f = kDyPlatGetDyFun(self->os_handle, dyfun_name);
        if (!f) { return kfalse; }

        *dyfun = f;
    }


    return ktrue;
}

u0 kDyCleanup(kDylib *self) {
    if (!self) return;

    if (self->os_handle != NULL) {
        try_callback(self->os_handle, "dylib_cleanup");
        kDyPlatUnloadLib(self->os_handle);
        self->os_handle = NULL;
    }
}
