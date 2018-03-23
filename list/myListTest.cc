#include "myList.h"
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    vector<int> vec(1, 10);
    std::cout << vec[0] << endl;
    int a = 0;
    mystl::list<int> list1(8, a);
    // mystl::list<vector<int>> list(8, vec);    
}