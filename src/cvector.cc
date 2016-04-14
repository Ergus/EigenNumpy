//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
//
// Simple numpy like example with pybind11. 
// This is a vector class for c++11, This class is exposed 
// to python with pybind11 in the cnumpy.cc file.
// Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
//
//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

#include "cnumpy.h"

cvector::cvector(size_t size, char fill):
    cmatrix(1,size,fill){};
        
cvector::cvector(size_t size, double* buff, bool copy):
    cmatrix(1,size,buff,copy){};

double cvector::operator[](const int col) const{
    if(col<c) return data[col];
    exit(EXIT_FAILURE);
    }
double &cvector::operator[](const int col){
    if(col<c) return data[col];
    exit(EXIT_FAILURE);
    }

bool cvector::operator==(cvector &in){
    return (data==in.data);
    }
        
bool cvector::operator!=(cvector &in){
    return (data!=in.data);
    }

ostream &operator<<(ostream &out, const cvector &in){
    out<<"[ ";
    for(size_t i=0; i<in.c; i++){
        out<<in.data[i]<<" ";
        }
    out<<"]"<<endl;
    return out;
    }


