#include "cnumpy.h"

cmatrix::cmatrix():
    r(0),c(0),full(0),data(NULL){};

cmatrix::cmatrix(int rows, int cols, char fill):
    r(rows),c(cols),full(rows*cols){
    data=new double[full]();
    switch (fill){
            case '0':
                break;
            case 'r':
                for(int i=0;i<full;i++)
                    data[i]=frand();
                break;
            default:
                cerr<<"Wrong fill option for matrix"<<endl;
            }
    }

cmatrix::cmatrix(const cmatrix &in):
    r(in.r),c(in.c),full(in.full){
    data=new double[full];
    memcpy(data,in.data,in.full*sizeof(double));
    }

double* cmatrix::operator[](int row){
    if (row<r) return &data[row*c];
    return NULL;
    }

cmatrix::~cmatrix(){
    delete [] data;
    }

cmatrix cmatrix::operator*(const cmatrix &in){
    if(c!=in.r){
        cerr<<"Error en multiplicacion"<<endl;
        return cmatrix(0,0);
        }

    const int cols=in.c;
    cmatrix *tmp=new cmatrix(r,cols);
    
    const int size=r*cols;
    
    for(int i=0; i<r; i++){
        for(int j=0; j<in.c; j++){
            const double temp=data[i*c+j];
            for(int k=0;k<cols;k++){
                tmp->data[i*cols+k]+=(temp*in.data[j*cols+k]);
                }
            }
        }
    return *tmp;
    }

cmatrix cmatrix::operator*(double in){
    cmatrix *tmp=new cmatrix(r,c);
    for(int i=0;i<full;i++){
        tmp->data[i]=data[i]*in;
        }
    return *this;
    }

cmatrix &cmatrix::operator=(const cmatrix &in){
    if (data) delete [] data;
    r=in.r;
    c=in.c;
    full=in.full;
    data=new double[full];
    memcpy(data,in.data,in.full*sizeof(double));
    return *this;
    }


ostream &operator<<(ostream &out,const cmatrix &in){
    for(int i=0;i<in.r;i++){
        const int idim=i*in.c;
        out<<"|  ";
        for(int j=0;j<in.c;j++){
            out<<setprecision(4)<<in.data[idim+j]<<"  ";
            }
        out<<"|"<<endl;
        }
    return out;
    }
