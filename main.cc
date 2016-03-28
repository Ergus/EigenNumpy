#include "cnumpy.h"

int main(){
    cmatrix a(5,6,'r'), b(6,7,'r');

    cout<<"random a:\n"<<a<<endl;
    cout<<"random b:\n"<<b<<endl;
    cmatrix c=a*b;
    cout<<"c=a*b:\n"<<c<<endl;
    b=a;
    cout<<"b=a:\n"<<b<<endl;
    cmatrix d(a);
    cout<<"d(a):\n"<<d<<endl;
    }
