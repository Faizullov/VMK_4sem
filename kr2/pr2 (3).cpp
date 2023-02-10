#include <iostream>
using namespace std;

class A {
    int a;
    ~A() {}
    public:
    A(int i = 0){
        a = i;
    }
    A & operator = (const A & y){
        a = y.a;
        return *this;
    }
    A *create(int a1 = 0) {
        A * c = new A(a1);
        return c;
    }
    void clear () {
        delete this;
    }
    void print() {
        cout << a << endl;
    }
};

int main() {
    A * p_main = (A*)::operator new (3 * sizeof(A));
    A * p = new (p_main) A;
    p = p->create(10);
    p_main = p_main + 1;
    A * p1 = new (p_main) A;
    p1 = p1->create(20);
    p_main = p_main + 1;
    A * p2 = new (p_main) A;
    p2 = p2->create(30);
    p->clear();
    p1->clear();
    p2->clear();
    p_main = p_main - 2;
    ::operator delete(p_main);
    return 0;
}