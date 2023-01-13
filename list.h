//
// Created by anmol on 13/1/23.
//

#ifndef MEMALLOCATOR_LIST_H
#define MEMALLOCATOR_LIST_H

#include <cstdint>

class node {
public:
    void *addr;
    node *next;

    node(void *addr_ptr, node *next_ptr) {
        addr = addr_ptr;
        next = next_ptr;
    }
};

class List {
    node *m_head = nullptr;
    uint32_t m_size = 0;
public:

    void push(void *addr) {
        if (m_size == 0) {
            m_head = new node(addr, nullptr);
            m_size++;
        } else {
            node *new_node = new node(addr, (m_head)->next);
            m_head = new_node;
            m_size++;
        }
    }

    void *pop() {
        void *popped_addr = (m_head)->addr;
        m_head = (m_head)->next;
        m_size--;
        return popped_addr;
    }

    void remove(void *addr) {
        if ((m_head)->addr == addr) {
            m_head = (m_head)->next;
            return;
        }
        node *t_head = m_head;
        while ((t_head)->next->addr != addr)
            t_head = (t_head)->next;
        //need to think of something better
        (t_head)->next = (t_head)->next->next;
        m_size--;
    }

    uint32_t size() {
        return m_size;
    }
};

#endif //MEMALLOCATOR_LIST_H
