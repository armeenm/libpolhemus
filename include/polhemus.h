#pragma once

#include <stdbool.h>

#ifdef __GNUC__
#define PLHM_NONNULL_(x) __attribute__((nonnull(x)))
#else
#define PLHM_NONNULL_(x)
#endif

typedef enum { PATRIOT, PATRIOT_HS } libpolhemus_device_type;

typedef struct libpolhemus_device_handle libpolhemus_device_handle;
typedef struct libpolhemus_context libpolhemus_context;

int libpolhemus_init(libpolhemus_context**);

int libpolhemus_open(libpolhemus_context*, libpolhemus_device_type, libpolhemus_device_handle**)
    PLHM_NONNULL_(3);

libpolhemus_device_type libpolhemus_get_device_type(libpolhemus_device_handle*) PLHM_NONNULL_(1);
char const* libpolhemus_get_name(libpolhemus_device_handle*) PLHM_NONNULL_(1);
unsigned int libpolhemus_get_timeout(libpolhemus_device_handle*) PLHM_NONNULL_(1);

void libpolhemus_set_timeout(libpolhemus_device_handle*, unsigned int timeout) PLHM_NONNULL_(1);

bool libpolhemus_check_connection(libpolhemus_device_handle*, int attempts) PLHM_NONNULL_(1);

int libpolhemus_send_cmd(libpolhemus_device_handle*, char const* cmd, char* resp, int max_resp_size)
    PLHM_NONNULL_(1);

void libpolhemus_close(libpolhemus_device_handle*);

void libpolhemus_exit(libpolhemus_context*);
