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
    t1.insert_unique(10);
    t1.insert_unique(7);
    t1.insert_unique(8);
    t1.insert_unique(15);
    t1.insert_unique(5);
    t1.insert_unique(6);
    t1.insert_unique(11);
    t1.insert_unique(13); 
    t1.insert_unique(12);
    // t1.insert_unique();
    // t1.insert_unique(3366);
    // t1.insert_unique(4254); 
    // t1.insert_unique(4);
    // t1.insert_unique(2);
    // t1.insert_unique(33);
    // t1.insert_unique(42); 
    rbiterator it = t1.begin();
    for(; it != t1.end(); it++){
        std::cout << *it << " color: " << it.node->color << endl;
    }

    if((it = t1.find(12)) == t1.end()) {
        std::cout << "not have 22";
    } else {
        std::cout << *it << endl;
    }
    
}