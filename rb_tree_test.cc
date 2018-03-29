#include "rb_tree.hpp"
#include <iostream>
#include <functional>
#include <memory>
using namespace std;
using namespace mystl;

class KeyOfValue {
public:
    int operator()(int i) {
        return i;
    }
};

int main()
{
    typedef rb_tree<int, int, KeyOfValue, std::less<int>>::iterator rbiterator;
    rb_tree<int, int, KeyOfValue, std::less<int>> t1;
    cout << ((*(t1.header)).color) << endl;
    cout << ((*(t1.header)).left) << endl;
    cout << ((*(t1.header)).right) << endl;
    cout << t1.node_count << endl;

    typedef rb_tree_node<int>* link_type;
    t1.insert_unique(4);
    t1.insert_unique(2);
    t1.insert_unique(33);
    t1.insert_unique(42);
    t1.insert_unique(4);
    t1.insert_unique(2);
    t1.insert_unique(33);
    t1.insert_unique(42); 
    t1.insert_unique(4);
    t1.insert_unique(2);
    t1.insert_unique(33);
    t1.insert_unique(42); 
    t1.insert_unique(4);
    t1.insert_unique(2);
    t1.insert_unique(33);
    t1.insert_unique(42); 
    rbiterator it = t1.begin();
    for(; it != t1.end(); it++){
        std::cout << *it << endl;
    }
}