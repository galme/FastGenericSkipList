# FastGenericSkipList
Fast and Templated Skip List implementation in C++

The idea for this project was to offer a fast replacement for <a href="http://www.cplusplus.com/reference/map/multimap/">std::multimap</a> (which uses a self-balancing binary search tree).

## Functionality
This skiplist covers all general functionality offered by multimap in the exact same way (as in the C++11 multimap), so the <a href="http://www.cplusplus.com/reference/map/multimap/">documentation</a> for multimap (C++11) is valid for this SkipList as well:
- fully templated
- <a href="http://www.cplusplus.com/reference/map/multimap/emplace/">emplace</a>
- <a href="http://www.cplusplus.com/reference/map/multimap/emplace_hint/">emplace_hint</a>
- <a href="http://www.cplusplus.com/reference/map/multimap/find/">find</a>
- <a href="http://www.cplusplus.com/reference/map/multimap/erase/">erase</a>
- <a href="http://www.cplusplus.com/reference/map/multimap/lower_bound/">lower_bound</a>
- <a href="http://www.cplusplus.com/reference/map/multimap/upper_bound/">upper_bound</a>

- <a href="http://www.cplusplus.com/reference/iterator/BidirectionalIterator/">bidirectional iterators</a>
- <a href="http://www.cplusplus.com/reference/map/multimap/begin/">begin</a>
- <a href="http://www.cplusplus.com/reference/map/multimap/end/">end</a>

Helper functions (which are not fundamental to this data-structure) are a work in progress.

## Benchmarks
To come. The situation currently is that the skip-list is on average a bit slower than the std::multimap container.
