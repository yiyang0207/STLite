/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu
{

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    class map
    {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        enum Color
        {
            RED, BLACK
        };

    private:
        struct Node
        {
            Color color;
            value_type value;
            Node *parent, *left, *right;

            Node() : color(RED), parent(nullptr), left(nullptr), right(nullptr)
            {}

            Node(const value_type &v, Color c = RED) : value(v), color(c), parent(nullptr), left(nullptr), right(nullptr)
            {}
        };

        Node *root;
        size_t num;
        Compare cmp;

    public:
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class const_iterator;

        class iterator
        {
            friend class map;

        private:
            map *mp;
            Node *node;

        public:
            iterator() : mp(nullptr), node(nullptr)
            {}

            iterator(const iterator &other) : mp(other.mp), node(other.node)
            {}

            iterator(map *_mp, Node *_node) : mp(_mp), node(_node)
            {}

            /**
             * TODO iter++
             */
            iterator operator++(int)
            {
                iterator it = *this;
                ++(*this);
                return it;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++()
            {
                if (node == nullptr) throw invalid_iterator();
                if (node->right != nullptr)
                {
                    node = node->right;
                    while (node->left != nullptr) node = node->left;
                } else
                {
                    while (node != nullptr && node->parent != nullptr && !isLeftChild(node)) node = node->parent;
                    node = node->parent;
                }
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int)
            {
                iterator it = *this;
                --(*this);
                return it;
            }

            /**
             * TODO --iter
             */
            iterator &operator--()
            {
                if (*this == mp->begin()) throw invalid_iterator();
                if (node == nullptr)
                {
                    if (mp->empty()) throw invalid_iterator();
                    node = mp->root;
                    while (node->right != nullptr) node = node->right;
                } else if (node->left != nullptr)
                {
                    node = node->left;
                    while (node->right != nullptr) node = node->right;
                } else
                {
                    while (node != nullptr && node->parent != nullptr && isLeftChild(node)) node = node->parent;
                    node = node->parent;
                }
                return *this;
            }

            /**
             * an operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const
            {
                return node->value;
            }

            bool operator==(const iterator &rhs) const
            {
                return mp == rhs.mp && node == rhs.node;
            }

            bool operator==(const const_iterator &rhs) const
            {
                return mp == rhs.mp && node == rhs.node;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                return mp != rhs.mp || node != rhs.node;
            }

            bool operator!=(const const_iterator &rhs) const
            {
                return mp != rhs.mp || node != rhs.node;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept
            {
                return &(node->value);
            }
        };

        class const_iterator
        {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
            friend class map;

        private:
            const map *mp;
            Node *node;

        public:
            const_iterator() : mp(nullptr), node(nullptr)
            {}

            const_iterator(const const_iterator &other) : mp(other.mp), node(other.node)
            {}

            const_iterator(const iterator &other) : mp(other.mp), node(other.node)
            {}

            const_iterator(const map *_mp, Node *_node) : mp(_mp), node(_node)
            {}

            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
            /**
             * TODO iter++
             */
            const_iterator operator++(int)
            {
                const_iterator it = *this;
                ++(*this);
                return it;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++()
            {
                if (node == nullptr) throw invalid_iterator();
                if (node->right != nullptr)
                {
                    node = node->right;
                    while (node->left != nullptr) node = node->left;
                } else
                {
                    while (node != nullptr && node->parent != nullptr && !isLeftChild(node)) node = node->parent;
                    node = node->parent;
                }
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int)
            {
                const_iterator it = *this;
                --(*this);
                return it;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--()
            {
                if (*this == mp->cbegin()) throw invalid_iterator();
                if (node == nullptr)
                {
                    if (mp->empty()) throw invalid_iterator();
                    node = mp->root;
                    while (node->right != nullptr) node = node->right;
                } else if (node->left != nullptr)
                {
                    node = node->left;
                    while (node->right != nullptr) node = node->right;
                } else
                {
                    while (node != nullptr && node->parent != nullptr && isLeftChild(node)) node = node->parent;
                    node = node->parent;
                }
                return *this;
            }

            /**
             * an operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const
            {
                return node->value;
            }

            bool operator==(const iterator &rhs) const
            {
                return mp == rhs.mp && node == rhs.node;
            }

            bool operator==(const const_iterator &rhs) const
            {
                return mp == rhs.mp && node == rhs.node;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const
            {
                return mp != rhs.mp || node != rhs.node;
            }

            bool operator!=(const const_iterator &rhs) const
            {
                return mp != rhs.mp || node != rhs.node;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept
            {
                return &(node->value);
            }
        };

    private:
        Node *create(Node *o, Node *p = nullptr)
        {
            if (o == nullptr) return nullptr;
            Node *t = new Node(o->value, o->color);
            t->parent = p;
            t->left = create(o->left, t);
            t->right = create(o->right, t);
            return t;
        }

        void _clear(Node *&t)
        {
            if (t == nullptr) return;
            _clear(t->left);
            _clear(t->right);
            delete t;
        }

    public:
        /**
         * TODO two constructors
         */
        map() : root(nullptr), num(0)
        {}

        map(const map &other) : num(other.num)
        {
            root = create(other.root);
        }

        /**
         * TODO assignment operator
         */
        map &operator=(const map &other)
        {
            if (this == &other) return *this;
            _clear(root);
            num = other.num;
            root = create(other.root);
            return *this;
        }

        /**
         * TODO Destructors
         */
        ~map()
        {
            clear();
        }

        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T &at(const Key &key)
        {
            iterator it = find(key);
            if (it == end()) throw index_out_of_bound();
            return it->second;
        }

        const T &at(const Key &key) const
        {
            const_iterator it = find(key);
            if (it == cend()) throw index_out_of_bound();
            return it->second;
        }

        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key)
        {
            iterator it = find(key);
            if (it == end()) return insert(value_type(key, T())).first.node->value.second;
            else return it->second;
        }

        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const
        {
            return at(key);
        }

        /**
         * return a iterator to the beginning
         */
        iterator begin()
        {
            if (empty()) return end();
            Node *t = root;
            while (t->left != nullptr) t = t->left;
            return iterator(this, t);
        }

        const_iterator cbegin() const
        {
            if (empty()) return cend();
            Node *t = root;
            while (t->left != nullptr) t = t->left;
            return const_iterator(this, t);
        }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end()
        {
            return iterator(this, nullptr);
        }

        const_iterator cend() const
        {
            return const_iterator(this, nullptr);
        }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const
        {
            return num == 0;
        }

        /**
         * returns the number of elements.
         */
        size_t size() const
        {
            return num;
        }

        /**
         * clears the contents
         */
        void clear()
        {
            _clear(root);
            root = nullptr;
            num = 0;
        }

    private:
        static bool isLeftChild(Node *t)
        {
            return t->parent != nullptr && t->parent->left == t;
        }

        Node *getUncle(Node *t)
        {
            if (t->parent == nullptr) return nullptr;
            Node *p = t->parent;
            if (p->parent == nullptr) return nullptr;
            if (isLeftChild(p)) return p->parent->right;
            else return p->parent->left;
        }

        Node *getSibling(Node *t)
        {
            if (t == nullptr) return nullptr;
            Node *p = t->parent;
            if (p == nullptr) return nullptr;
            if (isLeftChild(p)) return p->right;
            else return p->left;
        }

        Color getColor(Node *t)
        {
            if (t == nullptr) return BLACK;
            else return t->color;
        }

        void leftRotate(Node *t)
        {
            if (t == nullptr || t->right == nullptr) return;
            Node *r = t->right;
            t->right = r->left;
            if (r->left != nullptr) r->left->parent = t;
            r->parent = t->parent;
            if (t->parent == nullptr) root = r;
            else if (isLeftChild(t)) t->parent->left = r;
            else t->parent->right = r;
            r->left = t;
            t->parent = r;
        }

        void rightRotate(Node *t)
        {
            if (t == nullptr || t->left == nullptr) return;
            Node *l = t->left;
            t->left = l->right;
            if (l->right != nullptr) l->right->parent = t;
            l->parent = t->parent;
            if (t->parent == nullptr) root = l;
            else if (isLeftChild(t)) t->parent->left = l;
            else t->parent->right = l;
            l->right = t;
            t->parent = l;
        }

        Node *_find(const Key &key, Node *t) const
        {
            if (t == nullptr) return nullptr;
            if (cmp(key, t->value.first)) return _find(key, t->left);
            else if (cmp(t->value.first, key)) return _find(key, t->right);
            else return t;
        }

        Node *_insert(const value_type &value)
        {
            num++;
            Node *newNode = new Node(value);
            Node *t = root, *p = nullptr;
            while (t != nullptr)
            {
                p = t;
                if (cmp(value.first, t->value.first)) t = t->left;
                else t = t->right;
            }
            newNode->parent = p;
            if (p == nullptr) root = newNode;
            else if (cmp(value.first, p->value.first)) p->left = newNode;
            else p->right = newNode;
            insertRebalance(newNode);
            return newNode;
        }

        void insertRebalance(Node *t)
        {
            if (t == root)
            {
                t->color = BLACK;
                return;
            }
            if (getColor(t->parent) == BLACK) return;
            Node *p = t->parent, *g = p->parent, *u = getUncle(t);
            if (getColor(u) == RED)
            {
                p->color = u->color = BLACK;
                if (g != nullptr) g->color = RED;
                insertRebalance(g);
            } else if (isLeftChild(p))
            {
                if (isLeftChild(t))
                {
                    p->color = BLACK;
                    if (g != nullptr) g->color = RED;
                    rightRotate(g);
                } else
                {
                    leftRotate(p);
                    insertRebalance(p);
                }
            } else
            {
                if (!isLeftChild(t))
                {
                    p->color = BLACK;
                    if (g != nullptr) g->color = RED;
                    leftRotate(g);
                } else
                {
                    rightRotate(p);
                    insertRebalance(p);
                }
            }
        }

        void _erase(Node *t)
        {
            num--;
            Node *replace, *p = t->parent, *c;
            Color col;
            if (t->left == nullptr || t->right == nullptr)
            {
                c = t->left != nullptr ? t->left : t->right;
                if (c != nullptr) c->parent = p;
                if (p == nullptr) root = c;
                else
                {
                    if (isLeftChild(t)) p->left = c;
                    else p->right = c;
                }
                col = getColor(t);
                replace = c;
            } else
            {
                replace = t->right;
                while (replace->left != nullptr)
                    replace = replace->left;
                if (t == root) root = replace;
                else
                {
                    if (isLeftChild(t)) p->left = replace;
                    else p->right = replace;
                }
                c = replace->right;
                p = replace->parent;
                col = getColor(replace);
                if (p == t) p = replace;
                else
                {
                    if (c != nullptr) c->parent = p;
                    p->left = c;
                    replace->right = t->right;
                    t->right->parent = replace;
                }
                replace->parent = t->parent;
                replace->color = t->color;
                replace->left = t->left;
                t->left->parent = replace;
            }
            if (col == BLACK) eraseRebalance(replace, col);
            delete t;
        }

        void eraseRebalance(Node *t, Color delColor)
        {
            if (t == root || t == nullptr) return;
            if (getColor(t) == RED)
            {
                t->color = delColor;
                return;
            }
            Node *p = t->parent, *s = getSibling(t);
            if (s == nullptr) return;
            Node *sl = s->left, *sr = s->right;
            if (isLeftChild(t))
            {
                if (getColor(s) == RED)
                {
                    s->color = BLACK;
                    p->color = RED;
                    leftRotate(p);
                    eraseRebalance(t, delColor);
                } else
                {
                    if (getColor(sr) == RED)
                    {
                        s->color = p->color;
                        sr->color = p->color = BLACK;
                        leftRotate(p);
                    } else if (getColor(sl) == RED)
                    {
                        s->color = RED;
                        sl->color = BLACK;
                        rightRotate(s);
                        eraseRebalance(t, delColor);
                    } else
                    {
                        s->color = RED;
                        eraseRebalance(p, delColor);
                    }
                }
            } else
            {
                if (getColor(s) == RED)
                {
                    s->color = BLACK;
                    p->color = RED;
                    rightRotate(p);
                    eraseRebalance(t, delColor);
                } else
                {
                    if (getColor(sl) == RED)
                    {
                        s->color = p->color;
                        sl->color = p->color = BLACK;
                        rightRotate(p);
                    } else if (getColor(sr) == RED)
                    {
                        s->color = RED;
                        sr->color = BLACK;
                        leftRotate(s);
                        eraseRebalance(t, delColor);
                    } else
                    {
                        s->color = RED;
                        eraseRebalance(p, delColor);
                    }
                }
            }
        }

    public:

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value)
        {
            iterator it = find(value.first);
            if (it != end()) return pair<iterator, bool>(it, false);
            else return pair<iterator, bool>(iterator(this, _insert(value)), true);
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos)
        {
            if (pos.mp != this || pos.node == nullptr) throw invalid_iterator();
            _erase(pos.node);
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const
        {
            if (find(key) != cend()) return 1;
            else return 0;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key)
        {
            return iterator(this, _find(key, root));
        }

        const_iterator find(const Key &key) const
        {
            return const_iterator(this, _find(key, root));
        }
    };
}

#endif
