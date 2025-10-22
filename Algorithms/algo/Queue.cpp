#include <iostream>
#include <stdexcept>

template <typename T>
class queue {
private:
    struct Node {
        T value;
        Node* next;
        Node(T val) : value(val), next(nullptr) {}
    };
    
    Node* head;  // Указатель на начало очереди
    Node* tail;  // Указатель на конец очереди (для эффективного добавления)

public:
    // Конструктор (C++11: инициализация в списке инициализации)
    queue() : head(nullptr), tail(nullptr) {}
    
    // Деструктор (C++11: гарантирует отсутствие утечек памяти)
    ~queue() {
        while (head != nullptr) {
            pop();
        }
    }
    
    // Запрет копирования (C++11: удаленные функции)
    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;
    
    // Добавление элемента в конец очереди
    void push(T val) {
        Node* new_node = new Node(val);
        
        if (tail == nullptr) {
            // Если очередь пуста, инициализируем оба указателя
            head = tail = new_node;
        } else {
            // Добавляем в конец и обновляем tail
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    // Просмотр всех элементов очереди
    void lookup() const {
        Node* current = head;
        int counter = 1;
        
        while (current != nullptr) {
            std::cout << counter << " : " << current->value << std::endl;
            current = current->next;
            ++counter;
        }
    }
    
    // Извлечение элемента из начала очереди
    T pop() {
        if (empty()) {
            throw std::runtime_error("QUEUE EMPTY");
        }
        
        T result = head->value;
        Node* temp = head;
        head = head->next;
        
        if (head == nullptr) {
            tail = nullptr; // Если очередь стала пустой
        }
        
        delete temp;
        return result;
    }
    
    // Получение первого элемента без извлечения
    T front() const {
        if (empty()) {
            throw std::runtime_error("QUEUE EMPTY");
        }
        return head->value;
    }
    
    // Проверка на пустоту (C++11: constexpr для возможности вычисления на этапе компиляции)
    constexpr bool empty() const {
        return head == nullptr;
    }
};

int main() {
    // Демонстрация работы очереди
    queue<int> Q;
    
    // Добавление элементов
    Q.push(5);
    Q.push(10);
    Q.push(15);
    
    std::cout << "Queue contents:" << std::endl;
    Q.lookup();
    
    // Операции с очередью
    std::cout << "* FRONT (" << Q.front() << ")\n";
    std::cout << "* POP  (" << Q.pop() << ")\n";
    std::cout << "* POP  (" << Q.pop() << ")\n";
    std::cout << "* FRONT (" << Q.front() << ")\n";
    
    // Попытка извлечения из пустой очереди
    try {
        queue<int> empty_q;
        empty_q.pop();
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}