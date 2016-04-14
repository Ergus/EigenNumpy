#include "cnumpy.h"

//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
//
// Simple numpy like example with pybind11. 
// This is a matrix class for c++11, This class is exposed 
// to python with pybind11 in the cnumpy.cc files.
// Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
//
//..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

cmatrix::cmatrix():
    r(0),c(0),full(0),
    data(NULL),intern(true){
    cout<<"Default Constructor"<<endl;
    };

cmatrix::cmatrix(size_t rows, size_t cols, char fill):
    r(rows),c(cols),full(rows*cols),intern(true){
    cout<<"Normal Constructor"<<endl;
    data=new double[full]();
    switch (fill){
            case '0':
                break;
            case 'r':
                for(size_t i=0;i<full;i++)
                    data[i]=frand();
                break;
            default:
                cerr<<"Wrong fill option for matrix"<<endl;
            }
    }

cmatrix::cmatrix(size_t rows, size_t cols, double* in, bool copy):
    r(rows),c(cols),full(rows*cols),
    intern(copy),data(in){
    cout<<"Buffer Constructor copy="<<(copy?"True":"False")<<endl;
    if(copy){
        data=new double[full];
        memcpy(data,in,full*sizeof(double));
        }
    }

cmatrix::cmatrix(const cmatrix &in, bool copy):
    r(in.r),c(in.c),full(in.full),intern(copy){
    cout<<"Copy Constructor ";
    cout<<"copy="<<(copy?"true":"false")<<endl;
    if (copy){
        data=new double[full];
        memcpy(data,in.data,in.full*sizeof(double));
        }
    else{
        data=in.data;
        }
    }

cmatrix::cmatrix(cmatrix &&in):
    r(in.r),c(in.c),full(in.full),data(in.data){
    cout<<"Move Constructor"<<endl;
    in.r = 0;
    in.c = 0;
    in.full = 0;
    in.data = nullptr;
    }

cvector cmatrix::operator[](int nrow){
    if (nrow<r) return cvector(c,&data[nrow*c]);
    exit(EXIT_FAILURE);
    }

cmatrix::~cmatrix(){
    cout<<"Delete"<<endl;
    if (intern && data)
        delete [] data;
    }

cmatrix cmatrix::operator*(const cmatrix &in) const{
    if(c!=in.r){
        cerr<<"Error en multiplicacion"<<endl;
        return cmatrix(0,0);
        }

    const int cols=in.c;
    cmatrix tmp(r,cols);
    
    const size_t size=r*cols;
    
    for(int i=0; i<r; i++){
        for(int j=0; j<in.c; j++){
            const double temp=data[i*c+j];
            for(int k=0;k<cols;k++){
                tmp.data[i*cols+k]+=(temp*in.data[j*cols+k]);
                }
            }
        }
    return tmp;
    }

cmatrix cmatrix::operator*(double in) const{
    cmatrix tmp(r,c);
    for(size_t i=0;i<full;i++){
        tmp.data[i]=data[i]*in;
        }
    return tmp;
    }

cmatrix operator*(double a, const cmatrix &b){
    return b*a;
    }

cmatrix &cmatrix::operator=(const cmatrix &in){
    cout<<"Assignment operator"<<endl;
    if (intern && data) delete [] data;
    r=in.r;
    c=in.c;
    full=in.full;
    intern=true;
    data=new double[full];
    memcpy(data,in.data, full*sizeof(double));
    return *this;
    }

cmatrix &cmatrix::operator=(cmatrix &&in){
    cout<<"Move assignment operator"<<endl;    
    if (&in != this) {
        if (intern && data)
            delete[] data;
        r=in.r; c=in.c; full=in.full;
        intern=in.intern; data=in.data;
        
        in.r=0; in.c=0; in.full=0;
        in.intern=true; in.data=nullptr;
        }
    return *this;    
    }

inline cvector cmatrix::begin() const{return cvector(c,data);}
inline cvector cmatrix::end() const{return cvector(c,&data[full]);}

ostream &operator<<(ostream &out,const cmatrix &in){
    for (cvector &i : in){
        cout<<i;
        }
    return out;
    }

