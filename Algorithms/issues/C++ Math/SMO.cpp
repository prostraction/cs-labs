#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <vector>

class RandomGenerator {
private:
    std::mt19937 generator; // В C++11: Mersenne Twister - качественный генератор
    std::uniform_real_distribution<double> uniform_dist;
    
public:
    RandomGenerator() : 
        generator(std::chrono::steady_clock::now().time_since_epoch().count()),
        uniform_dist(0.0, 1.0) {}
    
    // Генерация равномерно распределенного числа в [0,1]
    double generate_uniform() {
        return uniform_dist(generator);
    }
    
    // Генерация пуассоновской случайной величины (метод обратных преобразований)
    int generate_poisson(double lambda) {
        int x = 0;
        double p = std::exp(-lambda);
        double s = p;
        double u = generate_uniform();
        
        while (u > s) {
            x++;
            p = p * lambda / x; // Рекуррентная формула для вероятностей Пуассона
            s += p;
        }
        return x;
    }
};

// Моделирование пуассоновского процесса (альтернативный метод)
int simulate_poisson_process(double lambda, RandomGenerator& rg) {
    int x = 0;
    double p = std::exp(-lambda);
    double s = 1.0;
    
    do {
        x++;
        double u = rg.generate_uniform();
        s = s * u;
    } while (s >= p);
    
    return x - 1;
}

// Основная функция моделирования
void run_simulation() {
    const int N = 1000; // Количество экспериментов
    const double e = std::exp(1.0); // Константа e из C++ standard library
    
    // Параметры пуассоновских распределений
    double lambda_input = 4.6;  // λ для входного потока
    double lambda_output1 = 3.2; // λ для первого оператора
    double lambda_output2 = 2.6; // λ для второго оператора
    
    RandomGenerator rg; // Единый генератор для всех случайных величин
    
    // Векторы для хранения данных (в C++11: uniform initialization)
    std::vector<double> input(N, 0.0);
    std::vector<double> output1(N, 0.0);
    std::vector<double> output2(N, 0.0);
    
    // Моделирование входного потока
    double sum_input = 0.0;
    for (int i = 0; i < N; i++) {
        input[i] = simulate_poisson_process(lambda_input, rg);
        sum_input += input[i];
    }
    std::cout << "Среднее входного потока: " << sum_input / N << std::endl;
    
    // Моделирование выходных потоков операторов
    double sum_output1 = 0.0;
    double sum_output2 = 0.0;
    for (int i = 0; i < N; i++) {
        output1[i] = rg.generate_poisson(lambda_output1);
        output2[i] = rg.generate_poisson(lambda_output2);
        sum_output1 += output1[i];
        sum_output2 += output2[i];
    }
    
    std::cout << "Среднее первого оператора: " << sum_output1 / N << std::endl;
    std::cout << "Среднее второго оператора: " << sum_output2 / N << std::endl;
    
    // Построение гистограммы входного потока
    std::vector<int> histogram(10, 0);
    for (int i = 0; i < N; i++) {
        if (input[i] < 10) {
            histogram[static_cast<int>(input[i])]++;
        }
    }
    
    std::cout << "\nГистограмма входного потока:" << std::endl;
    double hist_sum = 0.0;
    for (size_t i = 0; i < histogram.size(); i++) {
        std::cout << i << ": " << histogram[i] << std::endl;
        hist_sum += static_cast<double>(histogram[i]) / N;
    }
    std::cout << "Сумма вероятностей: " << std::fixed << hist_sum << std::endl;
    
    // Моделирование состояний системы
    std::vector<double> S0(N, 0.0); // Вероятность занятости оператора 1
    std::vector<double> S1(N, 0.0); // Вероятность свободы оператора 1  
    std::vector<double> S2(N, 0.0); // Вероятность занятости оператора 2
    std::vector<double> S3(N, 0.0); // Вероятность свободы оператора 2
    
    double sumS0 = 0.0, sumS1 = 0.0, sumS2 = 0.0, sumS3 = 0.0;
    double total_queue = 0.0;
    
    for (int i = 0; i < N; i++) {
        // Задача может быть обработана
        if (input[i] <= (output1[i] + output2[i])) {
            if (output1[i] != 0) {
                S0[i] = input[i] / output1[i];
                S1[i] = 1.0 - S0[i];
                S3[i] = 1.0;
            } else if (output2[i] != 0) {
                S2[i] = input[i] / output2[i];
                S3[i] = 1.0 - S2[i];
                S1[i] = 1.0;
            } else {
                S0[i] = S2[i] = 0.0;
                S1[i] = S3[i] = 1.0;
            }
            
            // Распределение задачи между операторами
            if (S0[i] >= 1.0 && output2[i] > 0) {
                if (output2[i] >= input[i]) {
                    S2[i] = input[i] / output2[i];
                    S3[i] = 1.0 - S2[i];
                    S0[i] = 0.0;
                    S1[i] = 1.0;
                } else {
                    S0[i] = std::abs(input[i] - output2[i]) / output1[i];
                    S1[i] = 1.0 - S0[i];
                    S2[i] = std::abs(input[i] - output1[i]) / output2[i];
                    S3[i] = 1.0 - S2[i];
                }
            }
        } 
        // Задача не может быть обработана - формируется очередь
        else {
            S0[i] = S2[i] = 1.0;
            S1[i] = S3[i] = 0.0;
            double queue = input[i] - (output1[i] + output2[i]);
            if (i < N - 1) {
                input[i + 1] += queue; // Перенос очереди на следующий шаг
            }
            total_queue += queue;
        }
        
        sumS0 += S0[i];
        sumS1 += S1[i];
        sumS2 += S2[i];
        sumS3 += S3[i];
    }
    
    // Вывод детальной информации (первые 20 записей)
    std::cout << "\nДетализация первых 20 экспериментов:" << std::endl;
    int display_count = std::min(20, N);
    for (int i = 0; i < display_count; i++) {
        std::cout << std::setprecision(3) 
                  << "input: " << input[i] << std::setw(10)
                  << "out1: " << output1[i] << std::setw(10) 
                  << "out2: " << output2[i] << std::setw(8)
                  << "S0: " << S0[i] << std::setw(8)
                  << "S1: " << S1[i] << std::setw(8)
                  << "S2: " << S2[i] << std::setw(8)
                  << "S3: " << S3[i] << std::endl;
    }
    
    // Теоретические вероятности состояний
    double p0 = lambda_input / (lambda_output1 + lambda_output2);
    double p = (1.0 - p0) / (1.0 + lambda_output1 / lambda_output2);
    double p2 = (1.0 - p) / (1.0 + lambda_input / lambda_output2) * lambda_input / lambda_output2;
    
    // Вывод результатов
    std::cout << "\n=== РЕЗУЛЬТАТЫ МОДЕЛИРОВАНИЯ ===" << std::endl;
    std::cout << "Средняя вероятность занятости оператора 1: " << sumS0 / N 
              << " (теоретическая: " << p0 << ")" << std::endl;
    std::cout << "Средняя вероятность свободы оператора 1: " << sumS1 / N 
              << " (теоретическая: " << 1.0 - p0 << ")" << std::endl;
    std::cout << "Средняя вероятность занятости оператора 2: " << sumS2 / N 
              << " (теоретическая: " << p2 << ")" << std::endl;
    std::cout << "Средняя вероятность свободы оператора 2: " << sumS3 / N 
              << " (теоретическая: " << 1.0 - p2 << ")" << std::endl;
    std::cout << "Средняя длина очереди: " << total_queue / N << std::endl;
}

int main() {
    try {
        run_simulation();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
}