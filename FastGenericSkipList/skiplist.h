#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <functional> // greater
#include <iterator> // bidirectional_iterator_tag
#include <utility> // pair
#include <random> // uniform_real_distribution, default_random_engine
#include <cmath> // frexp
#include <iostream> // cout
#include <stdexcept> // std::out_of_range


template <typename Key, typename T, class Compare = greater<Key> > class SkipList
{
public:
    struct Node
    {
        std::pair<Key, T> pair;
        Node** next; // aray of ptrs
        Node** prev; // aray of ptrs
        int height; // height of this node

        Node() { }
        Node(int level) : height(level + 1)
        {
            next = new Node*[level + 1];
            prev = new Node*[level + 1];
        }
        Node(std::pair<Key, T> KeyValuePair, int level) : Node(level)
        {
            pair(KeyValuePair);
        }

        Node(Key key, T value, int level) : Node(level)
        {
            pair.first = key;
            pair.second = value;
        }

        ~Node()
        {
            delete[] next;
            delete[] prev;
        }
    };

     // iterator implementation
    class iterator
    {
        friend class SkipList;
    private:
        Node* it;
    public:
        iterator(Node* node = NULL) : it(node) { }
        iterator operator++(int) { it = it->next[0]; return it; }
        iterator& operator++() { it = it->next[0]; return iteratori(it); }
        iterator operator--(int) { it = it->prev[0]; return it; }
        iterator& operator--() { it = it->prev[0]; return iterator(it); }
        bool operator==(iterator& other) const { return it == other.it; }
        bool operator!=(iterator& other) const { return it != other.it; }
        std::pair<Key, T>& operator*() const { return it->pair; }
        std::pair<Key, T>* operator->() const { return &it->pair; }

        // iterator traits
        using difference_type = std::ptrdiff_t;
        using value_type = Node*;
        using pointer = const Node**;
        using reference = const Node&;
        using iterator_category = std::bidirectional_iterator_tag;
    };

    typedef int size_type;

    SkipList(unsigned int maxLevels = 42); // 42 is surely the best option :>
    ~SkipList();

    typename SkipList<Key, T, Compare>::iterator emplace(const Key key, const T value);
    inline typename SkipList<Key, T, Compare>::iterator emplace(const std::pair<Key, T> pair);
    inline typename SkipList<Key, T, Compare>::iterator insert(const std::pair<Key, T> pair);
    inline typename SkipList<Key, T, Compare>::iterator insert(const Key key, const T value);
    inline typename SkipList<Key, T, Compare>::iterator insert(const typename SkipList<Key, T, Compare>::iterator position, const Key key, T value);
    inline typename SkipList<Key, T, Compare>::iterator insert(const typename SkipList<Key, T, Compare>::iterator position, const std::pair<Key, T> pair);

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last);

    typename SkipList<Key, T, Compare>::iterator emplace_hint(const typename SkipList<Key, T, Compare>::iterator position, const Key key, const T value);
    inline typename SkipList<Key, T, Compare>::iterator emplace_hint(const typename SkipList<Key, T, Compare>::iterator position, const std::pair<Key, T> pair);
    typename SkipList<Key, T, Compare>::iterator find(const Key key);
    typename SkipList<Key, T, Compare>::iterator lower_bound(const Key key) const;
    typename SkipList<Key, T, Compare>::iterator upper_bound(const Key key) const;
    typename SkipList<Key, T, Compare>::iterator erase(typename SkipList<Key, T, Compare>::iterator it);
    size_type erase(Key it);

    typename SkipList<Key, T, Compare>::iterator begin() const;
    typename SkipList<Key, T, Compare>::iterator end() const;
    bool empty() const;

private:
    void debug() const;

    const int maxHeight; // max num of levels ("height")
    int currentHeight = 0; // current "height" of skip-list
    Node* head; // head of skiplist
    Node* tail; // tail of skiplist

     // random
     std::default_random_engine generator;
};

/** implementation **/

template<typename Key, typename T, class Compare>
SkipList<Key, T, Compare>::SkipList(unsigned int maxHeight) : maxHeight(maxHeight) // initialises a skiplist with the specified level height
{
    // init space for head and tail
    head = new Node(maxHeight);
    tail = new Node(maxHeight);

    // init head and tail pointers
    head->prev = NULL;
    tail->next = NULL;
    for (int i = 0; i != maxHeight; i++)
    {
        head->next[i] = tail;
        tail->prev[i] = head;
    }
}

template<typename Key, typename T, class Compare>
SkipList<Key, T, Compare>::~SkipList()
{
    Node* it = head;
    Node* next;
    while (it != tail)
    {
        next = it->next[0];
        delete it;
        it = next;
    }

    delete tail;
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace(const Key key, const T value) // inserts a new node
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double p = distribution(generator); // [0, 1)

    /*
     * calculates the node's top level using a dice roll
     * lvl = -log_2(p)
    */
    int lvl;
    std::frexp(p, &lvl);
    lvl = -lvl;

    if (lvl >= maxHeight)
        lvl = maxHeight - 1;
    if (lvl >= currentHeight)
        currentHeight = lvl + 1;

    // insertion
    Node* newNode = new Node(key, value, lvl); // creation
    Node* it = head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentHeight - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
        }

        // rebind the pointers ?
        if (i <= lvl)
        {
            newNode->next[i] = it->next[i];
            it->next[i] = newNode;

            newNode->prev[i] = it;
            newNode->next[i]->prev[i] = newNode;
        }
    }

    return newNode;
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace(const std::pair<Key, T> pair) // inserts a new node
{
    return emplace(pair.first, pair.second);
}

// alias for emplace(pair)
template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::insert(const std::pair<Key, T> pair) // inserts a new node
{
    return emplace(pair);
}

// alias for emplace(key, value)
template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::insert(const Key key, const T value) // inserts a new node
{
    return emplace(key, value);
}

// alias for emplace_hint(w/key, value)
template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::insert(const typename SkipList<Key, T, Compare>::iterator position, const Key key, const T value) // inserts a new node
{
    return emplace_hint(position, key, value);
}

// alias for emplace_hint(w/pair)
template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::insert(const typename SkipList<Key, T, Compare>::iterator position, const std::pair<Key, T> pair) // inserts a new node
{
    return emplace_hint(position, pair);
}

template<typename Key, typename T, class Compare>
template<class InputIterator>
void SkipList<Key, T, Compare>::insert(InputIterator first, InputIterator last) // range insert
{
    while(first != last)
    {
        insert(*first);
        first++;
    }
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace_hint(const typename SkipList<Key, T, Compare>::iterator position, const Key key, const T value) // inserts a new element in the SkipList, with a hint on the insertion position
{
    SkipList<Key, T, Compare>::iterator pos = position;
    pos--; // so the perfect position is now just before the insertion point
    // is the given position invalid ?
    if (Compare()(pos.it->pair.first, key) || pos == end() || pos.it == head)
        return emplace(key, value); // --> then we ignore the hint entirely

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double p = distribution(generator); // [0, 1)

    /*
     * calculates the node's top level using a dice roll
     * lvl = -log_2(p)
    */
    int lvl;
    std::frexp(p, &lvl);
    lvl = -lvl;

    if (lvl >= maxHeight)
        lvl = maxHeight - 1;
    if (lvl >= currentHeight)
        currentHeight = lvl + 1;

    // insertion
    Node* newNode = new Node(key, value, lvl); // creation
    Node* it = pos.it; // our node iterator
    // iterate over levels, from pos.height to bottom
    for (int i = it->height - 1; i >= 0; i--)
    {
        // iterate through the current level, from left to right
        for (it; it->next[i] != tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
        }

        // rebind the pointers ?
        if (i <= lvl)
        {
            newNode->next[i] = it->next[i];
            it->next[i] = newNode;

            newNode->prev[i] = it;
            newNode->next[i]->prev[i] = newNode;
        }
    }

    // iterate over levels, from pos.height to currentLevelCount (up)
    int i = pos.it->height - 1; // we've built to that lvl at max
    if (i < lvl)
        it = newNode->prev[i]; // last node at least as high as @pos
    while(i < lvl)
    {
        // iterate through the current level, from right to left
        while(true)
        {
            // higher node ? --> rebind pointers
            if (it->height - 1 > i)
            {
                i++;
                newNode->next[i] = it->next[i];
                it->next[i] = newNode;

                newNode->prev[i] = it;
                newNode->next[i]->prev[i] = newNode;
                break;
            }
            it = it->prev[i]; // move left
        }
    }

    return newNode;
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace_hint(const typename SkipList<Key, T, Compare>::iterator position, const std::pair<Key, T> pair) // inserts a new element in the SkipList, with a hint on the insertion position
{
    return emplace_hint(position, pair.first, pair.second);
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::find(const Key key) // searches the container for an element with a key equivalent to k and returns an iterator to it if found, otherwise it returns an iterator to SkipList::end.
{
    Node* it = head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentHeight - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
            // found ?
            if (!Compare()(it->next[i]->pair.first, key) && !Compare()(key, it->next[i]->pair.first)) // same as: it->next[i]->pair.first == key
            {
                it = it->next[i];
                // move to the first elt. with this key (move towards left)
                for (; (!Compare()(it->pair.first, key) && !Compare()(key, it->pair.first)); it = it->prev[0]);
                return it->next[0]; // found node (iterator)
            }
        }
    }

    return tail; // end
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::lower_bound(const Key key) const // returns an iterator pointing to the first element in the container whose key is not considered to go before k (i.e., either it is equivalent or goes after)
{
    Node* it = head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentHeight - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
            // found ?
            if (!Compare()(it->next[i]->pair.first, key) && !Compare()(key, it->next[i]->pair.first)) // same as: it->next[i]->pair.first == key
            {
                it = it->next[i];
                // move to the first elt. with this key (move towards left)
                for (; (!Compare()(it->pair.first, key) && !Compare()(key, it->pair.first)); it = it->prev[0]);
                return it->next[0]; // found node (iterator)
            }
        }
    }

    return it->next[0]; // next
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::upper_bound(const Key key) const // returns an iterator pointing to the first element in the container whose key is not considered to go before k (i.e., either it is equivalent or goes after)
{
    Node* it = head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentHeight - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
        }
    }

    return it->next[0]; // next
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::erase(const typename SkipList<Key, T, Compare>::iterator it) // removes the node from the container, return the next node (@level 0)
{
    // we don't want to bite off our head or tail :)
    if (it.it != head && it.it != tail)
    {
        // rebind pointers
        for (int i = 0; i != it.it->height; i++)
        {
            it.it->prev[i]->next[i] = it.it->next[i];
            it.it->next[i]->prev[i] = it.it->prev[i];
        }

        iterator retIt = it.it->next[0]; // next node in level 0
        delete it.it;
        return retIt; // return the next node in level 0
    }
    else
    {
        throw std::out_of_range("argument iterator does not point to a valid node");
    }
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::size_type SkipList<Key, T, Compare>::erase(const Key key) // removes all the nodes with Key from the container, returns the number of elts removed
{
    Node* it = head; // our node iterator
    Node* leftOfFound;
    // iterate over levels, from top to bottom
    for (int i = currentHeight - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
            // found first match
            if (!Compare()(it->next[i]->pair.first, key) && !Compare()(key, it->next[i]->pair.first)) // same as: it->next[i]->pair.first == key
            {
                // remove all the elements with @key
                it = it->next[i];
                leftOfFound = it->prev[0];
                int count = 0;

                // remove right
                while(!Compare()(it->pair.first, key) && !Compare()(key, it->pair.first)) // same as: it->pair.first == key
                {
                    // rebind pointers
                    for (int i = 0; i != it->height; i++)
                    {
                        it->prev[i]->next[i] = it->next[i];
                        it->next[i]->prev[i] = it->prev[i];
                    }

                    Node* nextIt = it->next[0];
                    delete it;
                    it = nextIt;
                    count++;
                }

                // remove left
                it = leftOfFound;
                while(!Compare()(it->pair.first, key) && !Compare()(key, it->pair.first)) // same as: it->pair.first == key
                {
                    // rebind pointers
                    for (int i = 0; i != it->height; i++)
                    {
                        it->prev[i]->next[i] = it->next[i];
                        it->next[i]->prev[i] = it->prev[i];
                    }

                    Node* nextIt = it->prev[0];
                    delete it;
                    it = nextIt;
                    count++;
                }

                return count;
            }
        }
    }

    return 0; // couldn't remove anything (because key is not in the list)
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::begin() const // return start iterator of level 0
{
    return head->next[0];
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::end() const // return past-the-end iterator of level 0
{
    return tail;
}

template<typename Key, typename T, class Compare>
bool SkipList<Key, T, Compare>::empty() const // returns whether the SkipList container is empty (i.e. whether its size is 0).
{
     return head->next[0] == tail;
}

template<typename Key, typename T, class Compare>
void SkipList<Key, T, Compare>::debug() const // print debug list
{
    cout << "debug print START..." << endl;

    for (Node* it = head->next[0]; it != tail; it = it->next[0])
    {
        cout << "node {" << it->pair.first << " , " << it->pair.second << "}" << endl;
        cout << "height: " << it->height << endl;

        cout << "prev-ptrs: " << endl;
        for (int i = 0; i != it->height; i++)
        {
            cout << "  " << it->prev[i]->pair.first << endl;
        }


        cout << "next-ptrs: " << endl;
        for (int i = 0; i != it->height; i++)
        {
            cout << "  " << it->next[i]->pair.first << endl;
        }
        cout << "------------------------------------" << endl;
    }

    cout << "...debug print END" << endl;
}

#endif // SKIPLIST_H
