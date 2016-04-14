//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
//
// Simple numpy like example with pybind11. 
// This is a c++11 header for all the library with a cvector 
// and a cmatrix class defines in c++ and exposed to Python.
// Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
//
//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

#ifndef CNUMPY_H
#define CNUMPY_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include "pybind11.h"
#include "operators.h"
#include <sstream>

#include "cmatrix.h"
#include "cvector.h"

namespace py = pybind11;

#endif
