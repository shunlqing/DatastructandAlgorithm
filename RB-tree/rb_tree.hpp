/**
 * File：rb_tree.hpp
 * Brief : rb_tree implement
 * Authoer : shunlqing@163.com
 * Data : 2018-3-24
*/

/**
 * Plan :
 *  1. RB-tree节点设计
 *  2. 迭代器设计
 *  3. 数据结构
 *  4. 构造和内存管理
 *  5. 元素操作
*/

#ifndef RB_TREE_H_
#define RB_TREE_H_

#include "../iterator.h"
#include <cstddef>
#include <utility>
#include <memory>

namespace mystl {

/**
 * RB-tree节点设计
*/

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

// node_base,为构成红黑树状结构而设计。可以看做红黑树数据结构的“关节”。
struct __rb_tree_node_base {
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;

    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    static base_ptr minimun(base_ptr x) {
        while(x->left != 0) x = x->left;
        return x;
    }

    static base_ptr maxmum(base_ptr x) {
        while(x->right != 0) x = x->right;
        return x;
    }
};

// 完整的节点数据结构，在node_base上增加数据域，不然红黑树就只是个空架子。。
template <class Value>
struct __rb_tree_node : public __rb_tree_node_base {
    typedef __rb_tree_node<Value>* link_type;
    Value value_field; //节点存储
};

/**
 * 迭代器设计
 * 
 * 节点数据结构若看做是定义静态结构，那么迭代器是让rb-tree变得动态，成为泛型容器的关键。
 * 支持操作：
 *  考虑类别: 双向迭代器，不支持随机定位能力
 *  前进increment，后退decrement
 *  解引用dereference
 *  成员访问member access
 * 
 * 和节点设计相对应，使用双层迭代器结构设计
*/

// 基层迭代器
struct __rb_tree_base_iterator
{
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;

    base_ptr node;

    void increment() {
        if(node->right != 0) {
            node = node->right;
            while(node->left != 0) {
                node = node->left;
            }
        }
        else {
            base_ptr y = node->parent;
            while(node = y->right) {
                node = y;
                y = y->parent;
            }
            if(node->right != y)
                node = y;
        }
    }

    void decrement() {
        if(node->color == __rb_tree_red && node->parent->parent == node) {
            node = node->right;
        } 
        else if(node->left != 0) {
            base_ptr y = node->left;
            while(y->right != 0) {
                y = y->right;
            }
            node = y;
        }
        else {
            base_ptr y = node->parent;
            while(node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template <class Value, class Ref, class Ptr>
struct __rb_tree_iterator : public __rb_tree_base_iterator{
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value, Value&, Value*>   iterator;
    typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator; //const_iterator原来是这么定义
    typedef __rb_tree_iterator<Value, Ref, Ptr>  iterator_type;
    typedef __rb_tree_node<Value>* link_type;

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) : node(x) {}
    __rb_tree_iterator(const iterator& it) { node = it.node; }

    reference operator*() const { return link_type(node)->value_field; }
    pointer operator->() const { return &(operator*()); }

    iterator_type& operator++() { increment(); return *this; }
    iterator_type operator++(int) {
        iterator_type tmp = *this;
        increment();
        return tmp;
    }

    iterator_type& operator--() { decrement(); return *this; }
    iterator_type operator--(int) {
        iterator_type tmp = *this;
        decrement();
        return tmp;
    }
};

/**
 * rb-tree模板类
*/

template <class Key, class Value,  class KeyOfValue, class Compare, class Alloc = std::alloctor<__rb_tree_node>>
class rb_tree {
protected:
    typedef void* void_pointer;
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef __rb_tree_color_type color_type;
    typedef std::allocator<rb_tree_node> rb_tree_node_allocator;

public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type*  poniter;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_typed& const_reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:

    rb_tree_node_allocator alloc_;

    link_type get_node() { return alloc_.allocate(1); }
    void put_node(link_type p) { alloc_.deallocate(p, 1); }

    link_type create_node(const value_type& x) {
        link_type tmp = get_node();
        alloc_.construct(&tmp->value_field, x);
        return tmp;
    }

    link_type clone_node(link_type x) {
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = 0;
        tmp->right = 0;
        return tmp;
    }

    void destory_node(link_type p) {
        alloc_.destroy(&p->value_field);
        put_node(p);
    }

protected:
    //RB-tree 只以三笔数据表现
    size_type node_count;    //追踪记录树的大小
    link_type header;   //这是实现上的一个技巧
    Compare key_compare; // 节点间键值的比较准则。可以是个function object

    // 三个函数用来获取header的成员
    link_type& root() const { return (link_type&)header->parent; }
    link_type& leftmost() const { return (link_type&)header->left; }
    link_type& rightmost() const { return (link_type&)header->right; }

    // 以下六个成员方便获取节点x的成员
    static link_type& left(link_type x) { return (link_type&)(x->left); }
    static link_type& right(link_type x) { return (link_type&)(x->right); }
    static link_type& parent(link_type x) { return (link_type&)(x->parent); }
    static reference value(link_type x) { return x->value_field; }
    static const key_type& key(link_type x) { return KeyOfValue()(value(x)); }
    static color_type& color(link_type x) { return (color_type&)(x->color); }

    // 以下六个成员方便获取节点x的成员
    static link_type& left(base_type x) { return (link_type&)(x->left); }
    static link_type& right(base_type x) { return (link_type&)(x->right); }
    static link_type& parent(base_type x) { return (link_type&)(x->parent); }
    static reference value(base_type x) { return ((link_type)x)->value_field; }
    static const key_type& key(base_type x) { return KeyOfValue()(value((link_type)x)); }
    static color_type& color(base_type x) { return (color_type&)((link_type)x->color); }

    static link_type minimum(link_type x) {
        return (link_type) __rb_tree_node_base::minimum(x);
    }

    static link_type maximum(link_type x) {
        return (link_type) __rb_tree_node_base::maximum(x);
    }

public:
    typedef __rb_tree_iterator<value_type, reference, pointer> iterator;

private:
    iterator __insert(base_ptr x, base_ptr y, const value_type& v);
    link_type __copy(link_type x, link_type p);
    void erase(link_type pos);
    void init() {
        header = get_node();
        color(header) = __rb_tree_red;

        root() = 0;
        leftmost() = header;
        rightmost() = header;
    }

public:
    rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) { init(); }

    ~rb_tree() {
        clear();
        put_node(header);
    }
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

public:

    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost; }
    iterator end() { return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }

public:
    pair<iterator, bool> insert_unique(const value_type& x);
    iterator insert_equal(const value_type& x);
    //... 
};

// 允许键值重复 插入操作
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const Value& v)
{
    //允许节点键值重复
    //返回是一个RB-tree迭代器,指向新增节点
    link_type y = header;
    link_type x = root();
    while(x ！= 0) {
        y = x;
        x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
{
    //不允许节点键值重复
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while(x != 0 ) {
        y = x;
        comp = key_compare(KeyOfValue()(x), key(x));
        x = comp ? left(x) : right(x);
    }

    iterator j = iterator(y);
    if(comp) {
        if(j == begin()) {
            return pair<iterator, bool>(__insert(x, y, v), true);
        } else {
            --j;
        }
    }

    if(key_compare(KeyOfValue()(key(j.node)), KeyOfValue()(v)) {
        return pair<iterator, bool>(__insert(x, y, v), bool);
    }

    return pair<iterator, bool>(j, false);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<class Key, class Value, class KeyOfValue, class Compare, class Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const Value& v) {
    link_type x = (link_type)x_;
    link_type y = (link_type)y_;
    link_type z;

    if(y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
        z = alloc_.create_node(v);
        left(y) = z;
        if(y == header) {
            root() = z;
            rightmost() = z;
        }
        else if(y == leftmost()) {
            leftmost() = z;
        }
    }
    else {
        z = alloc_.create_node(v);
        right(y) = z;
        if(y == rightmost()) {
            rightmost() = z;
        }
    }
    parent(z) = y;
    left(z) = 0;
    right(z) = 0;

    __rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);
}

inline void
__rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) 
{
    x->color = __rb_tree_red;
    while(x != root && x->parent->color == _rb_tree_red) {
        if(x->parent == x->parent->parent->left) {
            __rb_tree_node_base* y = x->parent->parent->right;
            if(y && y->color == _rb_tree_red) { //有伯父节点，且伯父节点为红
                x->parent->color = __rb_tree_black;
                y->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent; 
            }
            else {
                if(x = x->parent->right) {
                    x = x->parent;
                    __rb_tree_rotate_left(x, root);
                }
                x->parent->color = __rb_tree_black; 
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root);
            }
        }
        else {
            __rb_tree_node_base* y = x->parent->parent->left;
            if(y&& y->color == __rb_tree_red) {
                x->parent->color = __rb_tree_balck;
                y->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent;
            }
            else {
                if(x == x->parent->left) {
                    x = x->parent;
                    __rb_tree_rotate_right(x, root);
                }
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    root->color = __rb_tree_black;
}

template <class >

} // end of namespace mystl

#endif
