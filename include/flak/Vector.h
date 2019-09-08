/*

Copyright 2019 flak authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#ifndef ALG_VECTOR_H
#define ALG_VECTOR_H

#include <algorithm>
#include <iterator>
#include <memory>

namespace flak {
template<class T, class Alloc = std::allocator<T>>
class Vector {
public:
    typedef T value_type;

    // iterator is a pointer
    typedef value_type *pointer;
    typedef value_type *iterator;

    typedef value_type &reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    typedef typename Alloc::template rebind<T>::other ValueAlloc;
    typedef std::allocator_traits<Alloc> AllocTraits;

    iterator start_;
    iterator finish_;
    iterator end_of_storage_;

    ValueAlloc valloc;

    pointer allocate(size_type n) {
        if (n == 0) {
            return pointer();
        }
        return AllocTraits::allocate(valloc, n);
    }

    void deallocate(pointer p, size_type n) {
        AllocTraits::deallocate(valloc, p, n);
    }

    void deallocate_all() {
        if (start_) {
            deallocate(start_, end_of_storage_ - start_);
        }
    }

    void createStorage(size_type n) {
        start_ = allocate(n);
        finish_ = start_;
        end_of_storage_ = start_ + n;
    }

    void fillInitialize(size_type n, const T &value) {
        // uninitialized_fill_n() will call fill_n (without new) probably,
        // so you need to allocate space manually.
        createStorage(n);
        finish_ = std::uninitialized_fill_n(start_, n, value);
        end_of_storage_ = finish_;
    }

public:
    iterator begin() const { return start_; }

    iterator end() const { return finish_; }

    size_type size() const { return size_type(end() - begin()); }

    size_type capacity() const {
        return size_type(end_of_storage_ - begin());
    }

    bool empty() const { return begin() == end(); }

    void clear() {
        erase(begin(), end());
    }

    Vector() : start_(0), finish_(0), end_of_storage_(0) {}

    Vector(size_type n, const T &value) : Vector() { fillInitialize(n, value); }

    Vector(int n, const T &value) : Vector() { fillInitialize(n, value); }

    Vector(long n, const T &value) : Vector() { fillInitialize(n, value); }

    template<class InputIterator>
    Vector(InputIterator first, InputIterator last) : Vector() {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }

    template<class Alloc2>
    explicit Vector(Vector<T, Alloc2> &x) {
        start_ = allocate(x.size());
        finish_ = std::uninitialized_copy(x.begin(), x.end(), start_);
        end_of_storage_ = start_ + x.size();
    }

    Vector(std::initializer_list<T> ils) : Vector() {
        for (auto x : ils) {
            push_back(x);
        }
    }

    explicit Vector(size_type n) { fillInitialize(n, T()); }

    ~Vector() {
        std::_Destroy(start_, finish_);
        deallocate_all();
    }

    reference front() { return *begin(); }

    reference back() { return *(end() - 1); }

    void push_back(const T &x) {
        if (finish_ != end_of_storage_) {
            std::_Construct(finish_, x);
            ++finish_;
        } else {
            _insertAux(end(), x);
        }
    }

    void pop_back() {
        --finish_;
        std::_Destroy(finish_);
    }

    // erase the element in [first, last)
    iterator erase(iterator first, iterator last) {
        // [last, finish) override to [first, finish - last)
        // return iterator that points to first + (finish - last)
        iterator i = std::copy(last, finish_, first);
        // first + (finish - last) and back of it are not needed.
        std::_Destroy(i, finish_);
        finish_ = finish_ - (last - first);
        return first;
    }

    // erase the element on [pos]
    iterator erase(iterator pos) {
        if (pos + 1 != end()) {
            std::copy(pos + 1, finish_, pos);
        }
        --finish_;
        std::_Destroy(finish_);
        return pos;
    }

    reference operator[](size_type n) {
        return *(begin() + n);
    }

    template<class U, class Alloc2>
    bool operator==(const Vector<U, Alloc2> &x) {
        return size() == x.size() && std::equal(begin(), end(), x.begin());
    }

    template<class U, class Alloc2>
    bool operator<(const Vector<U, Alloc2> &x) {
        return std::lexicographical_compare(begin(), end(), x.begin(), x.end());
    }

    Vector<T, Alloc> &operator=(const Vector<T, Alloc> &x) {
        if (&x == this) {
            return *this;
        }
        const size_type xlen = x.size();
        if (xlen > capacity()) {
            iterator newStart = allocate(xlen);
            iterator newFinish = std::copy(x.begin(), x.end(), newStart);
            std::_Destroy(start_, finish_);
            deallocate_all();

            start_ = newStart;
            finish_ = newFinish;
            end_of_storage_ = newStart + xlen;
        } else if (size() >= xlen) {
            iterator newFinish = std::copy(x.begin(), x.end(), start_);
            std::_Destroy(newFinish, finish_);
            finish_ = newFinish;
        } else {
            // [start finish) have been allocated, we can immedately call copy
            std::copy(x.begin(), x.begin() + size(), start_);
            // [finish, x.end()) have not been allocated, call uninitialized_copy.
            // uninitialized_copy will help us to allocate space.
            finish_ = std::uninitialized_copy(x.begin() + size(), x.end(), finish_);
        }
        return *this;
    }

public:
    // reserve a exact size
    void reserve(size_type n) {
        if (capacity() < n) {
            const size_type oldSize = size();
            iterator newStart = allocate(n);
            iterator newFinish = std::copy(start_, finish_, newStart);
            std::_Destroy(start_, finish_);
            deallocate_all();
            start_ = newStart;
            finish_ = start_ + oldSize;
            end_of_storage_ = start_ + n;
        }
    }

private:
    // Insert [x] to [pos].
    // Original [pos, finish-1] move back a position and place a copy of x to [pos]
    // If over the capacity, allocate a new len space,
    //  we firstly copy [start, pos) to new space, then place a copy to the finish of new space,
    //  then copy [pos, finish) to new space. At last, free the original space.
    void _insertAux(iterator pos, const value_type &x) {
        if (finish_ != end_of_storage_) {
            std::_Construct(finish_, *(finish_ - 1));
            ++finish_;
            T xcopy = x;
            // [pos, finish - 2] to [pos + 1, finish - 1] move back a position
            std::copy_backward(pos, finish_ - 2, finish_ - 1);
            *pos = xcopy;
        } else {
            const size_type oldSize = size();
            // If old size is 0, new size is 1
            const size_type newLen = oldSize != 0 ? 2 * oldSize : 1;

            // allocate a new len space
            iterator newStart = allocate(newLen);
            iterator newFinish = newStart;
            try {
                newFinish = std::uninitialized_copy(start_, pos, newStart);
                std::_Construct(newFinish, x);
                newFinish = std::uninitialized_copy(pos, finish_, newFinish + 1);
            } catch (...) {
                std::_Destroy(newStart, newFinish);
                deallocate(newStart, newLen);
                throw;
            }

            // destroy original vector
            std::_Destroy(begin(), end());
            deallocate_all();

            start_ = newStart;
            finish_ = newFinish;
            end_of_storage_ = newStart + newLen;
        }
    }
};

}
#endif //ALG_VECTOR_H
