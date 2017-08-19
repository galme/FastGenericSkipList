#include <iostream>
#include "skiplist.h"

using namespace std;

int main(int argc, char *argv[])
{
    SkipList<float, float> skiplist(32);

    SkipList<float, float>::iterator it = skiplist.begin();
    it = skiplist.end();
    cout << (skiplist.begin() == skiplist.end());


    for (int i = 0; i != 5; i++)
    {
        skiplist.emplace(i, i);
    }


    if (skiplist.find(1) == skiplist.end())
        cout << "not found" << endl;
    else
        cout << "found" << endl;

    skiplist.erase((skiplist.end()--));

    skiplist.emplace(10, 10);
    skiplist.emplace(10, 11);
    skiplist.emplace(10, 12);
    skiplist.emplace(10, 13);
    skiplist.emplace(10, 14);
    skiplist.emplace(11, 14);
    skiplist.emplace(10, 15);
    skiplist.debug();

    cout << "removed: " << skiplist.erase(10);

    skiplist.emplace(2, 15);
    skiplist.emplace(3, 15);
    cout << "removed: " << skiplist.erase(2);
    skiplist.debug();

    skiplist.emplace_hint(skiplist.begin(), 10, 16);
    skiplist.emplace_hint(skiplist.end()--, 15, 17);
    skiplist.emplace_hint(skiplist.end()--, 15, 18);

    skiplist.debug();


    return 0;
}
