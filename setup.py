import os
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name: str, sourcedir: Path) -> None:
        super().__init__(name, sources=[])
        self.sourcedir = sourcedir.resolve()


class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        ext_fullpath = Path.cwd() / self.get_ext_fullpath(ext.name)
        extdir = ext_fullpath.parent.resolve()

        build_temp = Path(self.build_temp) / ext.name
        build_temp.mkdir(parents=True, exist_ok=True)

        subprocess.run(["conan", "profile", "detect", "-e"], check=True)

        conan_odr_remote = "https://artifactory.opendocument.app/artifactory/api/conan/conan"

        result = subprocess.run(["conan", "remote", "list"], check=True, capture_output=True, text=True)
        if conan_odr_remote not in result.stdout:
            print(f"Adding Conan remote {conan_odr_remote}")
            subprocess.run([
                "conan",
                "remote",
                "add",
                "odr",
                conan_odr_remote,
            ], check=True)

        conan_args = [
            f"--output-folder={str(build_temp)}",
            "--build=missing",
            "-s",
            "build_type=Release",
        ]

        subprocess.run(["conan", "install", str(ext.sourcedir), *conan_args], check=True)

        cmake_args = [
            f"-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={str(extdir)}{os.sep}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE=Release",
        ]

        build_args = [
            "--config",
            "Release",
        ]

        subprocess.run(
            ["cmake", "-S", str(ext.sourcedir), "-B", str(build_temp), *cmake_args], check=True
        )
        subprocess.run(["cmake", "--build", str(build_temp), *build_args], check=True)


setup(
    name="pyodr",
    version="0.0.1",
    author="Andreas Stefl",
    author_email="stefl.andreas@gmail.com",
    description="It's Android's first OpenOffice Document Reader for Python!",
    long_description="",
    ext_modules=[CMakeExtension("pyodr", Path("src/cpp"))],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    install_requires=[],
    extras_require={
        "dev": ["black"],
        "test": ["pytest>=6.0"],
    },
    python_requires=">=3.7",
)
