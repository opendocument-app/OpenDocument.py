cmake_minimum_required(VERSION 3.18.1)
project(pyodr CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(odrcore REQUIRED)
find_package(pybind11 REQUIRED)

pybind11_add_module(pyodr pyodr.cpp)
target_link_libraries(pyodr PRIVATE odrcore::odrcore)

install(
    TARGETS pyodr
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
