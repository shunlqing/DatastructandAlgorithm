/**
 * File : rb_tree.hpp
 * Brief : rb_tree class implement
 * Author : shunlqing@163.com
 * Data : 2018-3-29
*/

#ifndef RB_TREE_HPP_
#define RB_TREE_HPP_

#include <cstddef>
#include <memory>
#include <utility>
using namespace std; 

namespace mystl {

/**
 * Design RB-tree node structure
*/
typedef bool rb_tree_color_type;
const rb_tree_color_type rb_tree_red = false;
const rb_tree_color_type rb_tree_balck = true;

template <class Value>
class rb_tree_node {
public:
    typedef rb_tree_node<Value>* link_type;
    typedef rb_tree_color_type color_type;

    color_type color;
    link_type parent;
    link_type left;
    link_type right;
    Value val;
};

template <class Value, class Ref, class Ptr>
class rb_tree_iterator {
public:
    typedef rb_tree_node<Value>* link_type;

    // iterator attributes
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;
    typedef rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef rb_tree_iterator<Value, Value&, Value*> iterator_type;

    //
    reference operator*() const { return node->val; }
    pointer operator->() const { return &(operator*()); }//???
    bool operator==(const iterator_type& rhs) const { return node == rhs.node; }
    bool operator!=(const iterator_type& rhs) const { return node != rhs.node; }
    

    // ctors
    rb_tree_iterator() {}
    rb_tree_iterator(link_type x) { node = x; }
    rb_tree_iterator(const iterator& it) { node = it.node; }

    // 
    rb_tree_iterator<Value, Ref, Ptr>& operator++() { increment(); return *this; }
    rb_tree_iterator<Value, Ref, Ptr>& operator--() { decrement(); return *this; }
    rb_tree_iterator<Value, Ref, Ptr> operator++(int) { 
        iterator_type tmp = *this;
        increment();
        return tmp;
    }
    rb_tree_iterator<Value, Ref, Ptr> operator--(int) {
        iterator_type tmp = *this;
        decrement();
        return tmp;
    }
    
    void increment();
    void decrement();
    
    link_type node;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = std::allocator<rb_tree_node<Value>>>
class rb_tree {
public:
    typedef void* void_pointer;
    typedef rb_tree_color_type color_type;
    typedef rb_tree_node<Value>* link_type;
    typedef size_t size_type;
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef ptrdiff_t difference_type;
    typedef rb_tree_iterator<value_type, reference, pointer> iterator;

    // allocator
    link_type get_node();
    void put_node(link_type p);

    link_type create_node(const value_type& v);
    void destroy_node(link_type p);

    // ctor
    rb_tree(const Compare& comp);
    ~rb_tree();

    // accessors
    Compare key_comp();
    iterator begin() { return iterator(header->left); }
    iterator end() { return iterator(header); }
    bool empty() const { return node_count == 0; }
    size_type size() { return node_count; }
    size_type max_size();

    // modifiers
    iterator _insert(link_type x, link_type y, const value_type& v);
    link_type _copy(link_type x, link_type y);
    void _erase(link_type x);
    void _clear();

    std::pair<iterator, bool> insert_unique(const value_type& x);
    iterator insert_equal(const value_type& x);

public:
    size_type node_count;
    link_type header;
    Compare key_compare;
    std::allocator<rb_tree_node<Value>> alloc_;
};

template <class Value, class Ref, class Ptr>
void rb_tree_iterator<Value, Ref, Ptr>::increment()
{
    if(node->right != 0) {
        node = node->right;
        while(node->left != 0) {
            node = node->left;
        }
    } else {
        link_type y = node->parent;
        while(node == y->right) {
            node = y;
            y = node->parent;
        }
        if(node->right != y)
            node = y;
    }
}

template <class Value, class Ref, class Ptr>
void rb_tree_iterator<Value, Ref, Ptr>::decrement()
{
    if(node->color == rb_tree_red && node->parent->parent == node) {
        node = node->right;
    } else if(node->left != 0) {
        node = node->left;
        while(node->right != 0) {
            node = node->right;
        }
    } else {
         link_type y = node->parent;
         while(node == y->left) {
             node = y;
             y = node->parent;
            }
            node = y;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp)
{
    header = get_node();

    header->color = rb_tree_red;
    header->parent = 0;
    header->left = header;
    header->right = header;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::~rb_tree()
{
    // _clear();
    put_node(header);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::get_node()
{
    alloc_.allocate(1);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::put_node(link_type p)
{
    alloc_.deallocate(p, 1);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::create_node(const value_type& value)
{
    link_type p = get_node();
    alloc_.construct(&p->val, value);
    return p;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::destroy_node(link_type p)
{
    alloc_.destroy(&p->val);
    put_node(p);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(link_type x, link_type y, const value_type& v)
{
    link_type z;
    if(y == header || key_compare(KeyOfValue()(v), KeyOfValue()(y->val))) {
        z = create_node(v);
        y->left = z;
        if(y == header) {
            header->parent = z;
            header->right = z;
        } else if(y == header->left) {
            header->left = z;
        }
    } else {
        z = create_node(v);
        y->right = z;
        if(y == header->right) {
            header->right = z;
        }
    }
    z->parent = y;
    z->left = 0;
    z->right = 0;

    // rebalance
    ++node_count;
    return iterator(z);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v)
{
    link_type y = header;
    link_type x = header->parent;

    while(x != 0) {
        y = x;
        x = key_compare(KeyOfValue()(v), KeyOfValue()(x->val)) ? x->left : x->right;
    }
    return _insert(x, y, v);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
std::pair<rb_tree_iterator<Value, Value&, Value*>, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v)
{
    link_type y = header;
    link_type x = header->parent;
    bool comp = true;
    while(x != 0) {
        y = x;
        comp = key_compare(KeyOfValue()(v), KeyOfValue()(x->val));
        x = comp ? x->left : x->right;
    }

    iterator j = iterator(y);
    if(comp) {
        if(j == begin()) {
            return std::pair<iterator, bool>(_insert(x, y, v), true);
        } else {
            --j;
        }
    }
    
    if(key_compare(KeyOfValue()(*j), KeyOfValue()(v)))
        return std::pair<iterator, bool>(_insert(x, y, v), true);

    return std::pair<iterator, bool>(j, false);
}

};

#endif