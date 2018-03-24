/**
 * File : myList.hpp
 * Brief : List class implement
*/

/**
 * Tasks:
 *  1. 定义list结点数据结构
 *  2. 定义list 迭代器结构
 *  3. 定义list分配器，每次分配一个结点大小
 *  4. 逐步实现list模板类
*/

#ifndef MY_LIST_H_
#define MY_LIST_H_

#include <memory>
#include <cstddef> //for ptrdiff_t
#include <algorithm>
#include "iterator.h"
//#include <???>    // for bidirectional_iterator_tag

namespace mystl {

// list结点结构
template <class T>
struct __list_node {
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

// list迭代器
template <class T, class Ref, class Ptr> //iterator_category、difference_type可以确定
class __list_iterator {
public:
    typedef __list_iterator<T, T&, T*>  iterator;
    typedef __list_iterator<T, Ref, Ptr>  list_iterator_type;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef __list_node<T>* link_type;

    link_type pNode;

    //构造函数
    __list_iterator() {}
    __list_iterator(link_type x) : pNode(x) {}
    __list_iterator(const iterator& x) : pNode(x.pNode) {}

    bool operator==(const list_iterator_type& x) const { return pNode == x.pNode; }
    bool operator!=(const list_iterator_type& x) const { return pNode != x.pNode; }

    // 取值
    reference operator*() const { return (*pNode).data; } //迭代器所指之对象（可能会修改，所以传回引用）

    // 成员存取
    pointer operator->() const { return &(operator*()); } //迭代器所指之对象的地址

    list_iterator_type& operator++() //前置++
    {
        // pNode = (link_type)(pNode->next); 
        pNode = (link_type)((*pNode).next);        
        return *this;
    }

    list_iterator_type operator++(int) //后置++
    {
        list_iterator_type tmp = *this;
        ++*this;
        return tmp;
    }

    list_iterator_type& operator--()
    {
        pNode = (link_type)((*pNode).prev);
        return *this;
    }

    list_iterator_type operator--(int) // 后置--
    {
        list_iterator_type tmp = *this;
        --*this;
        return tmp;
    }

    // list迭代器不应该提供+n -n操作， list是循环链表
    // list迭代器属于bidirectional级别，不应该有随机访问特性
    
    // list_iterator_type operator+(int n){}
    // list_iterator_type operator-(int n){}
};

// 专属空间配置器， 每次配置一个节点大小
template <class T, class Alloc = std::allocator<__list_node<T>>>
class list_allocator
{
public:
    typedef __list_node<T>* link_type;
    
    link_type create_node(const T& x) {
        link_type p = get_node();
        list_alloc_.construct(&p->data, x);
        return  p;
    }

    void destroy_node(link_type p)
    {
        list_alloc_.destroy(p);
        free_node(p);
    }

    link_type get_node() { return list_alloc_.allocate(1); }
    void free_node(link_type p) { list_alloc_.deallocate(p, 1); }

protected:
    std::allocator<__list_node<T>> list_alloc_;
};

template <class T, class Alloc = list_allocator<T>>
class list {
public:
    typedef __list_node<T>* link_type;
    typedef __list_iterator<T, T&, T*> iterator;
    typedef T               value_type;
    typedef value_type*     pointer;
    typedef value_type&     reference;
    typedef const iterator  const_iterator;
    typedef const reference const_reference;
    typedef size_t          size_type;
    typedef Alloc           allocator_type;
    
protected:
    __list_node<T> list_node;

    Alloc alloc_;
    link_type pNode;  //一个指针串起整个环状双向链表

public:
    //构造器
    list(); // 构造空表（有一个空结点）
    explicit list(size_type count, const T& value); //定义list<int> list(8, 10);没有匹配
                                            //(原因： list<int, int>最优匹配是初始化列表构造方式，在初始化列表构造方式没有实现的情况下, 报链接错误)
    explicit list(size_type count);
    list(const list& other);
    // list(list&& other);
    // list(std::initializer_list<T> init);
    // template <class InputIt>
    // list(InputIt first, InputIt last);
    ~list();

    list& operator=(const list& other);
    // list* operator=(list&& other);
    list& operator=(std::initializer_list<T> list);

    void assign(size_type count, const T& value);
    template <class InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);

    //元素访问
    reference front() { return (*(pNode->next)).data; }
    // const_reference front() const;
    reference back() { return (*(pNode->prev)).data; }
    // const_reference back() const;

    //迭代器
    iterator begin() noexcept { return iterator((link_type)(pNode->next)); };
    const_iterator begin() const noexcept { return const_iterator((link_type)(pNode->next)); }
    // const_iterator cbegin() const noexcept;
    iterator end() noexcept { return iterator(pNode); }
    const_iterator end() const noexcept { return const_iterator(pNode); }
    // const_iterator cend() const noexcept;
    
    // reverse_iterator rbegin() noexcept;
    // const_reverse_iterator rbegin() const noexcept;
    // const_reverse_iterator crbegin() const noexcept;
    // reverse_iterator rend() noexcept;
    // const_reverse_iterator rend() const noexcept;
    // const_reverse_iterator crend() const noexcept;

    //容量
    bool empty() const noexcept { return pNode->next == pNode; }
    size_type size() const noexcept;
    
    //修改器
    void clear() noexcept { erase(begin(), end()); }
    iterator insert(iterator pos, const T& value);
    // iterator insert(iterator pos, T&& value);
    iterator insert(iterator pos, size_type count, const T& value);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    
    void push_back(const T& value) { insert(end(), value); }
    // void push_back(T&& value);

    void pop_back() { iterator tmp = end(); erase(--tmp); }
    void push_front(const T& value) { insert(begin(), value); }
    // void push_front(T&& value) ;
    void pop_front() { erase(begin()); }

    void resize(size_type count);
    void resize(size_type count, const value_type& value);

    void swap(list& other);

    //操作
    // merge 合并操作， 两个list必须先经过递增排序
    void merge(list& other);
    void merge(list&& other);
    template <class Compare>
    void merge(list& other, Compare comp);

    //接合，把符合条件的list串接合到pos之前
    void splice(const_iterator pos, list& other);
    // void splice(const_iterator pos, list&& other);
    void splice(const_iterator pos, list& other, const_iterator it);
    // void splice(const_iterator pos, list&& other, const_iterator it);
    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);
    // void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last);

    void remove(const T& value);

    void reverse();

    void unique();

    void sort(); // list有自己独立的sort函数

    
protected:
    void empty_initialize()
    {
        pNode = alloc_.get_node(); //只是分配空间，不构造元素值
        pNode->prev = pNode;
        pNode->next = pNode;
    }

    void transfer(iterator pos, iterator first, iterator last); // 迁移动作

}; // end class list


template <class T, class Alloc>
list<T, Alloc>::list()
{
    //构造一个空表
    empty_initialize();
}

template <class T, class Alloc>
list<T, Alloc>::list(size_type count, const T& value)
{
    // 先构造一个空表，然后逐个插入（这里可以使用到insert的代码）
    empty_initialize();
    while(count--)
    {
        insert(end(), value);
    }
}

template <class T, class Alloc>
list<T, Alloc>::list(size_type count)
{
    // 先构造一个空表，然后逐个插入（这里可以使用到insert的代码）
    empty_initialize();
    while(count--)
    {
        insert(end(), T());
    }
}

template <class T, class Alloc>
list<T, Alloc>::list(const list<T, Alloc>& other)
{
    empty_initialize();
    iterator it = other.begin();
    while(it != other.end()) {
        insert(end(), *it);
        ++it;
    }
}

template <class T, class Alloc>
list<T, Alloc>::~list()
{
    erase(begin(), end());
    alloc_.destroy_node(pNode);
}

template <class T, class Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const list& other)
{
    if(&other == this)
        return *this;

    list<T, Alloc> tmp(other);
    swap(tmp);
    return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::size_type list<T, Alloc>::size() const noexcept
{
    iterator it = begin();
    size_type size = 0;
    while(it++ != end())
    {
        size++;
    }
    return size;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator pos, const T& value)
{
    link_type p = alloc_.create_node(value);
    p->next = pos.pNode;
    p->prev = pos.pNode->prev;
    ((link_type)(pos.pNode->prev))->next = p;
    pos.pNode->prev = p;

    return p;
}

// template <class T, class Alloc>
// typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator pos, T&& value)
// {
//     insert(pos, value);  //  ????怎么实现
// }

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator pos, size_type count, const T& value)
{
    while(count--) {
        pos = insert(pos, value);
    }
    return pos;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator pos)
{
    link_type next_node = link_type(pos.pNode->next);
    link_type prev_node = link_type(pos.pNode->prev);
    prev_node->next = next_node;
    next_node->prev = prev_node;
    alloc_.destroy_node(pos.pNode);
    return iterator(next_node);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last)
{
    iterator it = first;
    while(it != last)
    {
        it = erase(it);
    }
}

template <class T, class Alloc>
void list<T, Alloc>::resize(size_type count)
{
    resize(count, T());
}

template <class T, class Alloc>
void list<T, Alloc>::resize(size_type count, const value_type& value)
{
    if(count <= size())
    {
        size_type len = size() - count;
        while(len--) {
            iterator it = end();
            erase(--it);
        }
    } else {
        size_type len = count - size();
        while(len--) {
            insert(end(), value);
        }
    }
}

template <class T, class Alloc>
void list<T, Alloc>::swap(list<T, Alloc>& rhs)
{
    link_type tmp = rhs.pNode;
    rhs.pNode = pNode;
    pNode = tmp;
}

template <class T, class Alloc>
void list<T, Alloc>::remove(const T& value)
{
    iterator first = begin();
    iterator last = end();
    while(first != last) {
        iterator next = first;
        ++next;
        if(*first == value) erase(first);
        first = next;
    }
}

template <class T, class Alloc>
void list<T, Alloc>::unique() {
    iterator first = begin();
    iterator last = end();
    if(first == last) return;

    iterator next = first;
    while(++next != last) {
        if(*first == *next)
        {
            erase(next);
        } else {
            first = next;
        }
        next = first;
    }
}

template <class T, class Alloc>
void list<T, Alloc>::transfer(iterator pos, iterator first, iterator last)
{
    if(pos != last) {
        ((link_type)(last.pNode->prev))->next = pos.pNode;
        ((link_type)(first.pNode->prev))->next = last.pNode;
        ((link_type)(pos.pNode->prev))->next = first.pNode;
        link_type tmp = (link_type)pos.pNode->prev;
        pos.pNode->prev = last.pNode->prev;
        last.pNode->prev = first.pNode->prev;
        first.pNode->prev = tmp;
    }
}

template <class T, class Alloc>
void list<T, Alloc>::splice(iterator pos, list& other)
{
    if(!other.empty())
    {
        transfer(pos, other.begin(), other.end());
    }
}

template <class T, class Alloc>
void list<T, Alloc>::splice(iterator pos, list& other, iterator i)
{
    iterator j = i;
    ++j;
    if(pos == i || pos == j) return;
    transfer(pos, i, j);
}

// pos可以和[first, last）指向同一个list
// pos不能在[first, last）区间之内
template <class T, class Alloc>
void list<T, Alloc>::splice(iterator pos, list& other, iterator first, iterator last)
{
    if(first != last)
        transfer(pos, first, last);
}

template <class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x) 
{
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end()

    //注： 两个list都必须已经排好序
    while(first1 != last1 && first2 != last2) {
        if(*first2 < *first1) {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        }
        else {
            ++first1;
        }
    }
    if(first2 != last2)
        transfer(last1, first2, last2);
}

template <class T, class Alloc>
void list<T, Alloc>::reverse() {
    if(size() == 0 || size() == 1) {
        return;
    }

    iterator first = begin();
    ++first;
    while(first != end())
    {
        iterator pick = first;
        ++first;
        transfer(begin(), pick, first);
    }
}

// list不能使用STL算法sort()
// 因为STL算法只接受RamdomAccessIterator
// 本函数使用快排 (非递归的归并思想)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
template <class T, class Alloc>
void list<T, Alloc>::sort() 
{
    if(pNode->next == pNode || link_type(pNode->next)->next == pNode)
        return;

    list<T, Allc> carry;
    list<T, Alloc> counter[64];
    int fill = 0;
    while(!empty()) {
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while(i < fill && !counter[i].empty()) {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i++]);
        if( i == fill ) ++fill;
    }

    for(int i = 1l i < fill; ++i)
    {
        counter[i].merge(counter[i-1]);
    }
    swap(counter[fill -1]);
}
}

} // end namespace mystl
#endif