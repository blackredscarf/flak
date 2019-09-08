#include <cassert>
#include <iostream>
#include <flak/PriorityQueue.h>
using namespace std;
using namespace flak;
int main() {
    int ia[9] = {0,1,2,3,4,8,9,3,5};
    PriorityQueue<int> pq(ia, ia+9);
    assert((pq.size() == 9));

    pq.push(2);
    assert((pq.size() == 10));

    int ans[10]= {9,8,5,4,3,3,2,2,1,0};
    int cnt = 0;
    while(!pq.empty()) {
        assert((pq.top() == ans[cnt++]));
        pq.pop();
    }
    cout << "end" << endl;
}