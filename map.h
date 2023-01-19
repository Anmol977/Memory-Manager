//
// Created by anmol on 13/1/23.
//

#ifndef MEMALLOCATOR_MAP_H
#define MEMALLOCATOR_MAP_H

#include<cstdint>
#include <iostream>
#include "list.h"

bool isprime(int n)
{
    if (n <= 3)
        return true;
    int i = 2;
    while (i * i <= n)
    {
        if (n % i == 0)
            return false;
        i++;
    }
    return true;
}

int findnextprime(int n)
{
    bool res = false;
    while (n++)
    {
        res = isprime(n);
        if (res)
            return n;
    }
    return n;
}

class MapNode {
public:
    uint32_t m_key;
    void *m_value;
    MapNode *m_next;
    MapNode(uint32_t t_key, void *t_value) {
        m_key = t_key;
        m_value = t_value;
        m_next = nullptr;
    }

    ~MapNode() {
        if (m_next != nullptr) {
            delete m_next;
        }
    }
};

class Map {
    MapNode **m_table;
    uint32_t m_maxSize;
    uint32_t m_currentSize;

    uint32_t hash_func(uint32_t t_key){
        t_key =   t_key * 0xdeece66d + 0xb;
        return t_key % 32; // can be made faster
    }

//    void rehash(){
//        MapNode **old_table = m_table;
//        uint32_t  old_max_sz = m_maxSize;
//        m_maxSize = findnextprime(m_maxSize << 1);
//        m_currentSize = 0;
//        m_table = new MapNode * [m_maxSize];
//        for(int i = 0 ; i < old_max_sz; i++){
//            m_table[i] = nullptr;
//        }
//        MapNode *temp;
//        for(int i = 0 ; i < old_max_sz ; i++){
//            temp = old_table[i];
//            while(temp != nullptr){
//                insert(temp->m_key, temp->m_value);
//                temp = temp->m_next;
//            }
//            if(old_table[i] != nullptr){
//                delete old_table[i];
//            }
//        }
//        delete[] old_table;
//    }
public:
    Map() {
        m_maxSize = 32; //supports segregated list key values upto 256 bytes initially
//        m_currentSize = 0;
        m_table = new MapNode *[m_maxSize];
        for (int i = 0; i < m_maxSize; i++) {
            m_table[i] = nullptr;
        }
    }
    void insert(uint32_t t_key, void *t_val){
        uint32_t idx = hash_func(t_key);
        MapNode *new_node = new MapNode(t_key,t_val);
        new_node->m_next = m_table[idx];
        m_table[idx] = new_node;
//        m_currentSize++;
    }
    void printMap(){
        for(int i = 0; i < m_maxSize; i++){
            std::cout<< " idx "<< i << " -> ";
            MapNode *temp = m_table[i];
            if(temp != nullptr){
            while(temp != nullptr){
                std::cout<<temp->m_value<< " -> ";
                temp = temp->m_next;
            }
            }
            std::cout<<std::endl;
        }
    }
    void *search(uint32_t t_key){
        int idx = hash_func(t_key);
        MapNode *front = m_table[idx];
        while(front == nullptr){
            front = m_table[idx++];
        }
        if(!front){
            return nullptr;
        }

        m_table[idx] = m_table[idx]->m_next ? m_table[idx]->m_next : nullptr;
        return (*front).m_value;
    }
};

#endif //MEMALLOCATOR_MAP_H
