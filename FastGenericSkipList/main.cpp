#include <iostream>
#include <random> // uniform_real_distribution, default_random_engine
#include <map>
#include <chrono>
#include "skiplist.h"
#define TEST_SIZE 1000000

using namespace std;

class TestClass
{
    int a;
    int b;
    float c;
    char d;
};

int main(int argc, char *argv[])
{
    // random distributions, generator
    default_random_engine generator;
    uniform_real_distribution<double> doubleDistribution(-TEST_SIZE, TEST_SIZE);
    uniform_int_distribution<int> intDistribution(-TEST_SIZE, TEST_SIZE);
    chrono::steady_clock::time_point start;
    chrono::steady_clock::time_point end;

    // reserve space
    int* intPool = new int[TEST_SIZE];
    double* doublePool = new double[TEST_SIZE];
    SkipList<int, TestClass>::iterator* intSkiplistIterators = new SkipList<int, TestClass>::iterator[TEST_SIZE];
    SkipList<double, TestClass>::iterator* doubleSkiplistIterators = new SkipList<double, TestClass>::iterator[TEST_SIZE];
    multimap<int, TestClass>::iterator* intMmapIterators = new multimap<int, TestClass>::iterator[TEST_SIZE];
    multimap<double, TestClass>::iterator* doubleMmapIterators = new multimap<double, TestClass>::iterator[TEST_SIZE];

    // fill the pool with randoms
    for (int i = 0; i != TEST_SIZE; i++)
    {
        int randInt = intDistribution(generator);
        double randDouble = doubleDistribution(generator);

        intPool[i] = randInt;
        doublePool[i] = randDouble;
    }

    SkipList<int, TestClass> intSkiplist;
    multimap<int, TestClass> intMmap;
    SkipList<double, TestClass> doubleSkiplist;
    multimap<double, TestClass> doubleMmap;

    cout << "INSERTION TESTS [START]..." << endl;
    /* INSERTION TEST: INT SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intSkiplist.emplace(intPool[i], TestClass());
    }
    end = std::chrono::steady_clock::now();
    cout << "INSERTION TEST: INT SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* INSERTION TEST: INT MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intMmap.emplace(intPool[i], TestClass());
    }
    end = std::chrono::steady_clock::now();
    cout << "INSERTION TEST: INT MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* INSERTION TEST: DOUBLE SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleSkiplist.emplace(doublePool[i], TestClass());
    }
    end = std::chrono::steady_clock::now();
    cout << "INSERTION TEST: DOUBLE SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* INSERTION TEST: DOUBLE MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleMmap.emplace(doublePool[i], TestClass());
    }
    end = std::chrono::steady_clock::now();
    cout << "INSERTION TEST: DOUBLE MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    cout << "INSERTION TESTS [END]..." << endl;



    cout << endl;



    cout << "SEARCH TESTS [START]..." << endl;
    /* SEARCH TEST: INT SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intSkiplist.find(intPool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "SEARCH TEST: INT SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* SEARCH TEST: INT MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intMmap.find(intPool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "SEARCH TEST: INT MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* SEARCH TEST: DOUBLE SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleSkiplist.find(doublePool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "SEARCH TEST: DOUBLE SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* SEARCH TEST: DOUBLE MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleMmap.find(doublePool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "SEARCH TEST: DOUBLE MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    cout << "SEARCH TESTS [END]..." << endl;



    cout << endl;



    cout << "ERASE TESTS [START]..." << endl;
    /* ERASE TEST: INT SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intSkiplist.erase(intPool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "ERASE TEST: INT SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* ERASE TEST: INT MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intMmap.erase(intPool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "ERASE TEST: INT MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* ERASE TEST: DOUBLE SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleSkiplist.erase(doublePool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "ERASE TEST: DOUBLE SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* ERASE TEST: DOUBLE MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleMmap.erase(doublePool[i]);
    }
    end = std::chrono::steady_clock::now();
    cout << "ERASE TEST: DOUBLE MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    cout << "ERASE TESTS [END]..." << endl;

    cout << endl;

    // refill
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intSkiplist.emplace(intPool[i], TestClass());
    }
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intMmap.emplace(intPool[i], TestClass());
    }
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleSkiplist.emplace(doublePool[i], TestClass());
    }
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleMmap.emplace(doublePool[i], TestClass());
    }

    // get iterators
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intSkiplistIterators[i] = intSkiplist.find(intPool[i])++;
    }
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intMmapIterators[i] = intMmap.find(intPool[i])++;
    }
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleSkiplistIterators[i] = doubleSkiplist.find(doublePool[i])++;
    }
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleMmapIterators[i] = doubleMmap.find(doublePool[i])++;
    }


    cout << "EMPLACE_HINT w/perfect_hint TESTS [START]..." << endl;
    /* EMPLACE_HINT w/perfect_hint TEST: INT SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intSkiplist.emplace_hint(intSkiplistIterators[i], intPool[i], TestClass());
    }
    end = std::chrono::steady_clock::now();
    cout << "EMPLACE_HINT w/perfect_hint: INT SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* EMPLACE_HINT w/perfect_hint TEST: INT MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        intMmap.emplace_hint(intMmapIterators[i], pair<int, TestClass>(intPool[i], TestClass()));
    }
    end = std::chrono::steady_clock::now();
    cout << "EMPLACE_HINT w/perfect_hint: INT MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* EMPLACE_HINT w/perfect_hint TEST: DOUBLE SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleSkiplist.emplace_hint(doubleSkiplistIterators[i], doublePool[i], TestClass());
    }
    end = std::chrono::steady_clock::now();
    cout << "EMPLACE_HINT w/perfect_hint: DOUBLE SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* EMPLACE_HINT w/perfect_hint TEST: DOUBLE MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = 0; i != TEST_SIZE; i++)
    {
        doubleMmap.emplace_hint(doubleMmapIterators[i], pair<double, TestClass>(doublePool[i], TestClass()));
    }
    end = std::chrono::steady_clock::now();
    cout << "EMPLACE_HINT w/perfect_hint: DOUBLE MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    cout << "EMPLACE_HINT w/perfect_hint TESTS [END]..." << endl;



    cout << endl;



    cout << "LOWER_BOUND TESTS [START]..." << endl;
    const int step = 10;
    /* LOWER_BOUND TEST: INT SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        intSkiplist.lower_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "LOWER_BOUND TEST: INT SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* LOWER_BOUND TEST: INT MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        intMmap.lower_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "LOWER_BOUND TEST: INT MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* LOWER_BOUND TEST: DOUBLE SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        doubleSkiplist.lower_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "LOWER_BOUND TEST: DOUBLE SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* LOWER_BOUND TEST: DOUBLE MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        doubleMmap.lower_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "LOWER_BOUND TEST: DOUBLE MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    cout << "LOWER_BOUND TESTS [END]..." << endl;



    cout << endl;



    cout << "UPPER_BOUND TESTS [START]..." << endl;
    /* UPPER_BOUND TEST: INT SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        intSkiplist.upper_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "UPPER_BOUND TEST: INT SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* UPPER_BOUND TEST: INT MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        intMmap.upper_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "UPPER_BOUND TEST: INT MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* UPPER_BOUND TEST: DOUBLE SKIPLIST */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        SkipList<double, TestClass>::iterator it = doubleSkiplist.upper_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "UPPER_BOUND TEST: DOUBLE SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* UPPER_BOUND TEST: DOUBLE MULTIMAP */
    start = std::chrono::steady_clock::now();
    for (int i = -TEST_SIZE; i < TEST_SIZE; i += step)
    {
        doubleMmap.upper_bound(i);
    }
    end = std::chrono::steady_clock::now();
    cout << "UPPER_BOUND TEST: DOUBLE MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    cout << "UPPER_BOUND TESTS [END]..." << endl;



    cout << endl;




    cout << "SWEEP TESTS [START]..." << endl;
    /* SWEEP TEST: INT SKIPLIST */
    start = std::chrono::steady_clock::now();

    SkipList<int, TestClass>::iterator intItSL = intSkiplist.begin();
    while(intItSL != intSkiplist.end())
        intItSL++;

    end = std::chrono::steady_clock::now();
    cout << "SWEEP TEST: INT SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* SWEEP TEST: INT MULTIMAP */
    start = std::chrono::steady_clock::now();

    multimap<int, TestClass>::iterator intItMM = intMmap.begin();
    while(intItMM != intMmap.end())
        intItMM++;

    end = std::chrono::steady_clock::now();
    cout << "SWEEP TEST: INT MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* SWEEP TEST: DOUBLE SKIPLIST */
    start = std::chrono::steady_clock::now();

    SkipList<double, TestClass>::iterator doubleItSL = doubleSkiplist.begin();
    while(doubleItSL != doubleSkiplist.end())
        doubleItSL++;

    end = std::chrono::steady_clock::now();
    cout << "SWEEP TEST: DOUBLE SKIPLIST - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    /* SWEEP TEST: DOUBLE MULTIMAP */
    start = std::chrono::steady_clock::now();

    multimap<double, TestClass>::iterator doubleItMM = doubleMmap.begin();
    while(doubleItMM != doubleMmap.end())
        doubleItMM++;

    end = std::chrono::steady_clock::now();
    cout << "SWEEP TEST: DOUBLE MULTIMAP - " << chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;

    cout << "SWEEP TESTS [END]..." << endl;

    return 0;
}
