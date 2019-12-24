#include "core.h"

#include <cstddef>
#include <iostream>
#include <tuple>
#include <unordered_map>

#include "libpolhemus.h"

const int EP = 0;
const int TIMEOUT = 50;

using dev_handle = libpolhemus_device_handle;

const std::unordered_map<DevType, DevInfo> dev_types = {
    {PATRIOT, {PATRIOT, "Patriot", 0x0f44, 0xef12, 0x02, 0x82}},
    {PATRIOT_HS, {PATRIOT_HS, "Patriot HS", 0x0f44, 0xef20, 0x04, 0x88}},
};

int libpolhemus_init() { return libusb_init(nullptr); }

int libpolhemus_open(DevType dev_type, dev_handle** handle) {
    auto info = dev_types.at(dev_type);
    libusb_device** list;
    libusb_device* found;
    int err = 0, ret = 0;

    ssize_t cnt = libusb_get_device_list(nullptr, &list);
    if (cnt < 0) return -3;

    for (int i = 0; i < cnt; i++) {
        libusb_device* device = list[i];

        libusb_device_descriptor desc;
        err = libusb_get_device_descriptor(device, &desc);
        if (desc.idVendor == info.vid && desc.idProduct == info.pid) {
            found = device;
            break;
        }
    }

    if (found) {
        libusb_device_handle* usb_handle;
        err = libusb_open(found, &usb_handle);
        if (!err)
            *handle = new dev_handle{usb_handle, info};
        else
            ret = -2;
    } else
        ret = -1;

    libusb_free_device_list(list, 1);

    return ret;
}

int libpolhemus_read(dev_handle* handle, void* buf, int maxlen,
                     int* transferred) {
    if (!handle) return -1;

    int r = libusb_bulk_transfer(handle->libusb_handle, handle->info.read_ep,
                                 static_cast<unsigned char*>(buf), maxlen,
                                 transferred, TIMEOUT);

    return r;
}

int libpolhemus_write(dev_handle* handle, void* buf, int maxlen,
                      int* transferred) {
    if (!handle) return -1;

    int r = libusb_bulk_transfer(handle->libusb_handle, handle->info.write_ep,
                                 static_cast<unsigned char*>(buf), maxlen,
                                 transferred, TIMEOUT);

    return r;
}

void libpolhemus_close(dev_handle* handle) {
    if (!handle) return;

    if (handle->libusb_handle) libusb_close(handle->libusb_handle);
    delete handle;
}

void libpolhemus_exit() { libusb_exit(nullptr); }
