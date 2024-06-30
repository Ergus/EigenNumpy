#!/usr/bin/env python

#..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
#
# Simple numpy like example with pybind11. 
# This is a python main application, there is an equivalent 
# version for c++ 11
# Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
#
#..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

import EigenNumpy as enp
import numpy as np

a,b = enp.RowMatrix.random(5,6), enp.RowMatrix.random(6,7),

print("random a:\n",a)
print("random b:\n",b)

c = a * b
print("c=a*b:\n",c)

b = a
print("b=a:\n",b)

d = enp.RowMatrix(a)
print("d=RowMatrix(a):\n",d)

print("d[1,2]: ", d[1,2])

print("d*3.0\n",d*3.0)

print("3*d\n",3.0 * d)

print("=== Test Numpy ===\n")

npa = np.random.sample((3,4))

print("Numpy array npa=np.random.sample((3,4)) (random numpy array)\n",npa)

e = enp.RowMatrix(npa)

print("\ne=my.cmatrix(npa) (copy from numpy array copy=False)\n",e)

f=np.array(d)

print("f=np.array(d) (Copy to numpy)\n",f)


print("d[1:-1:2,1:-1:2]\n", d[1:-1:2, 1:-1:2])
