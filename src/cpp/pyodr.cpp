#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(pyodr, m) {
    m.attr("__version__") = "dev";
}
