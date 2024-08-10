from skbuild_conan import setup
from setuptools import find_packages


setup(
    name="pyodr",
    version="0.0.1",
    author="Andreas Stefl",
    author_email="stefl.andreas@gmail.com",
    description="It's Android's first OpenOffice Document Reader for Python!",
    long_description="",
    python_requires=">=3.7",
    cmake_minimum_required_version="3.12",
    packages=find_packages("src"),
    package_dir={"": "src"},
    install_requires=[],
    extras_require={
        "dev": ["black"],
        "test": ["pytest>=6.0"],
    },
    conanfile="src/cpp/conanfile.txt",
)
