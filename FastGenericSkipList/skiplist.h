#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <functional> // less
#include <iterator>
#include <utility> // pair
#include <random>
#include <math.h>


template <typename Key, typename T, class Compare = greater<Key> > class SkipList
{
public:
    struct Node
    {
        std::pair<Key, T> pair;
        Node** next; // aray of ptrs
        Node* prev; // prev node

        Node() { }
        Node(std::pair<Key, T> KeyValuePair, int level)
        {
            pair(KeyValuePair);
            next = new Node*[level + 1];
        }

        Node(Key key, T value, int level)
        {
            pair.first = key;
            pair.second = value;
            next = new Node*[level + 1];
        }
    };

     // iterator implementation
     class iterator
     {
     private:
         Node* it;
     public:
        iterator(Node* node) : it(node) { }
        iterator operator++(int) { return it->next[0]; }
        iterator& operator++() { return iterator(it->next[0]); }
        iterator operator--(int) { return it->prev; }
        iterator& operator--() { return iterator(it->prev); }
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
    iterator begin();
    iterator end();

private:
     const int levelCap; // max num of levels ("height")
     int currentLevelCount = 1; // current "height" of skip-list
     Node head; // head of skiplist
     Node tail; // tail of skiplist

     // random
     std::default_random_engine generator;
};

/** implementation **/

template<typename Key, typename T, class Compare>
SkipList<Key, T, Compare>::SkipList(unsigned int maxLevels) : levelCap(maxLevels) // initialises a skiplist with the specified level height
{
    // init head
    head.prev = NULL;
    head.next = new Node*[levelCap];
    for (int i = 0; i != levelCap; i++)
        head.next[i] = &tail;

    // init tail
    tail.prev = &head;
    tail.next = NULL;
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
        currentLevelCount = lvl;

    // insertion
    Node* newNode = new Node(key, value, lvl); // creation
    Node* it = &head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentLevelCount - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != &tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
        }

        // rebind the pointers ?
        if (i <= lvl)
        {
            newNode->next[i] = it->next[i];
            it->next[i] = newNode;
        }
    }

    newNode->prev = it;
    newNode->next[0]->prev = newNode;


    cout << "print:" << endl;
    for (it = head.next[0]; it != &tail; it = it->next[0])
    {
        cout << it->pair.first << " " << it->pair.second << endl;
    }
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::find(Key key) // searches the container for an element with a key equivalent to k and returns an iterator to it if found, otherwise it returns an iterator to SkipList::end.
{
    Node* it = &head; // our node iterator
    // iterate over levels, from top to bottom
    for (int i = currentLevelCount - 1; i >= 0; i--)
    {
        // iterate throught the current level, from left to right
        for (it; it->next[i] != &tail; it = it->next[i])
        {
            if(Compare()(it->next[i]->pair.first, key))
                break;
            if (it->next[i]->pair.first == key)
                return it->next[i]; // found node (iterator)
        }
    }

    return &tail; // end
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::begin() // return start iterator of level 0
{
    return &head;
}

template<typename Key, typename T, class Compare>
typename SkipList<Key, T, Compare>::iterator SkipList<Key, T, Compare>::end() // return past-the-end iterator of level 0
{
    if (head.next[0] != &tail)
        return &tail;
    return begin();
}



#endif // SKIPLIST_H
