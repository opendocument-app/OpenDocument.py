import os
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name: str, source_dir: Path) -> None:
        super().__init__(name, sources=[])
        self.source_dir = source_dir.resolve()


class CMakeBuild(build_ext):
    def conan_profile(self) -> str:
        subprocess.run(["conan", "profile", "detect", "-e"], check=True)

        default_path = Path.home() / ".conan2" / "profiles" / "default"

        with open(default_path, "r") as f:
            default_profile = f.read()
        default_profile = default_profile.replace("compiler.cppstd=gnu14", "compiler.cppstd=gnu20")
        with open(default_path, "w") as f:
            f.write(default_profile)

    def conan_remote(self) -> str:
        conan_odr_remote = (
            "https://artifactory.opendocument.app/artifactory/api/conan/conan"
        )

        result = subprocess.run(
            ["conan", "remote", "list"], check=True, capture_output=True, text=True
        )
        if conan_odr_remote not in result.stdout:
            print(f"Adding Conan remote {conan_odr_remote}")
            subprocess.run(
                [
                    "conan",
                    "remote",
                    "add",
                    "odr",
                    conan_odr_remote,
                ],
                check=True,
            )

    def conan_install(self, source_dir: Path, build_dir: Path) -> None:
        subprocess.run(
            [
                "conan",
                "install",
                str(source_dir),
                f"--output-folder={str(build_dir)}",
                "--build=missing",
                "-s",
                "build_type=Release",
            ],
            check=True,
        )

    def cmake_configure(self, source_dir: Path, build_dir: Path, ext_dir: Path) -> None:
        subprocess.run(
            [
                "cmake",
                "-S",
                str(source_dir),
                "-B",
                str(build_dir),
                f"-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake",
                f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={str(ext_dir)}{os.sep}",
                f"-DPYTHON_EXECUTABLE={sys.executable}",
                f"-DCMAKE_BUILD_TYPE=Release",
            ],
            check=True,
        )

    def cmake_build(self, build_dir: Path) -> None:
        subprocess.run(
            [
                "cmake",
                "--build",
                str(build_dir),
                "--config",
                "Release",
            ],
            check=True,
        )

    def cmake_install(self, build_dir: Path) -> None:
        subprocess.run(
            [
                "cmake",
                "--install",
                str(build_dir),
                "--config",
                "Release",
            ],
            check=True,
        )

    def build_extension(self, ext: CMakeExtension) -> None:
        source_dir = Path(ext.source_dir)

        ext_fullpath = Path.cwd() / self.get_ext_fullpath(ext.name)
        ext_dir = ext_fullpath.parent.resolve()

        build_temp = Path(self.build_temp) / ext.name
        build_temp.mkdir(parents=True, exist_ok=True)

        self.conan_profile()
        self.conan_remote()
        self.conan_install(source_dir, build_temp)

        self.cmake_configure(source_dir, build_temp, ext_dir)
        self.cmake_build(build_temp)
        self.cmake_install(build_temp)


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
