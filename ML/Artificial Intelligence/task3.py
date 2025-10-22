import matplotlib.pyplot as plt
import numpy as np
import math
import copy

variant = 9
np.random.seed(variant)
c_const = np.random.uniform(0.5, 1.5)
steps_count = 100

class Task:
    def __init__(self):
        #   1. Исходные данные
        self.x1_first = np.random.uniform(0, 25, 9)
        self.x2_first = np.random.uniform(0, 25, 9)
        self.y_first = np.zeros(9)
        
        #   2. Мутация
        # [0] = 4, [1] = 5, [2] = 6, [3] = 7, [4] = 8, [5] = 9
        self.x1_rand = np.random.uniform(0, 1, (6, steps_count))
        self.x1_mut_cf = np.random.uniform(0.4, 0.7)
        self.x2_rand = np.random.uniform(0, 1, (6, steps_count))
        self.x2_mut_cf = np.random.uniform(0.4, 0.7)
        
        #   3. Хромосомы потомков, сформированных из родительских хромосом
        # [0] = 1, [1] = 2, [2] = 3, [3] = 4, [4] = 5, [5] = 6, [6] = 7, [7] = 8, [8] = 9
        self.x1 = np.zeros((9, steps_count))
        self.x2 = np.zeros((9, steps_count))
        self.y = np.zeros((9, steps_count))
        self.r = np.zeros((9, steps_count))
        self.x1_class = np.zeros((3, steps_count))
        self.x2_class = np.zeros((3, steps_count))
        self.y_class = np.zeros((3, steps_count))

        #   Лист 2
        self.a_j = np.random.uniform(10, 100, 3)
        self.b_j1 = np.random.uniform(0.01, 0.1, 3)
        self.b_j2 = np.random.uniform(0.01, 0.1, 3)
        self.c_j1 = np.random.uniform(0, 25, 3)
        self.c_j2 = np.random.uniform(0, 25, 3)
    # RANK.EQ
    def rank_y(self, row, ind):
        arr = np.zeros(9)
        for i in range(0, 9):
            arr[i] = self.y[i][row]
        val = arr[ind]
        arr = np.sort(arr)
        arr = arr[::-1]
        for i in range(0, 9):
            if arr[i] == val:
                return i
        return 0
    #   Первая итерация листа
    def solve_list_iter1(self, is_list2 = False):
        #   Отличается по листу
        if not is_list2:
            for i in range(0, 9):
                self.y_first[i] = self.x1_first[i] / ((pow(self.x1_first[i], 2)) + (2 * pow(self.x2_first[i], 2)) + c_const)
        else:
            for i in range(0, 9):
                self.y_first[i] = self.a_j[0] * math.exp(
                        -self.b_j1[0]*math.pow((self.x1_first[i]-self.c_j1[0]), 2)
                        -self.b_j2[0]*math.pow(self.x2_first[i]-self.c_j2[0], 2)
                    ) + self.a_j[1] * math.exp(
                        -self.b_j1[1]*math.pow(self.x1_first[i]-self.c_j1[1], 2)
                        -self.b_j2[1]*math.pow(self.x2_first[i]-self.c_j2[1], 2)
                    ) + self.a_j[2] * math.exp(
                        -self.b_j1[2]*math.pow(self.x1_first[i]-self.c_j1[2], 2)
                        -self.b_j2[2]*math.pow(self.x2_first[i]-self.c_j2[2], 2)
                )
        
        #   Первая итерация
        for i in range(0, 9):
            self.x1[i][0] = self.x1_first[i]
            self.x2[i][0] = self.x2_first[i]
            self.y[i][0] = self.y_first[i]
        for i in range(0, 9):
            self.r[i][0] = self.rank_y(0, i)
        for j in range(0, 3):
            # x1
            for i in range(0, 9):
                if self.r[i][0] == j:
                    self.x1_class[j][0] = self.x1[i][0]
            # x2
            for i in range(0, 9):
                if self.r[i][0] == j:
                    self.x2_class[j][0] = self.x2[i][0]
            # y
            for i in range(0, 9):
                if self.r[i][0] == j:
                    self.y_class[j][0] = self.y[i][0]
    #   Все последующие итерации
    def solve_list_iter_all(self, is_list2 = False):
        for i in range(1, steps_count):
            # x1
            for j in range(0, 3):
                self.x1[j][i] = self.x1_class[j][i-1]
            for j in range(3, 6):
                self.x1[j][i] = self.x1_class[j%3][i-1] + (self.x1_rand[j-3][i]*self.x1_mut_cf)
            for j in range(6, 9):
                self.x1[j][i] = self.x1_class[j%3][i-1] + (self.x1_rand[j-3][i]*self.x2_mut_cf)
            # x2
            for j in range(0, 3):
                self.x2[j][i] = self.x2_class[j][i-1]
            for j in range(3, 6):
                self.x2[j][i] = self.x2_class[j%3][i-1] + (self.x2_rand[j-3][i]*self.x1_mut_cf)
            for j in range(6, 9):
                self.x2[j][i] = self.x2_class[j%3][i-1] + (self.x2_rand[j-3][i]*self.x2_mut_cf)
            # y
            if not is_list2:
                for j in range(0, 9):
                    self.y[j][i] = (self.x1[j][i-1]) / (pow(self.x1[j][i-1], 2) + (2 * pow(self.x2[j][i-1], 2) + c_const))
            else:
                for j in range(0, 9):
                    self.y[j][i] = self.a_j[0] * math.exp(
                            -self.b_j1[0]*math.pow((self.x1[j][i-1]-self.c_j1[0]), 2)
                            -self.b_j2[0]*math.pow(self.x2[j][i-1]-self.c_j2[0], 2)
                        ) + self.a_j[1] * math.exp(
                            -self.b_j1[1]*math.pow(self.x1[j][i-1]-self.c_j1[1], 2)
                            -self.b_j2[1]*math.pow(self.x2[j][i-1]-self.c_j2[1], 2)
                        ) + self.a_j[2] * math.exp(
                            -self.b_j1[2]*math.pow(self.x1[j][i-1]-self.c_j1[2], 2)
                            -self.b_j2[2]*math.pow(self.x2[j][i-1]-self.c_j2[2], 2)
                    )
            # r
            for j in range(0, 9):
                self.r[j][i] = self.rank_y(i, j)
            for j in range(0, 3):
                # x1
                for k in range(0, 9):
                    if self.r[k][i] == j:
                        self.x1_class[j][i] = self.x1[k][i]
                # x2
                for k in range(0, 9):
                    if self.r[k][i] == j:
                        self.x2_class[j][i] = self.x2[k][i]
                # y
                for k in range(0, 9):
                    if self.r[k][i] == j:
                        self.y_class[j][i] = self.y[k][i]

if __name__ == "__main__":
    t1 = Task()
    t2 = copy.deepcopy(t1)
    t1.solve_list_iter1()
    t1.solve_list_iter_all()
    t2.solve_list_iter1(True)
    t2.solve_list_iter_all(True)

    figure, axis = plt.subplots(2, 3)
    figure.set_figheight(7)
    figure.set_figwidth(14)

    axis[0, 0].scatter(np.arange(steps_count), t1.y_class[0], s=15, label='y (a)')
    axis[0, 0].plot(np.arange(steps_count), t1.y_class[0])
    axis[0, 0].scatter(np.arange(steps_count), t1.y_class[1], s=15, label='y (b)')
    axis[0, 0].plot(np.arange(steps_count), t1.y_class[1])
    axis[0, 0].scatter(np.arange(steps_count), t1.y_class[2], s=15, label='y (c)')
    axis[0, 0].plot(np.arange(steps_count), t1.y_class[2])
    axis[0, 0].set_title('1: Сходимость значений целевого показателя')
    axis[0, 0].legend()

    axis[0, 1].scatter(np.arange(steps_count), t1.x1_class[0], s=15, label='x1')
    axis[0, 1].plot(np.arange(steps_count), t1.x1_class[0])
    axis[0, 1].scatter(np.arange(steps_count), t1.x2_class[0], s=15, label='x2')
    axis[0, 1].plot(np.arange(steps_count), t1.x2_class[0])
    axis[0, 1].set_title('1: Сходимость значений целевого показателя')
    axis[0, 1].legend()

    axis[0, 2].scatter(t1.x1_class[0], t1.x2_class[0], s=15, label='y (a)')
    axis[0, 2].scatter(t1.x1_class[1], t1.x2_class[1], s=15, label='y (b)')
    axis[0, 2].scatter(t1.x1_class[2], t1.x2_class[2], s=15, label='y (c)')
    axis[0, 2].set_title('1: Сходимость к решению по координатам')
    axis[0, 2].legend()

    axis[1, 0].scatter(np.arange(steps_count), t2.y_class[0], s=15, label='y (a)')
    axis[1, 0].plot(np.arange(steps_count), t2.y_class[0])
    axis[1, 0].scatter(np.arange(steps_count), t2.y_class[1], s=15, label='y (b)')
    axis[1, 0].plot(np.arange(steps_count), t2.y_class[1])
    axis[1, 0].scatter(np.arange(steps_count), t2.y_class[2], s=15, label='y (c)')
    axis[1, 0].plot(np.arange(steps_count), t2.y_class[2])
    axis[1, 0].set_title('2: Сходимость значений целевого показателя')
    axis[1, 0].legend()

    axis[1, 1].scatter(np.arange(steps_count), t2.x1_class[0], s=15, label='x1')
    axis[1, 1].plot(np.arange(steps_count), t2.x1_class[0])
    axis[1, 1].scatter(np.arange(steps_count), t2.x2_class[0], s=15, label='x2')
    axis[1, 1].plot(np.arange(steps_count), t2.x2_class[0])
    axis[1, 1].set_title('2: Сходимость значений целевого показателя')
    axis[1, 1].legend()

    axis[1, 2].scatter(t2.x1_class[0], t2.x2_class[0], s=15, label='y (a)')
    axis[1, 2].scatter(t2.x1_class[1], t2.x2_class[1], s=15, label='y (b)')
    axis[1, 2].scatter(t2.x1_class[2], t2.x2_class[2], s=15, label='y (c)')
    axis[1, 2].set_title('2: Сходимость к решению по координатам')
    axis[1, 2].legend()
    
    plt.tight_layout()
    plt.show()