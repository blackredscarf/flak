#include <flak/Set.h>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <iterator>
using namespace std;
using namespace flak;
int main() {
    int ia[5] = {0, 1, 2, 3, 4};
    Set<int> iset(ia, ia+5);
    assert((iset.size() == 5));
    assert((iset.count(3) == 1));

    iset.insert(3);
    assert((iset.size() == 5));
    assert((iset.count(3) == 1));

    iset.insert(5);
    assert((iset.size() == 6));

    iset.erase(1);
    assert((iset.size() == 5));

    auto it1 = iset.begin();
    int ans[5] = {0,2,3,4,5};
    int cnt = 0;
    for(; it1 != iset.end(); it1++) {
        assert((*it1 == ans[cnt++]));
    }
    cout << endl;

    it1 = find(iset.begin(), iset.end(), 3);
    assert((it1 != iset.end() && *it1 == 3));

    it1 = iset.find(3);
    assert((it1 != iset.end() && *it1 == 3));

    cout << "end" << endl;
    // *it = 9 // compile error
}
