#pragma once

typedef enum { PATRIOT, PATRIOT_HS } libpolhemus_device_type;

typedef struct {
  unsigned char* data;
  int len;
} libpolhemus_buffer;

#ifdef __GNUC__
#define PLHM_DEV_HANDLE_ libpolhemus_device_handle*
#define PLHM_NONNULL_(x) __attribute__((nonull(x)))
#else
#define PLHM_DEV_HANDLE_ libpolhemus_device_handle dh[static 1]
#define PLHM_NONNULL_(x)
#endif

typedef struct libpolhemus_device_handle libpolhemus_device_handle;
typedef struct libpolhemus_context libpolhemus_context;

int libpolhemus_init(libpolhemus_context**);

int libpolhemus_open(libpolhemus_context*, libpolhemus_device_type, libpolhemus_device_handle**) PLHM_NONNULL_(3);

int libpolhemus_get_device_type(libpolhemus_device_handle*, libpolhemus_device_type*) PLHM_NONNULL_(1);
int libpolhemus_get_name(libpolhemus_device_handle*, char const** name) PLHM_NONNULL_(1);
int libpolhemus_get_timeout(libpolhemus_device_handle*, unsigned int* timeout) PLHM_NONNULL_(1);

int libpolhemus_set_timeout(libpolhemus_device_handle*, unsigned int timeout) PLHM_NONNULL_(1);

int libpolhemus_check_connection_att(libpolhemus_device_handle*, unsigned int attempts) PLHM_NONNULL_(1);
int libpolhemus_check_connection(libpolhemus_device_handle*) PLHM_NONNULL_(1);

int libpolhemus_send_cmd(libpolhemus_device_handle*, libpolhemus_buffer const* cmd, libpolhemus_buffer* resp)
    PLHM_NONNULL_(1);

void libpolhemus_close(libpolhemus_device_handle*);

void libpolhemus_exit(libpolhemus_context*);
