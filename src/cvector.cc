#include "cnumpy.h"

cvector::cvector(size_t size, char fill):
    cmatrix(1,size,fill){};
        
cvector::cvector(size_t size, double* buff):
    cmatrix(1,size,buff){};

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
