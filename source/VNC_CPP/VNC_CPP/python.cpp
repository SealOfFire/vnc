/**
 * 导出给python
 */
#include <boost/python.hpp>
//#include <boost/python/module.hpp>
#include "VNCService.h"
using namespace boost::python;

BOOST_PYTHON_MODULE (vnc_service) {
    class_<VNCService>("VNCService")
            .def("start", &VNCService::start)
            .def("startRepeater", &VNCService::startRepeater);
}