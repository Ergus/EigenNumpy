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

template<typename T1, typename T2>
RowMatrix mult(const T1 &a, T2 &b)
{
	return a * b;
}

PYBIND11_MODULE(EigenNumpy, m) {

	m.doc() = "EigenNumpy example plugin";

	py::class_<RowMatrix>(m, "RowMatrix", py::buffer_protocol())
	  .def(py::init<>(),"Default Constructor")
	  .def(py::init<int,int>(), "Init Constructor",
		   py::arg("int"), py::arg("int"))
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
      .def_static("zero", [](int r, int c) -> RowMatrix { return RowMatrix::Zero(r, c); })
      .def_static("random", [](int r, int c) -> RowMatrix { return RowMatrix::Random(r, c); })
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
	  .def("__mul__", &mult<const RowMatrix &, const RowMatrix &>, py::is_operator())
      .def("__mul__", &mult<const RowMatrix &, double>, py::is_operator())
      .def("__rmul__", &mult<const RowMatrix &, double>, py::is_operator())
	;
}
