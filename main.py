#!/usr/bin/env python

import libcnumpy as my

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
