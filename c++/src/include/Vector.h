//
// Created by lenovo on 9/27/2023.
//

#ifndef LEARN_RECORD_VECTOR_H
#define LEARN_RECORD_VECTOR_H

#include <cstdio>

template<typename T>
class Vector{
public:
    Vector(){
        ReAlloc(2);
    }

    void push_back(const T& value){
        if(m_size >= m_capacity){
            ReAlloc(m_capacity + m_capacity / 2);
        }

        m_data[m_size] = value;
        m_size++;
    }

    const T& operator[](size_t index) const{
        return m_data[index];
    }
    T& operator[](size_t index){
        return m_data[index];
    }

    size_t size() const { return m_size; }
private:
    void ReAlloc(size_t newCapacity){
        T* newBlock = new T[newCapacity];

        if(newCapacity < m_size){
            m_size = newCapacity;
        }

        for(size_t i = 0; i < m_size; i++){
            newBlock[i] = m_data[i];
        }
        delete[] m_data;
        m_data = newBlock;
        m_capacity = newCapacity;
    }

    T* m_data;
    size_t m_size = 0;
    size_t m_capacity = 0;
};

#endif //LEARN_RECORD_VECTOR_H
