#ifndef CMATRIX_H
#define CMATRIX_H

#include "cnumpy.h"

using namespace std;

#define frand()((double)rand()/(RAND_MAX))

class cvector;   // Declaracion previa de las filas

class cmatrix{
    public:
        cmatrix();
        cmatrix(size_t, size_t, char fill='0');
        cmatrix(size_t, size_t, double*, bool copy=false);
        cmatrix(const cmatrix &, bool copy=true);
        cmatrix(cmatrix &&);
        
        ~cmatrix();
        
        size_t rows() const{return r;}
        size_t cols() const{return c;}

        cmatrix operator*(const cmatrix &in) const;
        cmatrix operator*(double in) const;
        friend cmatrix operator*(double a, const cmatrix &b);

        cvector operator[](int row);
        
        cmatrix &operator=(const cmatrix &);
        cmatrix &operator=(cmatrix &&);

        cvector begin() const;
        cvector end() const;

        friend ostream &operator<<(ostream &out,const cmatrix &in);
        
        cmatrix &operator*(){return *this;};
        operator double*(){return data;};

        void set(size_t ro,size_t co, double val){
            data[ro*c+co]=val;}
        
        double get(size_t ro,size_t co) const{
            return data[ro*c+co];}
        
    protected:
        double *data;
        size_t r,c,full;
        bool intern;
    };

#endif


