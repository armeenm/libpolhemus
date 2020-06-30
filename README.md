# libpolhemus

A library written in C++ that provides an interface to Polhemus's sensors.

Exposes both C++ and C interfaces.

Currently only supports the Patriot over USB. May work with others over USB.

Build requirements: C++17, Python 3

Dependencies are managed via Conan.

Building:

```
conan install -if build --build=missing .
cmake -S. -Bbuild
cmake --build build
cmake --install build
```

For Windows, use `cmake --build build --config Release`
