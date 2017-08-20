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
        std::pair<Key, T>& operator*() { return it->pair; }
        std::pair<Key, T>* operator->() { return &it->pair; }

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

    typename SkipList<Key, T, Compare>::iterator emplace(Key key, T value);
    typename SkipList<Key, T, Compare>::iterator emplace(std::pair<Key, T> pair);
    typename SkipList<Key, T, Compare>::iterator emplace_hint(typename SkipList<Key, T, Compare>::iterator position, Key key, T value);
    typename SkipList<Key, T, Compare>::iterator emplace_hint(typename SkipList<Key, T, Compare>::iterator position, std::pair<Key, T> pair);
    typename SkipList<Key, T, Compare>::iterator find(Key key);
    typename SkipList<Key, T, Compare>::iterator erase(typename SkipList<Key, T, Compare>::iterator it);
    size_type erase(Key it);

    typename SkipList<Key, T, Compare>::iterator begin() const;
    typename SkipList<Key, T, Compare>::iterator end() const;
    bool empty() const;
    void debug();

private:
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
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace(Key key, T value) // inserts a new node
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
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace(std::pair<Key, T> pair) // inserts a new node
{
    return emplace(pair.first, pair.second);
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace_hint(typename SkipList<Key, T, Compare>::iterator position, Key key, T value) // inserts a new element in the SkipList, with a hint on the insertion position
{
    // is the given position invalid ?
    if (Compare()(position.it->pair.first, key) || position == end())
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
    Node* it = position.it; // our node iterator
    // iterate over levels, from position.height to bottom
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

    // iterate over levels, from position.height to currentLevelCount (up)
    int i = position.it->height - 1; // we've built to that lvl at max
    if (i < lvl)
        it = newNode->prev[i]; // last node at least as high as @position
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
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::emplace_hint(typename SkipList<Key, T, Compare>::iterator position, std::pair<Key, T> pair) // inserts a new element in the SkipList, with a hint on the insertion position
{
    return emplace_hint(position, pair.first, pair.second);
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::find(Key key) // searches the container for an element with a key equivalent to k and returns an iterator to it if found, otherwise it returns an iterator to SkipList::end.
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
            if (it->next[i]->pair.first == key)
            {
                it = it->next[i];
                // move to the first elt. with this key (move towards left)
                for (; it->pair.first == key; it = it->prev[0]);
                return it->next[0]; // found node (iterator)
            }
        }
    }

    return tail; // end
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::erase(typename SkipList<Key, T, Compare>::iterator it) // removes the node from the container, return the next node (@level 0)
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
typename SkipList<Key, T, Compare>::size_type SkipList<Key, T, Compare>::erase(Key key) // removes all the nodes with Key from the container, returns the number of elts removed
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
            if (it->next[i]->pair.first == key)
            {
                // remove all the elements with @key
                it = it->next[i];
                leftOfFound = it->prev[0];
                int count = 0;

                // remove right
                while(it->pair.first == key)
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
                while(it->pair.first == key)
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
    if (head->next[0] != tail)
        return head->next[0];
    return head;
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::end() const // return past-the-end iterator of level 0
{
    if (head->next[0] != tail)
        return tail;
    return begin();
}

template<typename Key, typename T, class Compare>
bool SkipList<Key, T, Compare>::empty() const // returns whether the SkipList container is empty (i.e. whether its size is 0).
{
     return head->next[0] == tail;
}

template<typename Key, typename T, class Compare>
void SkipList<Key, T, Compare>::debug() // print debug list
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
