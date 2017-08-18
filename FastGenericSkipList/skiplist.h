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
    };

     // iterator implementation
     class iterator
     {
         friend class SkipList;
     private:
         Node* it;
     public:
        iterator(Node* node) : it(node) { }
        iterator operator++(int) { return it->next[0]; }
        iterator& operator++() { return iterator(it->next[0]); }
        iterator operator--(int) { return it->prev[0]; }
        iterator& operator--() { return iterator(it->prev[0]); }
        bool operator==(iterator& other) const { return it == other.it; }
        bool operator!=(iterator& other) const { return it != other.it; }
        std::pair<Key, T>& operator*() { return it->pair; }

        // iterator traits
        using difference_type = std::ptrdiff_t;
        using value_type = Node*;
        using pointer = const Node**;
        using reference = const Node&;
        using iterator_category = std::bidirectional_iterator_tag;
     };


    SkipList(unsigned int maxLevels = 32);
    void emplace(Key key, T value);
    iterator find(Key key);
    iterator erase(typename SkipList<Key, T, Compare>::iterator it);
    iterator begin();
    iterator end();
    void debug();

private:
     const int levelCap; // max num of levels ("height")
     int currentLevelCount = 1; // current "height" of skip-list
     Node* head; // head of skiplist
     Node* tail; // tail of skiplist

     // random
     std::default_random_engine generator;
};

/** implementation **/

template<typename Key, typename T, class Compare>
SkipList<Key, T, Compare>::SkipList(unsigned int maxLevels) : levelCap(maxLevels) // initialises a skiplist with the specified level height
{
    // init space for head and tail
    head = new Node(levelCap);
    tail = new Node(levelCap);

    // init head and tail pointers
    head->prev = NULL;
    tail->next = NULL;
    for (int i = 0; i != levelCap; i++)
    {
        head->next[i] = tail;
        tail->prev[i] = head;
    }
}

template<typename Key, typename T, class Compare>
void SkipList<Key, T, Compare>::emplace(Key key, T value) // inserts a new node
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

    if (lvl >= levelCap)
        lvl = levelCap;
    if (lvl >= currentLevelCount)
        currentLevelCount = lvl + 1;

    // insertion
    Node* newNode = new Node(key, value, lvl); // creation
    Node* it = head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentLevelCount - 1; i >= 0; i--)
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
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::find(Key key) // searches the container for an element with a key equivalent to k and returns an iterator to it if found, otherwise it returns an iterator to SkipList::end.
{
    Node* it = head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentLevelCount - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
            if (it->next[i]->pair.first == key)
                return it->next[i]; // found node (iterator)
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
            // @optimize : this could be done faster with memcpy...
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
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::begin() // return start iterator of level 0
{
    if (head->next[0] != tail)
        return head->next[0];
    return head;
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::end() // return past-the-end iterator of level 0
{
    if (head->next[0] != tail)
        return tail;
    return begin();
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
