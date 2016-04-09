#include "cnumpy.h"

cmatrix::cmatrix():
    r(0),c(0),full(0),data(NULL){
    cout<<"Default Constructor"<<endl;
    };

cmatrix::cmatrix(size_t rows, size_t cols, char fill):
    r(rows),c(cols),full(rows*cols){
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

cmatrix::cmatrix(const cmatrix &in):
    r(in.r),c(in.c),full(in.full){
    cout<<"Copy Constructor"<<endl;
    data=new double[full];
    memcpy(data,in.data,in.full*sizeof(double));
    }

cmatrix::cmatrix(cmatrix &&in):
    r(in.r),c(in.c),full(in.full),data(in.data){
    cout<<"Move Constructor"<<endl;
    in.r = 0;
    in.c = 0;
    in.full = 0;
    in.data = nullptr;
    }

double* cmatrix::operator[](int row){
    if (row<r) return &data[row*c];
    exit(EXIT_FAILURE);
    }

cmatrix::~cmatrix(){
    cout<<"Delete"<<endl;
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
    if (data) delete [] data;
    r=in.r;
    c=in.c;
    full=in.full;
    data=new double[full];
    memcpy(data,in.data, full*sizeof(double));
    return *this;
    }

cmatrix &cmatrix::operator=(cmatrix &&in){
    cout<<"Move assignment operator"<<endl;    
    if (&in != this) {
        if (data) delete[] data;
        r=in.r; c=in.c; full=in.full; data=in.data;
        in.r=0; in.c=0; in.full=0; in.data=nullptr;
        }
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


namespace py = pybind11;

PYBIND11_PLUGIN(libcnumpy){

py::module m("libcnumpy", "Mini numpy en C++");

py::class_<cmatrix>(m,"cmatrix")
    .def(py::init<>(),"Constructor por defecto")
    .def(py::init<int,int,char>(),
        "Constructor basico",
        py::arg("int"), py::arg("int"),
        py::arg("char") ='0')
    .def(py::init<cmatrix>(),"Constructor copia")
    .def("__repr__",
         [](const cmatrix &a){
             stringstream out;
             out<<a;
             return out.str();
             }
        )
    .def("rows", &cmatrix::rows,
         "Return the number of rows")
    .def("cols", &cmatrix::cols,
         "Return the number of columns")

    .def("__getitem__", [](cmatrix &a, tuple<int, int> i) {
            if (get<0>(i)>= a.rows() || get<1>(i)>=a.cols())
                throw py::index_error();          
            return a[get<0>(i)][get<1>(i)];
            }
        )
    
    .def("__setitem__", [](cmatrix &a, tuple<int, int> i,
                           double v) {
             if (get<0>(i)>= a.rows() || get<1>(i)>=a.cols())
                 throw py::index_error(); 
             a[get<0>(i)][get<1>(i)] = v;
             })

    .def(py::self * py::self)
    .def(double() * py::self)
    .def(py::self * double())
    ;


return m.ptr();
}

