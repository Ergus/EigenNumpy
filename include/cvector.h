#ifndef CVECTOR_H
#define CVECTOR_H

#include "cnumpy.h"

class cmatrix;

class cvector: public cmatrix{
    public:
        cvector(size_t size, char fill='0');
        
        cvector(size_t size, double* buff);

        double operator[](const int col) const;
        double &operator[](const int col);

        cvector &operator*(){return *this;}
        
        bool operator==(cvector &in);
        bool operator!=(cvector &in);

        inline cvector &operator++(){
            data+=c;
            return *this;
            }

        friend ostream &operator<<(ostream &out, const cvector &in);

    };

#endif
