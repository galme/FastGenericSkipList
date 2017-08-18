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

    skiplist.debug();

    return 0;
}
