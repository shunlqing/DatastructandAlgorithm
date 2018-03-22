/**
 * File : myVector.h
 * Brief : vector implement
*/

#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cstddef>  //for ptrdiff_t, size_t
#include <memory>
#include <algorithm>

template <typename T, class Alloc=std::allocator<T>>
class MyVector {
public:
    //内嵌类型声明
    typedef T           value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef const value_type* const_iterator;
    typedef const value_type& const_reference;
    typedef const pointer const_pointer;

protected:
    std::allocator<value_type>  alloc_; //空间配置器，stl标准写法
    iterator    start_;
    iterator    end_;
    iterator    end_of_storage_;


public:
    //构造函数
    MyVector() : start_(0), end_(0), end_of_storage_(0) {} //默认构造函数
    MyVector(size_type n, const T& value);
    explicit MyVector(size_type n);
    MyVector(const MyVector& v);
    MyVector& operator=(const MyVector& rhs);  //赋值操作符
    ~MyVector();

    //元素访问
    reference operator[](size_type pos) { return *(begin() + pos); }
    const_reference operator[](size_type pos) const { return *(begin() + pos); }
    reference front() { return *begin(); }
    const_reference front() const { return *cbegin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }
    T* data() { return begin(); }
    const T* data() const { return cbegin(); }

    //迭代器
    iterator begin() const { return start_; }
    iterator end() const { return end_; }
    const_iterator cbegin() const { return const_iterator(start_); }
    const_iterator cend() const { return const_iterator(end_); }

    //容量
    size_type size() const { return size_type(end() - begin()); } //若改成end() - begin(),引发错误(在begin()没有设置成const以前)，问题是，在const成员函数中调用非const成员函数
    size_type capacity() const { return size_type(end_of_storage_ - begin()); }
    bool empty() const { return begin() == end(); }
    void reserve(size_type new_cap); //对容量
    void shrink_to_fit();   //C++11 请求移除未使用的容量

    //修改器
    void clear() { erase(begin(), end()); }
    // iterator insert(iterator pos, const T& value);
    iterator insert(const_iterator pos, const T& value) { insert(pos, 1, value); } //C++11
    iterator insert(const_iterator pos, T&& value); //C++11
    iterator insert(iterator pos, size_type count, const T& value); //C++11
    template <class InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last); //C++11
    iterator insert(const_iterator pos, std::initializer_list<T> ilist); //C++11

    // template <class... Args>
    // iterator emplace(const_iterator pos, Args&&... args);
    
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void push_back(const T& value);
    void push_back(T&& value); //C++11

    void pop_back();
    void resize(size_type new_size); //resize() 短则截断，长则补上（面向的是已构造部分，区别于reserve()）
    void resize(size_type new_size, const value_type& value);

    void swap(MyVector& v);

    void _destroy(iterator first, iterator end)
    {
        while(end != first)
        {
            alloc_.destroy(--end);
        }
    }

};

//非成员函数
//暂时未实现
template<class T,  class Alloc=std::allocator<T>>
bool operator==(const MyVector<T, std::allocator<T>>& lhs, const MyVector<T, std::allocator<T>>& rhs);
template<class T,  class Alloc=std::allocator<T>>
bool operator!=(const MyVector<T, std::allocator<T>>& lhs, const MyVector<T, std::allocator<T>>& rhs);
template<class T,  class Alloc=std::allocator<T>>
bool operator<(const MyVector<T, std::allocator<T>>& lhs, const MyVector<T, std::allocator<T>>& rhs);
template<class T,  class Alloc=std::allocator<T>>
bool operator<=(const MyVector<T, std::allocator<T>>& lhs, const MyVector<T, std::allocator<T>>& rhs);
template<class T,  class Alloc=std::allocator<T>>
bool operator>(const MyVector<T, std::allocator<T>>& lhs, const MyVector<T, std::allocator<T>>& rhs);
template<class T,  class Alloc=std::allocator<T>>
bool operator>=(const MyVector<T, std::allocator<T>>& lhs, const MyVector<T, std::allocator<T>>& rhs);
template<class T, class Alloc=std::allocator<T>>
void swap(const MyVector<T, std::allocator<T>>& lhs, const MyVector<T, std::allocator<T>>& rhs);

template <class T, class Alloc>
MyVector<T, Alloc>::MyVector(size_type n, const T& value)
{
    start_ = alloc_.allocate(n);
    std::uninitialized_fill(start_, start_ + n, value);
    end_ = end_of_storage_ = start_ + n;
}

template <class T, class Alloc>
MyVector<T, Alloc>::MyVector(size_type n)
{
    start_ = alloc_.allocate(n);
    std::uninitialized_fill(start_, start_ + n, T());
    end_ = end_of_storage_ = start_ + n;
}

template <class T, class Alloc>
MyVector<T, Alloc>::MyVector(const MyVector<T, Alloc>& v)
{
    start_ = alloc_.allocate(v.size());
    end_ = end_of_storage_ = std::uninitialized_copy(v.cbegin(), v.cend(), start_);
}

template <class T, class Alloc>
MyVector<T, Alloc>& MyVector<T, Alloc>::operator=(const MyVector<T, Alloc>& rhs)
{
    if(&rhs == this)
        return *this;

    //TODO：原来的空间咋办？
    MyVector<T, Alloc> tmp(rhs);
    swap(rhs);
    return *this;   
}

template <class T, class Alloc>
MyVector<T, Alloc>::~MyVector()
{
    _destroy(start_, end_);
    alloc_.deallocate(start_, end_of_storage_ - start_);
    start_ = end_ = end_of_storage_ = 0;
}

template <class T, class Alloc>
void MyVector<T, Alloc>::swap(MyVector<T, Alloc>& v) //交换迭代器即可
{
    std::swap(start_, v.start_);
    std::swap(end_, v.end_);
    std::swap(end_of_storage_, v.end_of_storage_);
}

template <class T, class Alloc>
void MyVector<T, Alloc>::reserve(size_type new_cap) //当new_cap大于当前容量，则分配心存储，否则不做任何事
{
    if(new_cap > capacity())
    {
        //决定新容量大小： new_cap 或旧capacity的两倍，两者取大值
        const size_type new_cap_size = std::max(new_cap, 2 * capacity());
        iterator new_start = alloc_.allocate(new_cap_size);
        iterator new_end = new_start;

        new_end = std::uninitialized_copy(start_, end_, new_start);

        _destroy(start_, end_);
        alloc_.deallocate(start_, end_of_storage_ - start_);

        start_ = new_start;
        end_ = new_end;
        end_of_storage_ = new_start + new_cap_size;
    }
}

template <class T, class Alloc>
void MyVector<T, Alloc>::shrink_to_fit()
{
    // alloc_.deallocate(end_, end_of_storage_ - end_); //不能这么简单的释放部分, 分配器分配的时候
    // end_of_storage_ = end_;
    
    //重新分配的方式, C++11的实现方式？
    const size_type new_cap_size = size();
    iterator new_start = alloc_.allocate(new_cap_size);
    iterator new_end = new_start;

    new_end = std::uninitialized_copy(start_, end_, new_start);

    _destroy(start_, end_);
    alloc_.deallocate(start_, end_of_storage_ - start_);

    start_ = new_start;
    end_ = new_end;
    end_of_storage_ = new_start + new_cap_size;
}

template <class T, class Alloc>
void MyVector<T, Alloc>::push_back(const T& value)
{
    if(end_ != end_of_storage_)
    {
        alloc_.construct(end_, value);
        end_++;
    }
    else //重新分配
    {
        size_type new_size = std::max(capacity() * 2, size_type(1));
        iterator new_start = alloc_.allocate(new_size);
        iterator new_end = new_start;

        new_end = std::uninitialized_copy(start_, end_, new_start);
        alloc_.construct(new_end, value);
        new_end++;

        _destroy(start_, end_);
        alloc_.deallocate(start_, end_of_storage_ - start_);

        start_ = new_start;
        end_ = new_end;
        end_of_storage_ = start_ + new_size;
    }
}

template <class T, class Alloc>
void MyVector<T, Alloc>::push_back(T&& value)
{
    if(end_ != end_of_storage_)
    {
        alloc_.construct(end_, value);
        end_++;
    }
    else //重新分配
    {
        size_type new_size = std::max(capacity() * 2, size_type(1));
        iterator new_start = alloc_.allocate(new_size);
        iterator new_end = new_start;

        new_end = std::uninitialized_copy(start_, end_, new_start);
        alloc_.construct(new_end, value);
        new_end++;

        _destroy(start_, end_);
        alloc_.deallocate(start_, end_of_storage_ - start_);

        start_ = new_start;
        end_ = new_end;
        end_of_storage_ = start_ + new_size;
    }
}

template <class T, class Alloc>
void MyVector<T, Alloc>::pop_back()
{
    --end_;
   alloc_.destroy(end_);
}

template <class T, class Alloc>
typename MyVector<T, Alloc>::iterator MyVector<T, Alloc>::erase(iterator pos)
{
    if(pos + 1 != end()) { //删除的不是最后一个元素，需要拷贝
        std::copy(pos + 1, end_, pos);
    }
    --end_;
   alloc_.destroy(end_);
    return pos; //返回的是删除后指向的后一个元素
}

template <class T, class Alloc>
typename MyVector<T, Alloc>::iterator MyVector<T, Alloc>::erase(iterator first, iterator last) //删除[first, last)
{   
    if(last != end())
    {
        std::copy(last, end_, first);
    }
    
    difference_type left = end() - last;
    iterator it = first + left;
    while(end_ != it)
       alloc_.destroy(--end_);
    //STL : alloc_.destory(it, end());
    return first;
}

template <class T, class Alloc>
typename MyVector<T, Alloc>::iterator MyVector<T, Alloc>::insert(iterator pos, size_type count, const T& value)
{
    if(count != 0)
    {
        if(size_type(end_of_storage_ - end() >= count)) {
            //剩余空间足够
            const size_type elem_after = end_ - pos;
            iterator old_end = end_;
            if(elem_after > count) { //由于未使用空间未构造，所以需要分情况考虑
                std::uninitialized_copy(end_ - count, end_, end_);
                end_ += count;
                std::copy_backward(pos, old_end - count, old_end);
                std::fill(pos, pos + count, value);
            } else {
                std::uninitialized_fill_n(end_, count - elem_after, value);
                end_ += count - elem_after;
                std::uninitialized_copy(pos, old_end, end_);
                end_ += elem_after;
                std::fill(pos, end_, value);
            }
        } else {
            //剩余空间不足
            //决定新长度：旧长度的两倍，或旧长度+新增元素个数
            const size_type old_size = size();
            const size_type new_size = size() + std::max(old_size, count);

            //重新分配
            iterator new_start =alloc_.allocate(new_size);
            iterator new_end = new_start;

            //拷贝元素(没有考虑发生异常的情况)
            new_end = uninitialized_copy(start_, pos, new_start);
            new_end = uninitialized_fill_n(new_end, count, value);
            new_end = uninitialized_copy(pos, end_, new_end);

           alloc_.destroy(start_, end_);
           alloc_.deallocate(start_, end_of_storage_ - end_);

            start_ = new_start;
            end_ = new_end;
            end_of_storage_ = new_start + new_size;
        }
    }
}

template <class T, class Alloc>
void MyVector<T, Alloc>::resize(size_type new_size)
{
    resize(new_size, T());
}

template <class T, class Alloc>
void MyVector<T, Alloc>::resize(size_type new_size, const T& x)
{
    if(new_size < size())
        erase(begin() + new_size, end());
    else
        insert(end(), new_size - size(), x);
}

#endif