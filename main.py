#!/usr/bin/env python

#..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
#
# Simple numpy like example with pybind11. 
# This is a python main application, there is an equivalent 
# version for c++ 11
# Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
#
#..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

import libcnumpy as my
import numpy as np

a,b = my.cmatrix(5,6,"r"), my.cmatrix(6,7,"r"),

print("random a:\n",a)
print("random b:\n",b)

c=a*b
print("c=a*b:\n",c)

b=a
print("b=a:\n",b)

d=my.cmatrix(a)
print("d=cmatrix(a):\n",d)

print("d[1,2]: ",d[1,2])

print("d*3.0\n",d*3.0)

print("3*d\n",3*d)

print("=== Test Numpy ===\n")

npa=np.random.sample((3,4))

print("Numpy array npa=np.random.sample((3,4)) (random numpy array)\n",npa)

e=my.cmatrix(npa)

print("\ne=my.cmatrix(npa) (copy from numpy array copy=False)\n",e)

f=np.array(d)

print("f=np.array(d) (Copy to numpy)\n",f)


print("d[1:-1:2,1:-1:2]\n",d[1:-1:2,1:-1:2])


# Test cvector

a=my.cvector(8,"r")
print("a=my.cvector(8,\"r\")\n",a)

b=my.cvector(a)
print("b=my.cvector(a)\n",b)

b=my.cvector(a,True)
print("b=my.cvector(a,True)\n",b)

print("d[1:2,:]\n",d[1:2,:])

h=my.cmatrix(d[1:2,:])
print("h=cmatrix(d[1:2,:])\n",h)

