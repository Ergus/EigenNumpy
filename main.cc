//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
//
// Simple numpy like example with pybind11.
// This is a main c++11 application, there is an equivalent
// version for python
// Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
//
//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

#include "cnumpy.h"

int main()
{
	cmatrix a(5, 6, 'r'), b(6, 7, 'r');

	cout << "random a:\n" << a << endl;
	cout << "random b:\n" << b << endl;

	cmatrix c = a * b;
	// cout << "c=a*b:\n" << c << endl;

	b = a;
	cout << "b=a:\n" << b << endl;

	cmatrix d(a);
	cout << "d(a):\n" << d << endl;

	cout << "d[1][2]: " << d[1][2] << endl << endl;

	cout << "d*3.0\n" << d * 3. << endl;

	cout << "3*d\n" << 3 * d << endl;

	d[1][2] = 3.14;
	cout << "d[1][2]=3.14\n" << d << endl;
}
