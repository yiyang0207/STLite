#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

//#include "class-bint.hpp"
//#include "class-integer.hpp"
//#include "class-matrix.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector
    {
    private:
        T **data;
        size_t _size;
        size_t capacity;

        void doubleSpace()
        {
            capacity*=2;
            T **temp;
            temp=data;
            data=new T*[capacity];
            for(size_t i=0;i<_size;++i)
            {
                data[i]=new T(*temp[i]);
            }
            for(size_t i=0;i<_size;++i)
                delete temp[i];
            delete [] temp;
        }

    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */

        class const_iterator;

        class iterator
        {
            friend class vector<T>;

        private:
            vector<T> *vec;
            size_t ptr;

        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator operator+(const int &n) const
            {
                  iterator it=*this;
                  it.ptr+=n;
                  return it;
            }

            iterator operator-(const int &n) const
            {
                iterator it=*this;
                it.ptr-=n;
                return it;
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const
            {
                if(vec!=rhs.vec) throw invalid_iterator();
                return ptr-rhs.ptr;
            }

            iterator &operator+=(const int &n)
            {
                ptr+=n;
                return *this;
            }

            iterator &operator-=(const int &n)
            {
                ptr-=n;
                return *this;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int)
            {
                iterator it=*this;
                ptr++;
                return it;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++()
            {
                ptr++;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int)
            {
                iterator it=*this;
                ptr--;
                return it;
            }

            /**
             * TODO --iter
             */
            iterator &operator--()
            {
                ptr--;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const
            {
                return *(vec->data[ptr]);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const
            {
                if(vec!=rhs.vec) return false;
                if(ptr!=rhs.ptr) return false;
                return true;
            }

            bool operator==(const const_iterator &rhs) const
            {
                if(vec!=rhs.vec) return false;
                if(ptr!=rhs.ptr) return false;
                return true;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                if(vec!=rhs.vec) return true;
                if(ptr!=rhs.ptr) return true;
                return false;
            }

            bool operator!=(const const_iterator &rhs) const
            {
                if(vec!=rhs.vec) return true;
                if(ptr!=rhs.ptr) return true;
                return false;
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator
        {
            friend class vector<T>;

        private:
            const vector<T> *vec;
            size_t ptr;

        public:
            const_iterator operator+(const int &n) const
            {
                const_iterator it=*this;
                it.it_ptr+=n;
                return it;
            }

            const_iterator operator-(const int &n) const
            {
                const_iterator it=*this;
                it.ptr-=n;
                return it;
            }

            const_iterator &operator+=(const int &n)
            {
                ptr+=n;
                return *this;
            }

            const_iterator &operator-=(const int &n)
            {
                ptr-=n;
                return *this;
            }

            int operator-(const const_iterator &rhs) const
            {
                if(vec!=rhs.vec) throw invalid_iterator();
                return ptr-rhs.ptr;
            }

            const_iterator operator++(int)
            {
                const_iterator it=*this;
                ptr++;
                return it;
            }

            const_iterator &operator++()
            {
                ptr++;
                return *this;
            }

            const_iterator operator--(int)
            {
                const_iterator it=*this;
                ptr--;
                return it;
            }

            const_iterator &operator--()
            {
                ptr--;
                return *this;
            }

            T &operator*() const
            {
                return *(vec->data[ptr]);
            }

            bool operator==(const iterator &rhs) const
            {
                if(vec!=rhs.vec) return false;
                if(ptr!=rhs.ptr) return false;
                return true;
            }

            bool operator==(const const_iterator &rhs) const
            {
                if (vec != rhs.vec) return false;
                if (ptr != rhs.ptr) return false;
                return true;
            }
            bool operator!=(const iterator &rhs) const
            {
                if(vec!=rhs.vec) return true;
                if(ptr!=rhs.ptr) return true;
                return false;
            }

            bool operator!=(const const_iterator &rhs) const
            {
                if(vec!=rhs.vec) return true;
                if(ptr!=rhs.ptr) return true;
                return false;
            }
        };

        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
        vector()
        {
            _size=0;
            capacity=10;
            data=new T*[capacity];
        }

        vector(const vector &other)
        {
            _size=other._size;
            capacity=other.capacity;
            data=new T*[capacity];
            for(size_t i=0;i<_size;++i)
            {
                data[i]=new T(*(other.data[i]));
            }
        }

        /**
         * TODO Destructor
         */
        ~vector()
        {
            for(size_t i=0;i<_size;++i)
                delete data[i];
            delete [] data;
            _size=0;
        }

        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other)
        {
            if(this==&other) return *this;

            //first forgotten
            for(size_t i=0;i<_size;++i)
                delete data[i];
            delete [] data;

            _size=other._size;
            capacity=other.capacity;
            data=new T*[capacity];
            for(size_t i=0;i<_size;++i)
            {
                data[i]=new T(*(other.data[i]));
            }
            return *this;
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if ptr is not in [0, size)
         */
        T &at(const size_t &pos)
        {
            if(pos<0||pos>=_size) throw index_out_of_bound();
            return *data[pos];
        }

        const T &at(const size_t &pos) const
        {
            if(pos<0||pos>=_size) throw index_out_of_bound();
            return *data[pos];
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos)
        {
            if(pos<0||pos>=_size) throw index_out_of_bound();
            return *data[pos];
        }

        const T &operator[](const size_t &pos) const
        {
            if(pos<0||pos>=_size) throw index_out_of_bound();
            return *data[pos];
        }

        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T &front() const
        {
            if(size()==0) throw container_is_empty();
            return *data[0];
        }

        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T &back() const
        {
            if(size()==0) throw container_is_empty();
            return *data[_size-1];
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin()
        {
            iterator it;
            it.vec=this;
            it.ptr=0;
            return it;
        }

        const_iterator cbegin() const
        {
            const_iterator it;
            it.vec=this;
            it.ptr=0;
            return it;
        }

        /**
         * returns an iterator to the end.
         */
        iterator end()
        {
            iterator it;
            it.vec=this;
            it.ptr=_size;
            return it;
        }

        const_iterator cend() const
        {
            const_iterator it;
            it.vec=this;
            it.ptr=_size;
            return it;
        }

        /**
         * checks whether the container is empty
         */
        bool empty() const
        {
            return begin()==end();
        }

        /**
         * returns the number of elements
         */
        size_t size() const
        {
            return _size;
        }

        /**
         * clears the contents
         */
        void clear()
        {
            for(int i=0;i<_size;++i)
                delete data[i];
            delete [] data;
            _size=0;
            capacity=10;
            data=new T*[capacity];
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value)
        {
            if(_size==capacity) doubleSpace();
            for(size_t i=_size;i>pos.ptr;--i)
                data[i]=data[i-1];
            data[pos.ptr]=new T(value);
            _size++;
            return pos;
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value)
        {
            if(ind>_size) throw index_out_of_bound();
            if(_size==capacity) doubleSpace();
            for(size_t i=_size;i>ind;--i)
                data[i]=data[i-1];
            data[ind]=new T(value);
            _size++;
            iterator it;
            it=begin()+ind;
            return it;
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos)
        {
            delete data[pos.ptr];
            _size--;
            for(size_t i=pos.ptr;i<_size;++i)
                data[i]=data[i+1];
            return pos;
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind)
        {
            if(ind>_size) throw index_out_of_bound();
            delete data[ind];
            _size--;
            for(size_t i=ind;i<_size;++i)
                data[i]=data[i+1];
            iterator it;
            it=begin()+ind;
            return it;
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value)
        {
            if(_size==capacity) doubleSpace();
            data[_size]=new T(value);
            _size++;
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back()
        {
            if(size()==0) throw container_is_empty();
            delete data[_size-1];
            _size--;
        }
    };


}

#endif
