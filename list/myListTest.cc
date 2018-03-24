#include "myList.hpp"
#include <iostream>
using namespace std;
using namespace mystl;

void printList(list<int>& li)
{
    cout << "list size: " << li.size() << endl;
    for(auto x : li)
    {
        cout << x << " ";
    }
    cout << endl << endl;
}

int main()
{
    list<int> li;
    printList(li);

    cout << "is empty? " << (li.empty() ? "yes" : "no") << endl << endl;

    list<int> li1(5);
    printList(li1);

    list<int> li2(5, 20);
    printList(li2);

    li.insert(li2.end(), 1);
    printList(li2);

    li2.push_back(2);
    printList(li2);
    
    li2.push_front(3);
    printList(li2);

    li2.pop_back();
    printList(li2);

    li2.pop_front();
    printList(li2);

    // li2.insert(li2.begin()+2, 5, 18); //不能这样使用，list_iterator属于bidirectional级别，不具有随机访问的特性
    li2.insert(li2.begin(), 5, 18);    
    printList(li2);

    list<int> li3;
    li3.swap(li2);
    printList(li3);
    printList(li2);

    li2 = li3;
    printList(li3);
    printList(li2);
    
    cout << "test resize() ..." << endl;
    li2.resize(li2.size() - 4);
    li3.resize(li3.size() + 4, 19);
    printList(li3);
    printList(li2);

    cout << "test clear() ..." << endl;
    li2.clear();
    printList(li2);

    cout << "test remove() ..." << endl;
    li3.remove(20);
    printList(li3);

    list<int> li4(4, 10);

    li4.transfer(li4.begin(), li3.begin(), li3.end());
    printList(li4);    
}