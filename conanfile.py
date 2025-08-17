from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class my_neural_network_libRecipe(ConanFile):
    name = "my-neural-network-lib"
    version = "0.0"
    package_type = "library"

    # Optional metadata
    author = "Zarwaldo martin.thioliere@gmail.com"
    url = "https://github.com/Zarwaldo/my-neural-network-lib"
    description = "A C++ library for building and training neural networks with hardware acceleration."
    topics = ("neural-networks", "machine-learning", "hardware-acceleration")
    build_requires = "cmake/[>=3.15]"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["my-neural-network-lib"]

