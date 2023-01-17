//
// Created by anmol on 13/1/23.
//

#ifndef MEMALLOCATOR_MAP_H
#define MEMALLOCATOR_MAP_H

#include<cstdint>

class Node {
    uint32_t m_key;
    void *m_value;
    Node *m_next;
public:
    Node(uint32_t t_key, void *t_value) {
        m_key = t_key;
        m_value = t_value;
        m_next = nullptr;
    }

    ~Node() {
        if (m_next != nullptr) {
            delete m_next;
        }
    }
};

class Map {
    Node **m_table;
    uint32_t m_maxSize;
    uint32_t m_currentSize;
public:
    Map() {
        m_maxSize = 32; //supports segregated list key values upto 256 bytes
        m_currentSize = 0;
        m_table = new Node *[m_maxSize];
        for (int i = 0; i < m_maxSize; i++) {
            m_table[i] = nullptr;
        }
    }
    Map operator [] (uint32_t key){
        
    }
};

#endif //MEMALLOCATOR_MAP_H
