#include <iostream>

class Table {
private:
    struct Node {
        int key = 0;           // Значение узла
        Node* next = nullptr;  // Указатель на следующий узел
        Node* prev = nullptr;  // Указатель на предыдущий узел
        bool is_empty = false; // Флаг пустого узла
    };
    
    Node* head = nullptr;      // Указатель на начало списка
    Node* tail = nullptr;      // Указатель на конец списка

public:
    Table() {
        head = new Node();
        head->is_empty = true;
        tail = head;  // В списке один узел
    }
    
    Table(int node_count) {
        head = new Node();
        head->is_empty = true;
        tail = head;
        createNodes(node_count);  // Создаем указанное количество узлов
    }
    
    Table(int node_count, int default_key) {
        head = new Node();
        head->is_empty = true;
        tail = head;
        createNodes(node_count); // Создаем указанное количество узлов
        fill(default_key);  // Заполняем все узлы значением
    }
    
    ~Table() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    
    Table(const Table&) = delete;
    Table& operator=(const Table&) = delete;

    void show() {
        if (head == nullptr || head->is_empty) {
            std::cout << "Empty\n";
            return;
        }
        
        Node* current = head;
        int index = 0;
        
        // Проходим по всем узлам списка
        while (current != nullptr) {
            // Выводим только не-пустые узлы
            if (!current->is_empty)
            {
                std::cout << index << ": " << current->key << "\n";
                index++;
            }
            current = current->next;
        }
    }
    
    // Создание указанного количества новых узлов
    void createNodes(int count) {
        if (count <= 0) return;
        
        Node* current = tail;
        
        // Создаем новые узлы и связываем их
        for (int i = 0; i < count; i++) {
            Node* new_node = new Node();
            current->next = new_node;
            new_node->prev = current;
            current = new_node;
        }
        tail = current;  // Обновляем хвост списка
    }
    
    // Заполнение всех узлов (кроме пустых) указанным значением
    void fill(int value) {
        Node* current = head;
        
        while (current != nullptr) {
            // Заполняем только не-пустые узлы
            if (!current->is_empty)
            	current->key = value;
            current = current->next;
        }
    }
};

int main() {
    // Создаем три разных списка для демонстрации
    Table empty_table;           // Только один пустой узел
    Table sized_table(10);       // Пустой узел + 10 узлов с key=0
    Table filled_table(5, 20);   // Пустой узел + 5 узлов с key=20
    
    std::cout << "Empty table:\n";
    empty_table.show();
    
    std::cout << "\nSized table (10 nodes):\n";
    sized_table.show();
    
    std::cout << "\nFilled table (5 nodes with value 20):\n";
    filled_table.show();
    
    return 0;
}