import os
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name: str, sourcedir: str = "") -> None:
        super().__init__(name, sources=[])
        self.sourcedir = os.fspath(Path(sourcedir).resolve())


class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        ext_fullpath = Path.cwd() / self.get_ext_fullpath(ext.name)
        extdir = ext_fullpath.parent.resolve()

        build_temp = Path(self.build_temp) / ext.name
        build_temp.mkdir(parents=True, exist_ok=True)

        conan_args = [
            f"--output-folder={build_temp}",
            "--build=missing",
            "-s", "build_type=Release",
        ]

        subprocess.run(
            ["conan", "install", ext.sourcedir, *conan_args], check=True
        )

        cmake_args = [
            f"-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}{os.sep}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE=Release",
        ]

        build_args = []

        subprocess.run(
            ["cmake", "-S", ext.sourcedir, "-B", build_temp, *cmake_args], check=True
        )
        subprocess.run(
            ["cmake", "--build", build_temp, *build_args], check=True
        )


setup(
    name="pyodr",
    version="0.0.1",
    author="Andreas Stefl",
    author_email="stefl.andreas@gmail.com",
    description="It's Android's first OpenOffice Document Reader for Python!",
    long_description="",
    ext_modules=[CMakeExtension("pyodr", "src/cpp")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    install_requires=[],
    extras_require={
        "dev": ["black"],
        "test": ["pytest>=6.0"],
    },
    python_requires=">=3.7",
)
