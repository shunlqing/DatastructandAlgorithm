#include <iostream>
#include "myVector.h"
#include <memory>
using namespace std;

int main()
{
    MyVector<int> vec(2);
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;
    cout << "is vec empty?"  << ((vec.empty()) ? "yes" : "no") << endl;

    vec.push_back(10);
    vec.push_back(1);
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;
    // cout << "vec.begin()" << vec.begin() << endl;
    // cout << "vec.end()" << vec.end() << endl;

    vec.push_back(10);
    vec.push_back(1);
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;

    vec.pop_back();
    vec.pop_back();
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;

    vec.erase(vec.begin());
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;

    vec.reserve(10);
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;

    vec.shrink_to_fit();
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;

    MyVector<int> vec2(5, 5);
    vec.swap(vec2);
    cout << "vec.size(): " << vec.size() << endl;
    cout << "vec.capacity(): " << vec.capacity() << endl;
    cout << "vec2.size(): " << vec2.size() << endl;
    cout << "vec2.capacity(): " << vec2.capacity() << endl;

    cout << "vec.front(): " << vec.front() << endl;
    vec.front() = 10;
    cout << "vec.front(): " << vec.front() << endl;
    cout << "vec[2]: " << vec[2] << endl;

    cout << "vec.data(): " << *(vec.data()) << endl;
    *(vec.data()) = 100;
    cout << "vec.data(): " << *(vec.data()) << endl;    

    for(auto s : vec)
    {
        cout << s << " ";
    };
    cout << endl;
    vec.erase(vec.begin(), vec.begin()+3);
    for(auto s : vec)
    {
        cout << s << " ";
    };
}