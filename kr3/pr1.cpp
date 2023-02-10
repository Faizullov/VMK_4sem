#include <iostream>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

template <class T>
void print_stl(T const & tmp){
    typename T :: const_iterator p = tmp.begin();
    while (p != tmp.end()){
        cout << *p;
        p++;
    }
    cout << " | ";
    p = tmp.end();
    while (p != tmp.begin()){
        p --;
        cout << *p;
    }
    cout << endl;
}

template <class V, class L>
V change (V &vect, L const &lst){
    int cnt = 0;
    typename V :: const_iterator v = vect.begin();
    typename L :: const_iterator l = lst.begin();
    l++;
    while (v != vect.end() && l != lst.end()){
        if (cnt % 2 != 0){
            v = vect.insert(v + 1, *l);
        }
        l++;
        v++;
        cnt++;
    }
    return vect;
}

template <class T>
void clear (T & tmp){
    int cnt = 1;
    typename T :: const_iterator p = tmp.begin();
    while (p != tmp.end()){
        if (cnt % 3 == 0){
            p = tmp.erase(p);
            cnt = 1;
        } else {
            p ++;
            cnt ++;
        }
    }
}
/*template <class T>
int search (T const &tmp, int v){
    typename T :: const_iterator p = tmp.begin();
    while (p != tmp.end()){
        p++;
        if (p == tmp.end()){
            break;
        }
        if (*p == v){
            return 1;
        }
    }
    return 0;
} */

int main(){
    vector<int> v(6);
    for (int i = 0; i < 6; i++){
        v[i] = i;
    }
    list<int> l2 = {1, 2, 3, 4, 5, 6};
    const list<int> l = {2, 3, 4, 5 ,6};
    print_stl(v);
    print_stl(l);
    v = change(v, l);
    print_stl(v);
    clear(v);
    print_stl(v);
    clear (l2);
    print_stl(l2);
    return 0;
}

