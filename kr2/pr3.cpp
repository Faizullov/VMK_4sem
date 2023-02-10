#include <iostream>

using namespace std;
class A{
    int x;
    public:
    int f1(){
        A i;
        A j;
        return (i,j)[i,j](i,j);
    }
    friend A operator ,(A i, A j);
    A operator ()(A i, A j){
        return i;
    }
    A operator ()(A i){
        return i;
    }
    A operator [](A i){
        return i;
    }
    operator int () const{
        return 5;
    }
};
A operator ,(A i, A j){
    return i;
}

int main(){
    return 0;
}