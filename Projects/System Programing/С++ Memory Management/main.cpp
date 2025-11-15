#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

// Глобальные переменные для управления памятью
unsigned int index_count;  // Счетчик для идентификаторов блоков

// Структура, представляющая блок памяти
struct MemoryBlock {
    unsigned int index;      // Уникальный идентификатор блока
    std::unique_ptr<char[]> data;  // Умный указатель на данные (C++11)
    unsigned int block_begin; // Начальная позиция блока
    unsigned int block_end;   // Конечная позиция блока
    unsigned int size;        // Размер данных
    std::unique_ptr<MemoryBlock> next; // Умный указатель на следующий блок (C++11)
    
    // Конструктор для удобного создания блоков
    MemoryBlock(unsigned int idx, std::unique_ptr<char[]>&& d, 
                unsigned int begin, unsigned int end, unsigned int sz)
        : index(idx), data(std::move(d)), block_begin(begin), 
          block_end(end), size(sz), next(nullptr) {}
};

// Глобальные указатели на хранилище
std::unique_ptr<MemoryBlock> storage;      // Текущий блок
std::unique_ptr<MemoryBlock> storage_begin; // Начальный блок

// Функция для печати всех блоков памяти
void print_blocks(const std::unique_ptr<MemoryBlock>& storage_ext) {
    const MemoryBlock* current = storage_ext.get();
    while (current != nullptr) {
        // Создаем буфер для отображения первых 9 символов данных
        char buffer[10] = {0};
        if (current->size > 0 && current->data != nullptr) {
            strncpy(buffer, current->data.get(), std::min(current->size, 9u));
            buffer[9] = '\0';
        }
        
        printf("%d | %s | %d | %d\n", 
               current->index, buffer, 
               current->block_begin, current->block_end);
        
        current = current->next.get();
    }
}

// Функция для выделения нового блока памяти
int take_memory(unsigned int size) {
    // Создаем данные для блока
    std::unique_ptr<char[]> data(new char[size + 1]);
    char temp = 'a';
    
    // Заполняем данные последовательными символами
    for (unsigned int i = 0; i < size; i++) {
        if (temp > 'z') {
            temp = 'a';
        }
        data[i] = temp++;
    }
    data[size] = '\0';
    
    // Специальный случай для размера 1
    if (size == 1) {
        data[0] = 'a';
        data[1] = '\0';
    }

    // Если это первый блок
    if (index_count == 0) {
        storage = std::make_unique<MemoryBlock>(0, std::move(data), 0, size - 1, size);
        storage_begin = std::move(storage);
        index_count++;
        return storage_begin->block_begin;
    } 
    else {
        // Находим последний блок в цепочке
        MemoryBlock* current = storage_begin.get();
        while (current->next != nullptr) {
            current = current->next.get();
        }
        
        // Создаем новый блок
        unsigned int begin = current->block_end + 1;
        unsigned int end = begin + size - 1;
        current->next = std::make_unique<MemoryBlock>(
            index_count, std::move(data), begin, end, size);
        
        index_count++;
        return current->next->block_begin;
    }
}

// Функция для освобождения блока памяти
void remove_memory(unsigned int block_begin) {
    MemoryBlock* current = storage_begin.get();
    
    while (current != nullptr) {
        if (current->block_begin == block_begin) {
            // Освобождаем данные и обнуляем размер
            current->data.reset();
            current->size = 0;
            break;
        }
        current = current->next.get();
    }
}

// Функция для поиска памяти методом наименее подходящего
int choose_memory(unsigned int size) {
    unsigned int max_hole_size = 0;
    unsigned int best_start = 0;
    
    MemoryBlock* current = storage_begin.get();
    
    // Проходим по всем блокам для поиска подходящей "дыры"
    while (current != nullptr) {
        if (current->size == 0) { // Найден свободный блок
            unsigned int hole_size = current->block_end - current->block_begin + 1;
            
            if (hole_size > max_hole_size) {
                max_hole_size = hole_size;
                best_start = current->block_begin;
            }
        }
        current = current->next.get();
    }
    
    printf("Largest hole: %d, starting at: %d\n", max_hole_size, best_start);
    
    // Если нашли достаточно большую дыру
    if (max_hole_size >= size) {
        current = storage_begin.get();
        
        while (current != nullptr) {
            if (current->block_begin == best_start && current->size == 0) {
                // Занимаем часть или всю дыру
                if (max_hole_size == size) {
                    // Занимаем всю дыру
                    current->data = std::make_unique<char[]>(size + 1);
                    char temp_char = 'A';
                    
                    for (unsigned int i = 0; i < size; i++) {
                        current->data[i] = temp_char;
                        temp_char = (temp_char == 'Z') ? 'A' : temp_char + 1;
                    }
                    current->data[size] = '\0';
                    current->size = size;
                } 
                else {
                    // Занимаем часть дыры, создаем новый блок для оставшейся части
                    current->data = std::make_unique<char[]>(size + 1);
                    char temp_char = 'A';
                    
                    for (unsigned int i = 0; i < size; i++) {
                        current->data[i] = temp_char;
                        temp_char = (temp_char == 'Z') ? 'A' : temp_char + 1;
                    }
                    current->data[size] = '\0';
                    
                    // Создаем новый блок для оставшегося свободного пространства
                    unsigned int new_block_begin = current->block_begin + size;
                    unsigned int new_block_end = current->block_end;
                    
                    current->block_end = current->block_begin + size - 1;
                    current->size = size;
                    
                    // Вставляем новый свободный блок
                    auto new_block = std::make_unique<MemoryBlock>(
                        index_count++, nullptr, 
                        new_block_begin, new_block_end, 0);
                    new_block->next = std::move(current->next);
                    current->next = std::move(new_block);
                }
                return current->block_begin;
            }
            current = current->next.get();
        }
    } 
    else {
        printf("Not enough memory. Required: %d, Largest available: %d\n", 
               size, max_hole_size);
        return -1;
    }
    
    return -1;
}

// Основная функция для демонстрации работы
int main() {
    index_count = 0;
    storage = nullptr;
    storage_begin = nullptr;
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Заполнение памяти случайными блоками
    std::vector<unsigned int> bounds;
    for (unsigned int i = 0; i < 15; i++) {
        unsigned int size = 1 + std::rand() % 100;
        bounds.push_back(take_memory(size));
    }
    
    printf("Initial memory blocks:\n");
    print_blocks(storage_begin);
    printf("\nBlock beginnings:\n");
    
    for (unsigned int bound : bounds) {
        printf("%d ", bound);
    }
    printf("\n\n");
    
    // Освобождение некоторых блоков
    for (unsigned int i = 0; i < 15; i += 3) {
        remove_memory(bounds[i]);
    }
    for (unsigned int i = 0; i < 15; i += 4) {
        remove_memory(bounds[i]);
    }
    
    printf("Memory after removal:\n");
    print_blocks(storage_begin);
    printf("\n");
    
    // Попытка занять память методом наименее подходящего
    int result = choose_memory(50);
    if (result != -1) {
        printf("Successfully allocated memory at: %d\n", result);
    }
    
    printf("\nFinal memory state:\n");
    print_blocks(storage_begin);
    
    return 0;
}