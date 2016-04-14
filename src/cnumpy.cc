#include "cnumpy.h"

//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
//
// Simple numpy like example with pybind11. 
// This is a file that exposes the cmatrix and cvector 
// clases to python using c++11. 
// Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
//
//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

PYBIND11_PLUGIN(libcnumpy){
py::module m("libcnumpy", "Mini numpy en C++");

py::class_<cmatrix>(m,"cmatrix")
    // Constructores
    .def(py::init<>(),"Constructor por defecto")
    .def(py::init<int,int,char>(),
         "Constructor basico",
         py::arg("int"), py::arg("int"),
         py::arg("char") ='0')
    // Constructor desde un numpy array o cualquier buffer
    .def("__init__",          
         [](cmatrix &in, py::buffer b, bool copy){
             py::buffer_info info = b.request();
             new (&in) cmatrix(info.shape[0], info.shape[1], (double*)info.ptr, copy);
             },
         "Constructor desde buffer",
         py::arg("buffer"),
         py::arg("copy")=false

        )
    .def(py::init<cmatrix>(),"Constructor copia")

    // Otras funciones
    .def("__repr__",
         [](const cmatrix &a){
             stringstream out;
             out<<a;
             return out.str();
             }
        )
    .def("rows", &cmatrix::rows,
         "Return the number of rows")
    .def("cols", &cmatrix::cols,
         "Return the number of columns")

    .def("__getitem__", [](cmatrix &a, tuple<int, int> i) {
            if (get<0>(i)>= a.rows() || get<1>(i)>=a.cols())
                throw py::index_error();          
            return a.get(get<0>(i),get<1>(i));
            }
        )

    .def("__getitem__", [](cmatrix &a, tuple<py::slice, py::slice> slices) -> cmatrix {
            py::ssize_t startr, stopr, stepr, slicelengthr,
                startc, stopc, stepc, slicelengthc;

            py::slice slice=get<0>(slices);
            if (!slice.compute(a.rows(), &startr, &stopr, &stepr, &slicelengthr))
                throw py::error_already_set();

            slice=get<1>(slices);
            if (!slice.compute(a.cols(), &startc, &stopc, &stepc, &slicelengthc))
                throw py::error_already_set();
            
            cmatrix out(slicelengthr,slicelengthc);
            
            for(size_t i=0, itr=startr;
                i<slicelengthr;
                ++i, itr+=stepr) {
                
                for(int j=0, itc=startc;
                    j<slicelengthc;
                    j++, itc+=stepc){
                    
                    const double val=a.get(itr,itc);
                    out.set(i,j,val);
                    
                    }
                }
            return out;
            }
        )    

    .def("__setitem__", [](cmatrix &a, tuple<int, int> i,
                           double v) {
             if (get<0>(i)>= a.rows() || get<1>(i)>=a.cols())
                 throw py::index_error(); 
             a[get<0>(i)][get<1>(i)] = v;
             })

    .def_buffer([](cmatrix &in) -> py::buffer_info {
             return py::buffer_info(
                                   (double*)(in), sizeof(double),
                                   py::format_descriptor<double>::value(),
                                   2,
                                   { in.rows(), in.cols() },
                                   { sizeof(double)*in.cols(),
                                           sizeof(double) }
                                   );
            })

    // Iterabilidad
    
    
    // Multiplicaciones
    .def(py::self * py::self)
    .def(double() * py::self)
    .def(py::self * double())
    ;

//--------------------------------------------
 
 py::class_<cvector>(m,"cvector",py::base<cmatrix>())
     .def(py::init<size_t, char>(),
         "Constructor normal")
     .def(py::init<size_t, double*>(),
          "Constructor array")
     
     .def("__init__",          
         [](cvector &in, py::buffer b, bool copy){
             py::buffer_info info = b.request();
             new (&in) cvector(info.shape[0], (double*)info.ptr, copy);
             },
         "Constructor desde buffer",
         py::arg("buffer"),
         py::arg("copy")=false
         )
     
     .def("__repr__",
          [](const cvector &a){
              stringstream out;
              out<<a;
              return out.str();
              }
          )
     .def("__getitem__", [](cvector &a, int i) {
             if (i>=a.cols())
                 throw py::index_error();          
             return a[i];
             }
         )

    .def("__getitem__", [](cmatrix &a, py::slice slice) -> cmatrix {
            py::ssize_t start, stop, step, slicelength;

            if (!slice.compute(a.cols(), &start, &stop, &step, &slicelength))
                throw py::error_already_set();

            cvector out(slicelength);
            
            for(size_t i=0, it=start;
                i<slicelength;
                ++i, it+=step) {
                out.set(0,i,a.get(0,it));
                }
            return out;
            }
        )

     .def("__setitem__", [](cvector &a, int i,
                           double v) {
              if (i>= a.cols())
                  throw py::index_error(); 
              a[i] = v;
              })
     .def_buffer([](cvector &in) -> py::buffer_info {
             return py::buffer_info(
                                    (double*)(in),
                                    sizeof(double),
                                    py::format_descriptor<double>::value(),
                                    1,
                                    { in.cols() },
                                    { sizeof(double) }
                                    );
             })

     
     ;

return m.ptr();
}

