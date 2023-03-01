/**
 * 导出给python
 */
#ifdef WIN
    #define BOOST_PYTHON_STATIC_LIB
#endif

#include <boost/python.hpp>
//#include <boost/python/module.hpp>
#include "VNCService.h"
using namespace boost::python;

BOOST_PYTHON_MODULE (VNC_CPP) {
    class_<VNCService>("VNCService")
            .def("start", &VNCService::start)
            .def("startRepeater", &VNCService::startRepeater);
}