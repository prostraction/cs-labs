#include <iostream>
#include <queue>
#include <algorithm>
#include <stdint.h>
#include <chrono>
#include <sstream>

#include "include/bid.h"
#include "include/stockdeque.h"
#include "include/stockmultiset.h"
#include "include/stockunorderedmap.h"

template <typename T>
class Verify {
    public:
        void AddTest() {
            r_deq.add = std::chrono::duration_cast<std::chrono::milliseconds>(add(s_deq)).count();
            r_set.add = std::chrono::duration_cast<std::chrono::milliseconds>(add(s_set)).count();
            r_map.add = std::chrono::duration_cast<std::chrono::milliseconds>(add(s_map)).count();
        }
        void RemoveTest() {
            r_deq.remove = std::chrono::duration_cast<std::chrono::milliseconds>(remove(s_deq)).count();
            r_set.remove = std::chrono::duration_cast<std::chrono::milliseconds>(remove(s_set)).count();
            r_map.remove = std::chrono::duration_cast<std::chrono::milliseconds>(remove(s_map)).count();
        }
        void EditTest() {
            r_deq.edit = std::chrono::duration_cast<std::chrono::milliseconds>(edit(s_deq)).count();
            r_set.edit = std::chrono::duration_cast<std::chrono::milliseconds>(edit(s_set)).count();
            r_map.edit = std::chrono::duration_cast<std::chrono::milliseconds>(edit(s_map)).count();
        }
        void ShowTopTest() {
            r_deq.print = std::chrono::duration_cast<std::chrono::milliseconds>(print(s_deq, r_deq.print_value)).count();
            r_set.print = std::chrono::duration_cast<std::chrono::milliseconds>(print(s_set, r_set.print_value)).count();
            r_map.print = std::chrono::duration_cast<std::chrono::milliseconds>(print(s_map, r_map.print_value)).count();
            if (r_deq.print_value == r_set.print_value && r_set.print_value == r_map.print_value) {
                is_results_equal = true;
            }
        }

        void PrintResults() {
            std::cout << "*** RESULTS ***\n";
            std::cout << ((is_results_equal) ? "-- Results are EQUAL" : "-- Results are NOT equal") << "\n";
            std::cout << "*** ADD ***\n";
            std::cout << "-- Deque:         " << r_deq.add << " ms\n";
            std::cout << "-- Multiset:      " << r_set.add << " ms\n";
            std::cout << "-- Unordered map: " << r_map.add << " ms\n";
            std::cout << "*** EDIT ***\n";
            std::cout << "-- Deque:         " << r_deq.edit << " ms\n";
            std::cout << "-- Multiset:      " << r_set.edit << " ms\n";
            std::cout << "-- Unordered map: " << r_map.edit << " ms\n";
            std::cout << "*** REMOVE ***\n";
            std::cout << "-- Deque:         " << r_deq.remove << " ms\n";
            std::cout << "-- Multiset:      " << r_set.remove << " ms\n";
            std::cout << "-- Unordered map: " << r_map.remove << " ms\n";
            std::cout << "*** PRINT ***\n";
            std::cout << "-- Deque:         " << r_deq.print << " ms\n";
            std::cout << "-- Multiset:      " << r_set.print << " ms\n";
            std::cout << "-- Unordered map: " << r_map.print << " ms\n";
        }
    private:
        // Заполнение данных
        template <class C>
        std::chrono::duration<float> add(C &s) {
            const auto start = std::chrono::steady_clock::now();
            for (uint64_t i = 1; i <= 10000000; i++) {
                (i % 2 == 0) ?
                    s.Add(Bid<T>{i * 0.001, (i + 1) % 10 + 1, Type::buy}) :
                    s.Add(Bid<T>{i * 0.001, (i + 1) % 10 + 1, Type::sell});
            }
            const auto end = std::chrono::steady_clock::now();
            return end - start;
        }

        // Редактирование данных
        template <class C>
        std::chrono::duration<float> edit(C &s) {
            const auto start = std::chrono::steady_clock::now();
            for (uint64_t i = 10000000/3; i <= 10000000/3 + 32; i++) {
                (i % 2 == 0) ?
                    s.Edit(i, Bid<T>{i * 0.021, (i + 1) % 10 + 2, Type::buy}) :
                    s.Edit(i, Bid<T>{i * 0.021, (i + 1) % 10 + 2, Type::sell});
            }
            const auto end = std::chrono::steady_clock::now();
            return end - start;
        }

        // Удаление данных
        template <class C>
        std::chrono::duration<float> remove(C &s) {
            const auto start = std::chrono::steady_clock::now();
            for (uint64_t i = 10000000/2; i <= 10000000/2 + 32; i++) {
                (i % 2 == 0) ?
                    s.Remove(i, Type::buy) :
                    s.Remove(i, Type::sell);
            }
            const auto end = std::chrono::steady_clock::now();
            return end - start;
        }

        // Печать
        template <class C>
        std::chrono::duration<float> print(C &s, std::string &val) {
            const auto start = std::chrono::steady_clock::now();
            val = s.ShowTop();
            const auto end = std::chrono::steady_clock::now();
            return end - start;
        }

        // chrono::ms -> count() -> results
        struct results {
            int add {-1};
            int edit{-1};
            int remove {-1};
            int print {-1};
            std::string print_value {};
        } r_deq, r_set, r_map;
        bool is_results_equal {false};

        StockDeque<T> s_deq;
        StockMultiset<T> s_set;
        StockUnorderedMap<T> s_map;
};

int main() {
    std::cout << "Started benchmarking, this could take a while. Result will be printed after tests.\n";
    Verify<double> v;
    std::cout << "Add test.\n";
    v.AddTest();
    std::cout << "Edit test.\n";
    v.EditTest();
    std::cout << "Remove test.\n";
    v.RemoveTest();
    std::cout << "Show test.\n";
    v.ShowTopTest();
    v.PrintResults();
}