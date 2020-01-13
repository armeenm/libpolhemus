#pragma once

typedef enum { PATRIOT, PATRIOT_HS } libpolhemus_device_type;

typedef struct {
    unsigned char* data;
    int len;
} libpolhemus_buffer;

typedef struct libpolhemus_device_handle libpolhemus_device_handle;

int libpolhemus_init();

int libpolhemus_open(libpolhemus_device_type dev_type,
                     libpolhemus_device_handle* handle);

unsigned int libpolhemus_get_timeout(libpolhemus_device_handle* handle);

void libpolhemus_set_timeout(libpolhemus_device_handle* handle,
                             unsigned int timeout);

int libpolhemus_send_raw(libpolhemus_device_handle* handle,
                         const libpolhemus_buffer* buf);
int libpolhemus_recv_raw(libpolhemus_device_handle* handle,
                         libpolhemus_buffer* buf);

int libpolhemus_check_connection_att(libpolhemus_device_handle* handle,
                                     unsigned int attempts);
int libpolhemus_check_connection(libpolhemus_device_handle* handle);

int libpolhemus_send_cmd(libpolhemus_device_handle* handle,
                         const libpolhemus_buffer* cmd,
                         libpolhemus_buffer* resp);

void libpolhemus_close(libpolhemus_device_handle* handle);

void libpolhemus_exit();
