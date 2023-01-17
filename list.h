//
// Created by anmol on 13/1/23.
//

#ifndef MEMALLOCATOR_LIST_H
#define MEMALLOCATOR_LIST_H

#include <cstdint>

class Node {
public:
    void *m_addr;
    Node *m_next;

    Node(void *addr_ptr, Node *next_ptr) {
        m_addr = addr_ptr;
        m_next = next_ptr;
    }
};

class List {
    Node *m_head = nullptr;
    uint32_t m_size = 0;
public:

    void push(void *addr) {
        Node *new_node = new Node(addr, m_head);
        m_head = new_node;
        m_size++;
    }

    //pop always from front for constant complexity
    void *pop() {
        void *popped_addr = (m_head)->m_addr;
        m_head = (m_head)->m_next;
        m_size--;
        return popped_addr;
    }

    void remove(void *addr) {
        if ((m_head)->m_addr == addr) {
            m_head = (m_head)->m_next;
            return;
        }
        Node *t_head = m_head;
        while ((t_head)->m_next->m_addr != addr)
            t_head = (t_head)->m_next;
        //need to think of something better
        (t_head)->m_next = (t_head)->m_next->m_next;
        m_size--;
    }

    uint32_t size() {
        return m_size;
    }
};

#endif //MEMALLOCATOR_LIST_H
