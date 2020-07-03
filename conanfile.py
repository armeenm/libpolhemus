from conans import ConanFile, CMake, tools

class Polhemus(ConanFile):
    name = 'polhemus'
    version = '1.0.0'
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'cmake_find_package_multi'
    requires = [ 'libusb/1.0.23'
               , 'fmt/6.2.1'
               ]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
