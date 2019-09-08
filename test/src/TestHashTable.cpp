#include <flak/HashTable.h>
#include <hashtable.h>
#include <functional>
#include <cassert>
#include <hashtable.h>
#include <iostream>
using namespace std;
using namespace flak;
using __gnu_cxx::hashtable;

void test1() {
    HashTable<int,
            int,
            hash<int>,
            _Identity<int>,
            equal_to<int>,
            std::allocator<int>> ht(50, hash<int>(), equal_to<int>());

    hashtable<int,
            int,
            hash<int>,
            _Identity<int>,
            equal_to<int>,
            std::allocator<int>> ht2(50, hash<int>(), equal_to<int>());

    assert((ht.size() == ht2.size()));
    assert((ht.bucketCount() == ht2.bucket_count()));
    assert((ht.maxBucketCount() == ht2.max_bucket_count()));

    ht.insertUnique(59);
    ht.insertUnique(63);
    ht.insertUnique(108);
    ht.insertUnique(2);
    ht.insertUnique(53);
    ht.insertUnique(55);

    ht2.insert_unique(59);
    ht2.insert_unique(63);
    ht2.insert_unique(108);
    ht2.insert_unique(2);
    ht2.insert_unique(53);
    ht2.insert_unique(55);

    assert((ht.size() == 6 && ht.size() == ht2.size()));

    auto it1 = ht.begin();
    int ans[6] = {53,55,2,108,59,63};
    for(int i = 0; i < ht.size(); i++, ++it1) {
        assert((*it1 == ans[i]));
    }
    cout << endl;

    int cans[4] = {1,3,1,1};
    int cnt = 0;
    for(int i = 0; i < ht.bucketCount(); i++) {
        int n = ht.elemsInBucket(i);
        int n2 = ht2.elems_in_bucket(i);
        if(n != 0) {
            assert((n == cans[cnt++] && n == n2));
        }
    }

    for(int i = 0; i <= 47; i++){
        ht.insert_equal(i);
        ht2.insert_equal(i);
    }
    assert((ht.size() == 54));
    assert((ht2.size() == ht.size()));
    assert((ht.bucketCount() == 97));
    assert((ht2.bucket_count() == ht.bucketCount()));


    assert((*ht.find(2) == 2));
    assert((*ht2.find(2) == 2));
    assert((ht.count(2) == 2));
    assert((ht2.count(2) == 2));

    ht.erase(2);
    ht2.erase(2);
    assert((ht.size() == 52));
    assert((ht.size() == ht2.size()));
    cout << "test 1 end" << endl;
}

template<class T>
struct MyHash : public __hash_base<size_t, T>
{
  size_t
  operator()(T __val) const noexcept
  { return static_cast<size_t>(__val); }
};

void test2() {
    typedef HashTable<char,
            int,
            MyHash<char>,
            _Identity<char>,
            equal_to<char>,
            std::allocator<int>>  MyHashTable;

    MyHashTable ht(50, MyHash<char>(), equal_to<char>());

    hashtable<char,
            int,
            MyHash<char>,
            _Identity<char>,
            equal_to<char>,
            std::allocator<int>> ht2(50, MyHash<char>(), equal_to<char>());

    ht.insertUnique('a');
    ht.insertUnique('b');
    ht.insertUnique('c');

    ht2.insert_unique('a');
    ht2.insert_unique('b');
    ht2.insert_unique('c');

    cout << ht.size() << endl;
    auto it1 = ht.begin();
    auto it2 = ht2.begin();
    for(int i = 0; i < ht.size(); i++) {
        assert((*it1 == *it2));
        ++it1;
        ++it2;
    }
    cout << "test 2 end" << endl;
}

void test3() {
    hash<string> shash;
    cout << shash("a") << endl;
    cout << shash("b") << endl;
    cout << shash("hello") << endl;
    cout << shash("hello2") << endl;
}

void test4() {
    typedef HashTable<char,
            int,
            MyHash<char>,
            _Identity<char>,
            equal_to<char>,
            std::allocator<int>>  MyHashTable;

    MyHashTable ht(50, MyHash<char>(), equal_to<char>());
    ht.insertUnique('a');
    ht.insertUnique('b');
    ht.insertUnique('c');
    ht.insertUnique('d');
    ht.insertUnique('e');
    ht.insertUnique('f');

    auto it1 = ht.begin();
    it1++, it1++, it1++;
    ht.erase(it1, ht.end());
    assert(ht.size() == 3);

    ht.erase(ht.begin());
    assert((ht.size() == 2));
    it1 = ht.begin();
    char ans[2] = {'b', 'c'};
    int cnt = 0;
    for(int i = 0; i < ht.size(); i++) {
        assert((*it1 == ans[cnt++]));
        ++it1;
    }
    cout << "test4 end" << endl;
}

int main() {
//    test1();
//    test2();
    test4();
}
