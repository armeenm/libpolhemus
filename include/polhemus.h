#pragma once

typedef enum { PATRIOT, PATRIOT_HS } libpolhemus_device_type;

typedef struct {
  unsigned char* data;
  int len;
} libpolhemus_buffer;

typedef struct libpolhemus_device_handle libpolhemus_device_handle;
typedef struct libpolhemus_context libpolhemus_context;

int libpolhemus_init(libpolhemus_context**);

int libpolhemus_open(libpolhemus_context*, libpolhemus_device_type, libpolhemus_device_handle**);

int libpolhemus_get_device_type(libpolhemus_device_handle*, libpolhemus_device_type*);
int libpolhemus_get_name(libpolhemus_device_handle*, char const** name);
int libpolhemus_get_timeout(libpolhemus_device_handle*, unsigned int* timeout);

int libpolhemus_set_timeout(libpolhemus_device_handle*, unsigned int timeout);

int libpolhemus_check_connection_att(libpolhemus_device_handle*, unsigned int attempts);
int libpolhemus_check_connection(libpolhemus_device_handle*);

int libpolhemus_send_cmd(libpolhemus_device_handle*, libpolhemus_buffer const* cmd, libpolhemus_buffer* resp);

void libpolhemus_close(libpolhemus_device_handle*);

void libpolhemus_exit(libpolhemus_context*);
