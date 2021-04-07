#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu
{
    const int SIZE = 512;

    template<class T>
    class deque
    {
    private:
        struct node
        {
            T **data;
            size_t size;
            node *pre;
            node *nxt;

            node() : size(0), pre(nullptr), nxt(nullptr)
            {
                data = new T *[SIZE + 1];
            }

            ~node()
            {
                clear();
            }

            node(const node &o)
            {
                size = o.size;
                data = new T *[SIZE + 1];
                for (size_t i = 0; i < size; ++i)
                    data[i] = new T(*o.data[i]);
            }

            node &operator=(const node &o)
            {
                if (this == &o) return *this;

                clear();

                size = o.size;
                data = new T *[SIZE + 1];
                for (size_t i = 0; i < size; ++i)
                    data[i] = new T(*o.data[i]);
                return *this;
            }

            T &operator[](const int pos) const
            {
                if (pos < 0 || pos >= size) throw index_out_of_bound();
                return *data[pos];
            }

            void insert(const int pos, const T &x)
            {
                if (pos < 0 || pos > size) throw index_out_of_bound();
                for (int i = size; i > pos; --i)
                    data[i] = data[i - 1];
                data[pos] = new T(x);
                size++;
            }

            void erase(int pos)
            {
                if (pos < 0 || pos >= size) throw index_out_of_bound();
                delete data[pos];
                for (int i = pos; i < size - 1; ++i)
                    data[i] = data[i + 1];
                size--;
            }

            void clear()
            {
                for (size_t i = 0; i < size; ++i)
                    delete data[i];
                delete[]data;
                size = 0;
            }
        };

        size_t len;
        node *head;
        node *tail;

    public:
        class const_iterator;

        class iterator
        {
            friend class deque<T>;

        private:
            deque *dq;
            node *Node;
            int ptr;  //never use size_t

        public:
            /**
             * return a new iterator which pointer n-next elements
             *   if there are not enough elements, iterator becomes invalid
             * as well as operator-
             */
            iterator operator+(const int &n) const
            {
                if (n == 0) return *this;

                iterator it = *this;
                if (n > 0)
                {
                    int p = n;
                    while (p >= it.Node->size - it.ptr && it.Node->nxt != it.dq->tail)
                    {
                        p -= it.Node->size - it.ptr;
                        it.Node = it.Node->nxt;
                        it.ptr = 0;
                    }
                    if (it.Node->nxt == nullptr) throw index_out_of_bound();
                    it.ptr += p;
                } else
                {
                    int p = -n;
                    while (p > it.ptr && it.Node->pre != nullptr)
                    {
                        p -= it.ptr + 1;
                        it.Node = it.Node->pre;
                        it.ptr = it.Node->size - 1;
                    }
                    if (it.Node->pre == nullptr && p > it.ptr) throw index_out_of_bound();
                    it.ptr -= p;
                }
                return it;
            }

            iterator operator-(const int &n) const
            {
                iterator it = operator+(-n);
                return it;
            }

            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const
            {
                if (dq != rhs.dq) throw invalid_iterator();

                if (Node == rhs.Node) return ptr - rhs.ptr;

                node *p = rhs.Node->nxt;
                int ans = rhs.Node->size - rhs.ptr;
                while (p != Node && p != nullptr)
                {
                    ans += p->size;
                    p = p->nxt;
                }
                if (p != nullptr) return ans + ptr;

                p = Node->nxt;
                ans = Node->size - ptr;
                while (p != rhs.Node && p != nullptr)
                {
                    ans += p->size;
                    p = p->nxt;
                }
                if (p != nullptr) return -(ans + rhs.ptr);
                else throw invalid_iterator();
            }

            iterator &operator+=(const int &n)
            {
                (*this) = (*this) + n;
                return *this;
            }

            iterator &operator-=(const int &n)
            {
                (*this) = (*this) - n;
                return *this;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int)
            {
                iterator it = *this;
                if (ptr < Node->size - 1) ptr++;
                else
                {
                    if (Node->nxt == nullptr) throw invalid_iterator();
                    Node = Node->nxt;
                    ptr = 0;
                }
                return it;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++()
            {
                if (ptr < Node->size - 1) ptr++;
                else
                {
                    if (Node->nxt == nullptr) throw invalid_iterator();
                    Node = Node->nxt;
                    ptr = 0;
                }
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int)
            {
                iterator it = *this;
                if (ptr > 0) ptr--;
                else
                {
                    if (Node->pre == nullptr) throw invalid_iterator();
                    Node = Node->pre;
                    ptr = Node->size - 1;
                }
                return it;
            }

            /**
             * TODO --iter
             */
            iterator &operator--()
            {
                if (ptr > 0) ptr--;
                else
                {
                    if (Node->pre == nullptr) throw invalid_iterator();
                    Node = Node->pre;
                    ptr = Node->size - 1;
                }
                return *this;
            }

            /**
             * TODO *it
             * 		throw if iterator is invalid
             */
            T &operator*() const
            {
                if (ptr < 0 || ptr >= Node->size) throw index_out_of_bound();
                return *(Node->data[ptr]);
            }

            /**
             * TODO it->field
             * 		throw if iterator is invalid
             */
            T *operator->() const noexcept
            {
                return Node->data[ptr];
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const
            {
                if (dq != rhs.dq) return false;
                if (Node == rhs.Node && ptr == rhs.ptr) return true;
                if (Node->nxt == nullptr && rhs.Node->nxt == nullptr) return true;
                if (Node->nxt == rhs.Node && ptr == Node->size && rhs.Node->nxt == nullptr) return true;
                return false;
            }

            bool operator==(const const_iterator &rhs) const
            {
                if (dq != rhs.dq) return false;
                if (Node == rhs.Node && ptr == rhs.ptr) return true;
                if (Node->nxt == nullptr && rhs.Node->nxt == nullptr) return true;
                if (Node->nxt == rhs.Node && ptr == Node->size && rhs.Node->nxt == nullptr) return true;
                return false;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const
            {
                return !(*this == rhs);
            }
        };

        class const_iterator
        {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
            friend class deque<T>;

        private:
            const deque *dq;
            node *Node;
            int ptr;

        public:
            const_iterator() : dq(nullptr), Node(nullptr), ptr(-1)
            {}

            const_iterator(const const_iterator &other)
                    : dq(other.dq), Node(other.Node), ptr(other.ptr)
            {}

            const_iterator(const iterator &other)
                    : dq(other.dq), Node(other.Node), ptr(other.ptr)
            {}


            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
            const_iterator operator+(const int &n) const
            {
                if (n == 0) return *this;

                const_iterator it = *this;
                if (n > 0)
                {
                    int p = n;
                    while (p >= it.Node->size - it.ptr && it.Node->nxt != it.dq->tail)
                    {
                        p -= it.Node->size - it.ptr;
                        it.Node = it.Node->nxt;
                        it.ptr = 0;
                    }
                    if (it.Node->nxt == nullptr) throw index_out_of_bound();
                    it.ptr += p;
                } else
                {
                    int p = -n;
                    while (p > it.ptr && it.Node->pre != nullptr)
                    {
                        p -= it.ptr + 1;
                        it.Node = it.Node->pre;
                        it.ptr = it.Node->size - 1;
                    }
                    if (it.Node->pre == nullptr && p > it.ptr) throw index_out_of_bound();
                    it.ptr -= p;
                }
                return it;
            }

            const_iterator operator-(const int &n) const
            {
                const_iterator it = operator+(-n);
                return it;
            }

            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const
            {
                if (dq != rhs.dq) throw invalid_iterator();

                if (Node == rhs.Node) return ptr - rhs.ptr;

                node *p = rhs.Node->nxt;
                int ans = rhs.Node->size - rhs.ptr;
                while (p != Node && p != nullptr)
                {
                    ans += p->size;
                    p = p->nxt;
                }
                if (p != nullptr) return ans + ptr;

                p = Node->nxt;
                ans = Node->size - ptr;
                while (p != rhs.Node && p != nullptr)
                {
                    ans += p->size;
                    p = p->nxt;
                }
                if (p != nullptr) return -(ans + rhs.ptr);
                else throw invalid_iterator();
            }

            const_iterator &operator+=(const int &n)
            {
                (*this) = (*this) + n;
                return *this;
            }

            const_iterator &operator-=(const int &n)
            {
                (*this) = (*this) - n;
                return *this;
            }

            /**
             * TODO iter++
             */
            const_iterator operator++(int)
            {
                const_iterator it = *this;
                if (ptr < Node->size - 1) ptr++;
                else
                {
                    if (Node->nxt == nullptr) throw invalid_iterator();
                    Node = Node->nxt;
                    ptr = 0;
                }
                return it;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++()
            {
                if (ptr < Node->size - 1) ptr++;
                else
                {
                    if (Node->nxt == nullptr) throw invalid_iterator();
                    Node = Node->nxt;
                    ptr = 0;
                }
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int)
            {
                const_iterator it = *this;
                if (ptr > 0) ptr--;
                else
                {
                    if (Node->pre == nullptr) throw invalid_iterator();
                    Node = Node->pre;
                    ptr = Node->size - 1;
                }
                return it;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--()
            {
                if (ptr > 0) ptr--;
                else
                {
                    if (Node->pre == nullptr) throw invalid_iterator();
                    Node = Node->pre;
                    ptr = Node->size - 1;
                }
                return *this;
            }

            /**
             * TODO *it
             * 		throw if iterator is invalid
             */
            T &operator*() const
            {
                if (ptr < 0 || ptr >= Node->size) throw index_out_of_bound();
                return *(Node->data[ptr]);
            }

            /**
             * TODO it->field
             * 		throw if iterator is invalid
             */
            T *operator->() const noexcept
            {
                if (ptr < 0 || ptr >= Node->size) throw index_out_of_bound();
                return Node->data[ptr];
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const
            {
                if (dq != rhs.dq) return false;
                if (Node == rhs.Node && ptr == rhs.ptr) return true;
                if (Node->nxt == nullptr && rhs.Node->nxt == nullptr) return true;
                if (Node->nxt == rhs.Node && ptr == Node->size && rhs.Node->nxt == nullptr) return true;
                return false;
            }

            bool operator==(const const_iterator &rhs) const
            {
                if (dq != rhs.dq) return false;
                if (Node == rhs.Node && ptr == rhs.ptr) return true;
                if (Node->nxt == nullptr && rhs.Node->nxt == nullptr) return true;
                if (Node->nxt == rhs.Node && ptr == Node->size && rhs.Node->nxt == nullptr) return true;
                return false;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const
            {
                return !(*this == rhs);
            }
        };

        /**
         * TODO Constructors
         */
        deque()
        {
            len = 0;
            head = new node;
            tail = new node;
            head->nxt = tail;
            tail->pre = head;
        }

        deque(const deque &other)
        {
            len = other.len;
            head = new node(*(other.head));
            tail = new node();
            node *p = head, *q = other.head->nxt;
            while (q != other.tail)
            {
                p->nxt = new node(*q);
                (p->nxt)->pre = p;
                p = p->nxt;
                q = q->nxt;
            }
            p->nxt = tail;
            tail->pre = p;
        }

        /**
         * TODO Deconstructor
         */
        ~deque()
        {
            clear();
            delete head;
            delete tail;
        }

        /**
         * TODO assignment operator
         */
        deque &operator=(const deque &other)
        {
            if (this == &other) return *this;

            clear();

            len = other.len;
            delete head;
            delete tail;
            head = new node(*(other.head));
            tail = new node();
            node *p = head, *q = other.head->nxt;
            while (q != other.tail)
            {
                p->nxt = new node(*q);
                (p->nxt)->pre = p;
                p = p->nxt;
                q = q->nxt;
            }
            p->nxt = tail;
            tail->pre = p;
            return *this;
        }

        /**
         * access specified element with bounds checking
         * throw index_out_of_bound if out of bound.
         */
        T &at(const size_t &pos)
        {
            node *p = head;
            int cnt = pos;
            while (cnt >= p->size && p != tail)
            {
                cnt -= p->size;
                p = p->nxt;
            }
            if (p == tail) throw index_out_of_bound();
            return (*p)[cnt];
        }

        const T &at(const size_t &pos) const
        {
            node *p = head;
            int cnt = pos;
            while (cnt >= p->size && p != tail)
            {
                cnt -= p->size;
                p = p->nxt;
            }
            if (p == tail) throw index_out_of_bound();
            return (*p)[cnt];
        }

        T &operator[](const size_t &pos)
        {
            return at(pos);
        }

        const T &operator[](const size_t &pos) const
        {
            return at(pos);
        }

        /**
         * access the first element
         * throw container_is_empty when the container is empty.
         */
        const T &front() const
        {
            if (empty()) throw container_is_empty();
            return (*head)[0];
        }

        /**
         * access the last element
         * throw container_is_empty when the container is empty.
         */
        const T &back() const
        {
            if (empty()) throw container_is_empty();
            return (*(tail->pre))[(tail->pre)->size - 1];
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin()
        {
            if (empty()) return end();
            iterator it;
            it.dq = this;
            it.Node = head;
            it.ptr = 0;
            return it;
        }

        const_iterator cbegin() const
        {
            if (empty()) return cend();
            const_iterator it;
            it.dq = this;
            it.Node = head;
            it.ptr = 0;
            return it;
        }

        /**
         * returns an iterator to the end.
         */
        iterator end()
        {
            iterator it;
            it.dq = this;
            it.Node = tail;
            it.ptr = 0;
            return it;
        }

        const_iterator cend() const
        {
            const_iterator it;
            it.dq = this;
            it.Node = tail;
            it.ptr = 0;
            return it;
        }

        /**
         * checks whether the container is empty.
         */
        bool empty() const
        {
            return len == 0;
        }

        /**
         * returns the number of elements
         */
        size_t size() const
        {
            return len;
        }

        /**
         * clears the contents
         */
        void clear()
        {
            node *p, *q = head->nxt;
            while (q->nxt != nullptr)
            {
                p = q->nxt;
                delete q;
                q = p;
            }
            len = 0;
            delete head;
            head = new node;
            head->nxt = tail;
            tail->pre = head;
        }

    private:
        void merge(node *n)
        {
            while (n->nxt != tail && n->size + n->nxt->size <= SIZE)
            {
                while (n->nxt != tail && n->size == 0)
                {
                    node *p = n;
                    n = n->nxt;
                    delete p;
                }
                node *p = n->nxt;
                for (int i = n->size; i < n->size + p->size; ++i)
                    n->data[i] = p->data[i - n->size];
                n->size += p->size;
                p->size = 0;
                n->nxt = p->nxt;
                n->nxt->pre = n;
                delete p;
            }
            if (n->nxt != tail && n->size < SIZE / 2)
            {
                node *p = n->nxt;
                int tmp = std::min(p->size, SIZE / 2 - n->size);
                for (int i = 0; i < tmp; ++i)
                {
                    n->data[n->size + i] = n->nxt->data[i];
                }
                n->size += tmp;
                int tmp2 = p->size - tmp;
                if (tmp2 == 0)
                {
                    n->nxt = p->nxt;
                    n->nxt->pre = n;
                    delete p;
                } else
                {
                    for (int i = 0; i < tmp2; ++i)
                        p->data[i] = p->data[i + tmp];
                    p->size -= tmp;
                }
            }
        }

        void split(node *n)
        {
            if (n == tail || n->size < SIZE) return;
            node *tmp = new node;
            (n->nxt)->pre = tmp;
            tmp->nxt = n->nxt;
            tmp->pre = n;
            n->nxt = tmp;
            for (size_t i = n->size / 2; i < n->size; ++i)
                tmp->data[i - n->size / 2] = n->data[i];
            tmp->size = n->size - n->size / 2;
            n->size /= 2;
        }

    public:
        /**
         * inserts elements at the specified locate on in the container.
         * inserts value before pos
         * returns an iterator pointing to the inserted value
         *     throw if the iterator is invalid or it point to a wrong place.
         */
        iterator insert(iterator pos, const T &value)
        {
            if (pos.dq != this) throw invalid_iterator();
            if (pos.ptr == 0 && pos.Node->pre != nullptr)
            {
                pos.Node = pos.Node->pre;
                pos.ptr = pos.Node->size;
            }

            if (pos.Node != tail)
            {
                if (pos.Node->size < SIZE / 2) merge(pos.Node);
                else if (pos.Node->size > SIZE) split(pos.Node);
            }

            while (pos.ptr > pos.Node->size && pos.Node != tail)
            {
                pos.ptr -= pos.Node->size;
                pos.Node = pos.Node->nxt;
            }
            if (pos.Node == tail) throw index_out_of_bound();
            pos.Node->insert(pos.ptr, value);
            len++;
            return pos;
        }

        /**
         * removes specified element at pos.
         * removes the element at pos.
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid or it points to a wrong place.
         */
        iterator erase(iterator pos)
        {
            if (empty()) throw container_is_empty();
            if (pos.dq != this || pos.Node == tail) throw invalid_iterator();
            while (pos.ptr < 0)
            {
                pos.Node = pos.Node->pre;
                pos.ptr += pos.Node->size;
            }

            if (pos.Node != tail)
            {
                if (pos.Node->size < SIZE / 2) merge(pos.Node);
                else if (pos.Node->size > SIZE) split(pos.Node);
            }

            while (pos.ptr >= pos.Node->size && pos.Node != tail)
            {
                pos.ptr -= pos.Node->size;
                pos.Node = pos.Node->nxt;
            }
            if (pos.Node == tail) throw index_out_of_bound();
            pos.Node->erase(pos.ptr);
            if (pos.ptr == pos.Node->size)
            {
                pos.ptr = 0;
                pos.Node = pos.Node->nxt;
            }
            len--;
            return pos;
        }

        /**
         * adds an element to the end
         */
        void push_back(const T &value)
        {
            iterator it;
            it.dq = this;
            it.Node = tail->pre;
            it.ptr = it.Node->size;
            insert(it, value);
        }

        /**
         * removes the last element
         *     throw when the container is empty.
         */
        void pop_back()
        {
            if (empty()) throw container_is_empty();
            iterator it;
            it.dq = this;
            it.Node = tail->pre;
            it.ptr = it.Node->size - 1;
            erase(it);
        }

        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value)
        {
            iterator it;
            it.dq = this;
            it.Node = head;
            it.ptr = 0;
            insert(it, value);
        }

        /**
         * removes the first element.
         *     throw when the container is empty.
         */
        void pop_front()
        {
            if (empty()) throw container_is_empty();
            iterator it;
            it.dq = this;
            it.Node = head;
            it.ptr = 0;
            erase(it);
        }
    };

}

#endif
