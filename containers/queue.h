#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <iostream>
#include <mutex>
#include <assert.h>
#include "../general/types.h"
#include "../util.h"

using namespace std;

template <typename T>
class CQueue{
    using value_type  = T;
    struct Node{
        value_type m_value;
        ref_type    m_ref;
        Node     *m_pNext;
        Node ( value_type _value, ref_type _ref,Node *_pNext = nullptr)
            : m_value(_value), m_ref(_ref), m_pNext(_pNext){}
    };
private:

    Node *m_pFront    = nullptr;
    Node *m_pBack     = nullptr;
    size_t m_nElements = 0;
    mutable std::mutex m_mtx;

public: 

    CQueue() {}
    CQueue(const CQueue<T>& other);
    CQueue(CQueue<T>&& other) noexcept;
    virtual ~CQueue(); 

    void push(const T& _value, ref_type _ref); 
    bool pop();
    
    size_t getSize() const { return m_nElements; }

    friend ostream& operator<<(ostream& os, const CQueue<T>& container) {
        lock_guard<mutex> lock(container.m_mtx);
        os << "Queue size: " << container.getSize() << endl;
        os << "[";
        Node* pTemp = container.m_pFront;
        while (pTemp != nullptr) {
            os << "(" << pTemp->m_value << ":" << pTemp->m_ref << ")";
            pTemp = pTemp->m_pNext;
            if (pTemp != nullptr) 
            os << ", ";
        }
        os << "]";
        return os;
    }

    friend istream& operator>>(istream& is, CQueue<T>& container) {
        value_type _value;
        ref_type _ref;
        if (is >> _value >> _ref) {
            container.push(_value, _ref);
        }
        return is;
    }
};

template <typename T>
CQueue<T>::CQueue(const CQueue<T>& other) {
    lock_guard<mutex> lock(other.m_mtx);
    Node *pTemp = other.m_pFront;
    while (pTemp){
        this->push(pTemp->m_value, pTemp->m_ref);
        pTemp = pTemp->m_pNext;
    }
}

template <typename T>
CQueue<T>::CQueue(CQueue<T>&& other) noexcept {
    lock_guard<mutex> lock(other.m_mtx);
    m_pFront    = other.m_pFront;
    m_pBack     = other.m_pBack;
    m_nElements = other.m_nElements;

    other.m_pFront    = nullptr;
    other.m_pBack     = nullptr;
    other.m_nElements = 0;
}

template <typename T>
CQueue<T>::~CQueue() {
    lock_guard<mutex> lock(m_mtx);
    while (m_pFront) {
        Node *pTemp = m_pFront;
        m_pFront    = m_pFront->m_pNext;
        delete pTemp;
    }
    m_pBack     = nullptr;
    m_nElements = 0;
}
template <typename T>
void CQueue<T>::push(const T& _value, ref_type _ref) {
    lock_guard<mutex> lock(m_mtx);
    Node *pNew = new Node(_value,_ref);
    if (!m_pFront) 
        m_pFront = pNew;
    else
        m_pBack->m_pNext = pNew;
    m_pBack = pNew;
    m_nElements++;
}

template <typename T>
bool CQueue<T>::pop() {
    lock_guard<mutex> lock(m_mtx);
    if (!m_pFront) 
        return false;

    Node* pTemp = m_pFront;
    m_pFront = m_pFront->m_pNext;
    delete pTemp;

    if (!m_pFront) 
        m_pBack = nullptr;
    m_nElements--;
 return true;
}

#endif // __QUEUE_H__