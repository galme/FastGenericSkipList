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

    for (int i = 0; i != 5; i++)
    {
        skiplist.emplace(2, i);
    }

    if (skiplist.find(1) == skiplist.end())
        cout << "not found" << endl;
    else
        cout << "found" << endl;

    cout << "Hello World!" << endl;
    return 0;
}
