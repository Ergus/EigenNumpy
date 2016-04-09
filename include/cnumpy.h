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

using namespace std;

#define frand()((double)rand()/(RAND_MAX))

class cmatrix{
    public:
        cmatrix();
        cmatrix(size_t rows, size_t cols,
                char fill='0');
        
        cmatrix(const cmatrix &);
        cmatrix(cmatrix &&);
        
        ~cmatrix();
        
        size_t rows() const{return r;}
        size_t cols() const{return c;}

        cmatrix operator*(const cmatrix &in) const;
        cmatrix operator*(double in) const;
        friend cmatrix operator*(double a, const cmatrix &b);

        double* operator[](int row);
        
        cmatrix &operator=(const cmatrix &);
        cmatrix &operator=(cmatrix &&);

        friend ostream &operator<<(ostream &out,const cmatrix &in);
        
    private:
        double *data;
        size_t r,c,full;
    };


#endif
