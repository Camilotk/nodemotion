from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps

class NodeMotionConan(ConanFile):
    name = "nodemotion"
    version = "0.1.0"
    license = "GPL-3.0"
    url = "https://github.com/Camilotk/nodemotion/"
    description = "Node Motion — a C++23 library to visualize data structure operations as 2D animations."
    topics = ("visualization", "animation", "data-structures", "sfml", "ffmpeg", "education")
    authors = [
        "Camilo Cunha de Azevedo <cazevedo@alunos.utfpr.edu.br>",
        "Murilo Henrique <murilohenriquealves@alunos.utfpr.edu.br>",
        "Walysson Duarte <walyssonduarte@alunos.utfpr.edu.br>"
    ]

    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "include/*", "src/*", "examples/*"

    def requirements(self):
        self.requires("sfml/2.6.1")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
