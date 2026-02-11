#ifndef __STACK_H__
#define __STACK_H__

#include <iostream>
#include <mutex>
#include <assert.h>
#include "../general/types.h"
#include "../util.h"

using namespace std;

template <typename T>
class CStack{
    using value_type = T;
    struct Node {
        value_type m_value;
        ref_type     m_ref;
        Node *m_pNext;
        Node(value_type _value, ref_type _ref, Node *_pNext = nullptr)
            : m_value(_value), m_ref(_ref), m_pNext(_pNext){}
    };

private: 
    Node *m_pTop       = nullptr;
    size_t m_nElements = 0;
    mutable std::mutex m_mtx;

public:
    CStack(){}
    CStack(const CStack<T>& other);
    CStack(CStack<T>&& other) noexcept;
    virtual ~CStack();

    void push(const T& _value, ref_type _ref);
    bool pop();

    size_t getSize() const{ return m_nElements; }

    friend ostream& operator<<(ostream& os, const CStack<T>& container) {
        lock_guard<mutex> lock(container.m_mtx);
        os << "Stack size: " << container.getSize() << endl;
        os << "Top -> [";
        Node* pTemp = container.m_pTop;
        while (pTemp != nullptr) {
            os << "(" << pTemp->m_value << ":" << pTemp->m_ref << ")";
            pTemp = pTemp->m_pNext;
            if (pTemp != nullptr) os << ", ";
        }
        os << "]";
        return os;
    }

    friend istream& operator>>(istream& is, CStack<T>& container) {
        value_type _value;
        ref_type _ref;
        if (is >> _value >> _ref) {
            container.push(_value, _ref);
        }
        return is;
    }
};

template <typename T>
CStack<T>::CStack(const CStack<T>& other){
    lock_guard<mutex> lock(other.m_mtx);

    Node *pTemp = other.m_pTop;
    while (pTemp){
        this->push(pTemp->m_value, pTemp->m_ref);
        pTemp = pTemp->m_pNext;
    }
}

template <typename T>
CStack<T>::CStack(CStack<T>&& other) noexcept {
    lock_guard<mutex> lock(other.m_mtx);
    m_pTop = other.m_pTop;
    m_nElements = other.m_nElements;

    other.m_pTop = nullptr;
    other.m_nElements = 0;
}

template <typename T>
CStack<T>::~CStack() {
    lock_guard<mutex> lock(m_mtx);
    while (m_pTop) {
        Node* pTemp = m_pTop;
        m_pTop = m_pTop->m_pNext;
        delete pTemp;
    }
    m_nElements = 0;
}

template <typename T>
void CStack<T>::push(const T& _value, ref_type _ref) {
    lock_guard<mutex> lock(m_mtx);

    Node* pNew = new Node(_value, _ref, m_pTop);
    m_pTop = pNew;
    m_nElements++;
}

template <typename T>
bool CStack<T>::pop(){
    lock_guard<mutex> lock(m_mtx);
    if (!m_pTop)
        return false;
    
    Node *pTemp = m_pTop;
    m_pTop      = m_pTop->m_pNext;
    delete pTemp;
    m_nElements--;
    return true;
}

#endif // __STACK_H__