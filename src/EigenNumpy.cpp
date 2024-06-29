//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
//
// Simple numpy like example with pybind11. 
// This is a file that exposes the cmatrix and cvector 
// clases to python using c++11. 
// Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
//
//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <Eigen/Dense>

#include <iostream>

namespace py = pybind11;

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMatrix;
typedef RowMatrix::Scalar Scalar;

py::ssize_t getIdx(const RowMatrix &a, int idx)
{
	return static_cast<py::ssize_t>(idx);
}

auto getIdx(const RowMatrix &a, const py::slice &slice)
{
	py::ssize_t r1, r2, stepr, rows;

	if (!slice.compute(a.rows(), &r1, &r2, &stepr, &rows))
		throw py::error_already_set();

	return Eigen::seq(r1, r2, stepr);
}

template<typename T1, typename T2, typename R>
R getItem(const RowMatrix &a, const std::pair<T1, T2> &slice)
{
	if constexpr (std::numeric_limits<T1>::is_integer and std::numeric_limits<T2>::is_integer)
		return a(slice.first, slice.second);

	return a(getIdx(a, slice.first), getIdx(a, slice.second));
}

PYBIND11_MODULE(EigenNumpy, m) {

	m.doc() = "EigenNumpy example plugin";

	pybind11::class_<RowMatrix>(m, "RowMatrix", py::buffer_protocol())
	  .def(pybind11::init<>(),"Default Constructor")
	  .def(pybind11::init<int,int>(), "Init Constructor",
		   pybind11::arg("int"), pybind11::arg("int"))
      .def(py::init([](py::buffer b)
		{
			typedef Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic> Strides;

			/* Request a buffer descriptor from Python */
			py::buffer_info info = b.request();

			/* Some basic validation checks ... */
			if (info.format != py::format_descriptor<Scalar>::format())
				throw std::runtime_error("Incompatible format: expected a double array!");

			if (info.ndim != 2)
				throw std::runtime_error("Incompatible buffer dimension!");

			auto strides = Strides(info.strides[0] / (py::ssize_t) sizeof(Scalar),
								   info.strides[1] / (py::ssize_t) sizeof(Scalar));

			auto map = Eigen::Map<RowMatrix, 0, Strides>(static_cast<Scalar *>(info.ptr),
														 info.shape[0],
														 info.shape[1],
														 strides
														 );

			return RowMatrix(map);
		}))
	  .def("rows", &RowMatrix::rows, "Return the number of rows")
	  .def("cols", &RowMatrix::cols, "Return the number of cols")
      .def("__getitem__", &getItem<int, int, double>)
	  .def("__getitem__", &getItem<py::slice, py::slice, RowMatrix>)
      .def("__getitem__", &getItem<int, py::slice, RowMatrix>)
      .def("__getitem__", &getItem<py::slice, int, RowMatrix>)
	  .def("__setitem__", [](RowMatrix &a, std::pair<int, int> idx, double v)
		{
			if (idx.first >= a.rows() || idx.second >=a.cols())
				throw py::index_error();
			a(idx.first, idx.second) = v;
		})
	  .def("__repr__", [](const RowMatrix &a)
		{
			std::stringstream out;
			out << a;
			return out.str();
		})
      .def_buffer([](RowMatrix &a) -> py::buffer_info {
	    return py::buffer_info(a.data(), sizeof(double),
	 						   py::format_descriptor<double>::format(),
	 						   2,
	 						   { a.rows(), a.cols() },
	 						   { sizeof(double) * a.cols(), sizeof(double) }
	 						   );
	    })
      .def("__mul__", [](const RowMatrix &a, const RowMatrix &b)
		{
			return RowMatrix(a * b);
		}, py::is_operator())
	  .def(py::self * py::self)
	  .def(double() * py::self)
	  .def(py::self * double())
	;
}
	// py::class_<RowMatrix>(m,"cmatrix")
	// // Constructores
	// .def(py::init<>(),"Constructor por defecto")
	// .def(py::init<int,int,char>(),
	//		"Constructor basico",
	//		py::arg("int"), py::arg("int"),
	//		py::arg("char") ='0')
	// // Constructor desde un numpy array o cualquier buffer
	// .def("__init__",			 
	//		[](cmatrix &in, py::buffer b, bool copy){
	//			py::buffer_info info = b.request();
	//			new (&in) cmatrix(info.shape[0], info.shape[1], (double*)info.ptr, copy);
	//		},
	//		"Constructor desde buffer",
	//		py::arg("buffer"),
	//		py::arg("copy")=false

	//		)
	// .def(py::init<cmatrix>(),"Constructor copia")

	// // Otras funciones
	// .def("__repr__",
	//		[](const cmatrix &a){
	//			stringstream out;
	//			out<<a;
	//			return out.str();
	//		}
	//		)
	// .def("rows", &cmatrix::rows,
	//		"Return the number of rows")
	// .def("cols", &cmatrix::cols,
	//		"Return the number of columns")

	// .def("__getitem__", [](cmatrix &a, tuple<int, int> i) {
	//	   if (get<0>(i)>= a.rows() || get<1>(i)>=a.cols())
	//		   throw py::index_error();			 
	//	   return a.get(get<0>(i),get<1>(i));
	// }
	//		)

	// .def("__getitem__", [](cmatrix &a, tuple<py::slice, py::slice> slices) -> cmatrix {
	//	   py::ssize_t startr, stopr, stepr, slicelengthr,
	//	   startc, stopc, stepc, slicelengthc;

	//	   py::slice slice=get<0>(slices);
	//	   if (!slice.compute(a.rows(), &startr, &stopr, &stepr, &slicelengthr))
	//		   throw py::error_already_set();

	//	   slice=get<1>(slices);
	//	   if (!slice.compute(a.cols(), &startc, &stopc, &stepc, &slicelengthc))
	//		   throw py::error_already_set();
			
	//	   cmatrix out(slicelengthr,slicelengthc);
			
	//	   for(size_t i=0, itr=startr;
	//			i<slicelengthr;
	//			++i, itr+=stepr) {
				
	//		   for(int j=0, itc=startc;
	//				j<slicelengthc;
	//				j++, itc+=stepc){
					
	//			   const double val=a.get(itr,itc);
	//			   out.set(i,j,val);
					
	//		   }
	//	   }
	//	   return out;
	// }
	//		)	 

	// .def("__setitem__", [](cmatrix &a, tuple<int, int> i,
	//						  double v) {
	//	   if (get<0>(i)>= a.rows() || get<1>(i)>=a.cols())
	//		   throw py::index_error(); 
	//	   a[get<0>(i)][get<1>(i)] = v;
	// })

	// .def_buffer([](cmatrix &in) -> py::buffer_info {
	//	   return py::buffer_info(
	//							  (double*)(in), sizeof(double),
	//							  py::format_descriptor<double>::value(),
	//							  2,
	//							  { in.rows(), in.cols() },
	//							  { sizeof(double)*in.cols(),
	//							  sizeof(double) }
	//							  );
	// })

	// // Iterabilidad
	
	
	// // Multiplicaciones
	// .def(py::self * py::self)
	// .def(double() * py::self)
	// .def(py::self * double())
	// ;

	// //--------------------------------------------
 
	// py::class_<cvector>(m,"cvector",py::base<cmatrix>())
	//	.def(py::init<size_t, char>(),
	//		 "Constructor normal")
	//	.def(py::init<size_t, double*>(),
	//		 "Constructor array")
	 
	//	.def("__init__",		  
	//		 [](cvector &in, py::buffer b, bool copy){
	//			 py::buffer_info info = b.request();
	//			 new (&in) cvector(info.shape[0], (double*)info.ptr, copy);
	//		 },
	//		 "Constructor desde buffer",
	//		 py::arg("buffer"),
	//		 py::arg("copy")=false
	//		 )
	 
	//	.def("__repr__",
	//		 [](const cvector &a){
	//			 stringstream out;
	//			 out<<a;
	//			 return out.str();
	//		 }
	//		 )
	//	.def("__getitem__", [](cvector &a, int i) {
	//	   if (i>=a.cols())
	//		   throw py::index_error();			 
	//	   return a[i];
	// }
	//		 )

	// .def("__getitem__", [](cmatrix &a, py::slice slice) -> cmatrix {
	//	   py::ssize_t start, stop, step, slicelength;

	//	   if (!slice.compute(a.cols(), &start, &stop, &step, &slicelength))
	//		   throw py::error_already_set();

	//	   cvector out(slicelength);
			
	//	   for(size_t i=0, it=start;
	//			i<slicelength;
	//			++i, it+=step) {
	//		   out.set(0,i,a.get(0,it));
	//	   }
	//	   return out;
	// }
	//		)

	//	.def("__setitem__", [](cvector &a, int i,
	//							double v) {
	//	   if (i>= a.cols())
	//		   throw py::index_error(); 
	//	   a[i] = v;
	// })
	//	.def_buffer([](cvector &in) -> py::buffer_info {
	//	   return py::buffer_info(
	//							  (double*)(in),
	//							  sizeof(double),
	//							  py::format_descriptor<double>::value(),
	//							  1,
	//							  { in.cols() },
	//							  { sizeof(double) }
	//							  );
	// })

	 
	//	;

