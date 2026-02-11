#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "containers/queue.h" 

using namespace std;

using T1 = int;

template <typename Q>
void Print(Q &elem) {  cout << elem << " ";  }

void DemoQueue() {
    CQueue<T1> queue1;
    const int N = 5;

    for (auto i = 0; i < N; ++i) 
        queue1.push(i * 10, i * 100);

    cout << "Cola Size: " << queue1.getSize() << endl;
    cout << queue1 << endl;

    cout << "\npop() " << endl;
    if (queue1.pop()) 
        cout << queue1 << endl;

    cout << "\n(CQueue&&) " << endl;
    CQueue<T1> queue2 = std::move(queue1);
    
    cout << "size queue1: " << queue1.getSize() << endl;
    cout << "size queue2: " << queue2.getSize() << endl;
    cout << "queue2- " << queue2 << endl;

    cout << "\nIngresa un valor y su ref, ej: 77 9: ";
    cin >> queue2;
    cout << queue2 << endl;

}