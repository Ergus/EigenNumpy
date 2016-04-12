#include "cnumpy.h"

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

cvector cmatrix::begin() const{return cvector(c,data);}
cvector cmatrix::end() const{return cvector(c,&data[full]);}

ostream &operator<<(ostream &out,const cmatrix &in){
    for (cvector &i : in){
        cout<<i;
        }
    return out;
    }

//Expose to python

PYBIND11_PLUGIN(libcnumpy){
py::module m("libcnumpy", "Mini numpy en C++");

py::class_<cmatrix>(m,"cmatrix")
    // Constructores
    .def(py::init<>(),"Constructor por defecto")
    .def(py::init<int,int,char>(),
         "Constructor basico",
         py::arg("int"), py::arg("int"),
         py::arg("char") ='0')
    // Constructor desde un numpy array o cualquier buffer
    .def("__init__",          
         [](cmatrix &in, py::buffer b, bool copy=false){
             py::buffer_info info = b.request();
             new (&in) cmatrix(info.shape[0], info.shape[1], (double*)info.ptr, copy);
             },
         "Constructor desde buffer",
         py::arg("buffer")=false,
         py::arg("copy")=false

        )
    .def(py::init<cmatrix>(),"Constructor copia")

    // Otras funciones
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

    .def("__getitem__", [](cmatrix &a, tuple<py::slice, py::slice> slices) -> cmatrix {
            py::ssize_t startr, stopr, stepr, slicelengthr,
                startc, stopc, stepc, slicelengthc;

            py::slice slice=get<0>(slices);
            if (!slice.compute(a.rows(), &startr, &stopr, &stepr, &slicelengthr))
                throw py::error_already_set();

            slice=get<1>(slices);
            if (!slice.compute(a.cols(), &startc, &stopc, &stepc, &slicelengthc))
                throw py::error_already_set();
            
            cmatrix out(slicelengthr,slicelengthc);
            
            for(size_t i=0, itr=startr;
                i<slicelengthr;
                ++i, itr+=stepr) {
                
                for(int j=0, itc=startc;
                    j<slicelengthc;
                    j++, itc+=stepc){
                    
                    const double val=a.get(itr,itc);
                    out.set(i,j,val);
                    
                    }
                }
            return out;
            }
        )    

    .def("__setitem__", [](cmatrix &a, tuple<int, int> i,
                           double v) {
             if (get<0>(i)>= a.rows() || get<1>(i)>=a.cols())
                 throw py::index_error(); 
             a[get<0>(i)][get<1>(i)] = v;
             })

    .def_buffer([](cmatrix &in) -> py::buffer_info {
             return py::buffer_info(
                                   (double*)(in), sizeof(double),
                                   py::format_descriptor<double>::value(),
                                   2,
                                   { in.rows(), in.cols() },
                                   { sizeof(double)*in.cols(),
                                           sizeof(double) }
                                   );
            })
    
    // Multiplicaciones
    .def(py::self * py::self)
    .def(double() * py::self)
    .def(py::self * double())
    ;


return m.ptr();
}

