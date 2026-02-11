#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include "containers/stack.h" 

using namespace std;

using T2 = int;

void DemoStack() {
    
    CStack<T2> stack1;
    const int N = 5;

    for (auto i = 0; i < N; ++i) 
        stack1.push(i * 10, i * 100);

    cout << stack1.getSize() << endl;
    cout << stack1 << endl;

    cout << "\npop() " << endl;
    cout << stack1 << endl;

    cout << "\nCStack&& " << endl;
    CStack<T2> stack2 = std::move(stack1);
    
    cout << "Size stack1: " << stack1.getSize() << endl;
    cout << "Size stack2: " << stack2.getSize() << endl;
    cout << "stack2- " << stack2 << endl;

    cout << "\nIngresa un valor y su ref para el Stack, ej: 88 5: ";
    cin >> stack2;
    cout << "Stack actualizado: " << stack2 << endl;

}