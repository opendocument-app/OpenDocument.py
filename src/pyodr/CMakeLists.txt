pybind11_add_module(core core.cpp)
target_link_libraries(core PRIVATE odrcore::odrcore)

install(
    TARGETS core
    LIBRARY DESTINATION src/pyodr
)
