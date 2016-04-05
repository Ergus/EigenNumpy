#ifndef CNUMPY_H
#define CNUMPY_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include "pybind11.h"
#include <sstream>

using namespace std;

#define frand()((double)rand()/(RAND_MAX))

class cmatrix{
    public:
        cmatrix();
        cmatrix(int rows, int cols, char fill='0');
        cmatrix(const cmatrix&);
        ~cmatrix();
        
        int rows(){return r;}
        int cols(){return c;}
        
        double* operator[](int row);
        cmatrix operator*(const cmatrix &in);
        cmatrix operator*(double in);
        cmatrix &operator=(const cmatrix &in);
        
    private:
        double *data;
        int r,c,full;
        friend ostream &operator<<(ostream &out,const cmatrix &in);
    };

ostream &operator<<(ostream &out,const cmatrix &in);


#endif
