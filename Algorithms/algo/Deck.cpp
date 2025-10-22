#include <iostream>

class Deck
{
    struct element
    {
        int* data;
        element* next;
        element* prev;
        element(int d)
        { data = new int(d); }
        element() {}
    };
    element* head;
    element* tail;
public:
    Deck()
    {
        head = nullptr;
    }
    Deck(int d)
    {
        head = new element(d);
        tail = head;
        head->prev = nullptr;
        tail->next = nullptr;
    }
    void pushBack(int d)
    {
        if (head == nullptr)
        {
           head = new element(d); tail = head;
        }
        else
        {
            element* E = tail;
            E->next = new element(d);
            E->next->prev = E;
            tail = E->next;
            tail->next = nullptr;
            E = nullptr; delete E;
         }
    }
    void pushFront(int d)
    {
        if (head == nullptr)
        {
            head = new element(d); tail = head;
        }
        else
        {
            element* E = nullptr;
            E = new element(d);
            E->next = head;
            head->prev = E;
            head = E;
            E = nullptr; delete E;
        }
    }
    bool isEmpty()
    {
        return (head == nullptr) ? true : false;
    }
    element* getLast()
    {
        return tail;
    }
    element* getFirst()
    {
        return head;
    }
    void deleteFirst()
    {
        element* E = head->next;
        delete head;
        head = E;
        E->prev = nullptr;
        E = nullptr; delete E;
    }
    void deleteLast()
    {
        element* E = tail->prev;
        delete tail;
        tail = E;
        E->nezt = nullptr;
        E = nullptr; delete E;
    }
};


int main()
{
    Deck D;
    std::cout << "D is empty? " << D.isEmpty() << std::endl;
    for (int i = 1; i <= 5; i++) 
        D.pushBack(i);
    D.pushFront(0);
    std::cout << *D.getFirst()->data << " -- " << *D.getLast()->data << std::endl;
    return 0;
}