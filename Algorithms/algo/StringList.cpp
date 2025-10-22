#include <iostream>
#include <cstring>

class String
{
private:
    size_t length = 0;
    struct Node
    {
        size_t length = 0;
        char* word = nullptr;
        Node* next = nullptr;
        Node* prev = nullptr;
        ~Node() {
            delete[] word;
        }
    };
    
    Node* head = nullptr;

public:
    String() = default;
    String(const char* data) {
        if (data) add(data);
    }
    ~String() {
        clear();
    }
    String(const String&) = delete;
    String& operator=(const String&) = delete;

    // Добавление строки
    void add(const char* data) {
        if (!data || !*data) return; // Проверка пустой строки
        
        const char* start = data;
        const char* end = data;
        
        while (*end) {
            if (*end == ' ') {
                if (start != end) { // Игнорируем множественные пробелы
                    addWord(start, end - start);
                }
                start = end + 1;
            }
            ++end;
        }
        
        // Добавляем последнее слово
        if (start != end) {
            addWord(start, end - start);
        }
    }

    void show() const {
        Node* current = head;
        while (current) {
            std::cout << current->word;
            if (current->next) std::cout << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Ввод от пользователя
    void user_defined() {
        std::cout << "Enter string (up to 100 chars): ";
        char buffer[101];
        std::cin.getline(buffer, 101);
        add(buffer);
    }

    void del_by_index(size_t idx) {
        if (idx == 0 || !head) {
            std::cout << "Invalid index" << std::endl;
            return;
        }

        Node* current = head;
        for (size_t i = 1; i < idx; ++i) {
            if (!current->next) {
                std::cout << "Index out of range" << std::endl;
                return;
            }
            current = current->next;
        }
        removeNode(current);
    }

    void del_by_word(const char* target) {
        if (!target) return;
        
        Node* current = head;
        while (current) {
            if (std::strcmp(current->word, target) == 0) {
                removeNode(current);
                return;
            }
            current = current->next;
        }
        std::cout << "Word not found" << std::endl;
    }

private:
    void addWord(const char* start, size_t len) {
        Node* newNode = new Node();
        newNode->length = len;
        newNode->word = new char[len + 1];
        std::strncpy(newNode->word, start, len);
        newNode->word[len] = '\0';

        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
            newNode->prev = current;
        }
        
        length += len + 1; // +1 для пробела
    }

    void removeNode(Node* node) {
        if (!node) return;

        // Корректируем связи
        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        
        if (node->next) {
            node->next->prev = node->prev;
        }

        // Корректируем длину
        length -= node->length + 1;

        // Удаляем узел (деструктор Node очистит word)
        delete node;
    }

    void clear() {
        while (head) {
            Node* next = head->next;
            delete head;
            head = next;
        }
        length = 0;
    }
};