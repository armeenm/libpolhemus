#ifndef LIBPOLHEMUS_H
#define LIBPOLHEMUS_H

/* Structures */

typedef struct libpolhemus_device_handle libpolhemus_device_handle;

typedef enum libpolhemus_devtype { PATRIOT, PATRIOT_HS } libpolhemus_devtype;

typedef struct libpolhemus_buffer {
    unsigned char* data;
    int len;
} libpolhemus_buffer;

/* Functions */

int libpolhemus_init();

int libpolhemus_open(libpolhemus_devtype dev_type,
                     libpolhemus_device_handle* handle);

unsigned int libpolhemus_get_timeout(
    const libpolhemus_device_handle* const handle);
void libpolhemus_set_timeout(libpolhemus_device_handle* const handle,
                             unsigned int timeout);

int libpolhemus_send_raw(libpolhemus_device_handle* handle,
                         libpolhemus_buffer buf);
int libpolhemus_recv_raw(libpolhemus_device_handle* handle,
                         libpolhemus_buffer buf);

int libpolhemus_check_connection_att(libpolhemus_device_handle* handle,
                                     unsigned int attempts);
int libpolhemus_check_connection(libpolhemus_device_handle* handle);

int libpolhemus_send_cmd(libpolhemus_device_handle* handle,
                         libpolhemus_buffer cmd, libpolhemus_buffer resp);

void libpolhemus_close(libpolhemus_device_handle* handle);

void libpolhemus_exit();

#endif /* LIBPOLHEMUS_H */
