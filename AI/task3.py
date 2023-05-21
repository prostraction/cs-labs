import matplotlib.pyplot as plt
import numpy as np

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
    def solve(self):
        #   Отличается по листу
        for i in range(0, 9):
            self.y_first[i] = self.x1_first[i] / ((pow(self.x1_first[i], 2))+(2*pow(self.x2_first[i], 2))+c_const)
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
            # x3
            for i in range(0, 9):
                if self.r[i][0] == j:
                    self.y_class[j][0] = self.y[i][0]
        #   Все последующие итерации
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
            # x3
            for j in range(0, 9):
                self.y[j][i] = (self.x1[j][i-1])/(pow(self.x1[j][i-1], 2) + (2 * pow(self.x2[j][i-1], 2) + c_const))
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
                # x3
                for k in range(0, 9):
                    if self.r[k][i] == j:
                        self.y_class[j][i] = self.y[k][i]

if __name__ == "__main__":
    t = Task()
    t.solve()
    figure, axis = plt.subplots(2, 3)

    axis[0, 0].scatter(np.arange(steps_count), t.y_class[0], s=15)
    axis[0, 0].plot(np.arange(steps_count), t.y_class[0])
    axis[0, 0].scatter(np.arange(steps_count), t.y_class[1], s=15)
    axis[0, 0].plot(np.arange(steps_count), t.y_class[1])
    axis[0, 0].scatter(np.arange(steps_count), t.y_class[2], s=15)
    axis[0, 0].plot(np.arange(steps_count), t.y_class[2])

    axis[0, 1].scatter(np.arange(steps_count), t.x1_class[0], s=15)
    axis[0, 1].plot(np.arange(steps_count), t.x1_class[0])
    axis[0, 1].scatter(np.arange(steps_count), t.x2_class[0], s=15)
    axis[0, 1].plot(np.arange(steps_count), t.x2_class[0])

    axis[0, 2].scatter(t.x1_class[0], t.x2_class[0], s=15)
    axis[0, 2].scatter(t.x1_class[1], t.x2_class[1], s=15)
    axis[0, 2].scatter(t.x1_class[2], t.x2_class[2], s=15)

    #axis[1, 0].scatter(np.arange(steps_count), t.y_class[0], s=15)
    #axis[1, 0].plot(np.arange(steps_count), t.y_class[0])
    #axis[1, 0].scatter(np.arange(steps_count), t.y_class[1], s=15)
    #axis[1, 0].plot(np.arange(steps_count), t.y_class[1])
    #axis[1, 0].scatter(np.arange(steps_count), t.y_class[2], s=15)
    #axis[1, 0].plot(np.arange(steps_count), t.y_class[2])
#
    #axis[1, 1].scatter(np.arange(steps_count), t.x1_class[0], s=15)
    #axis[1, 1].plot(np.arange(steps_count), t.x1_class[0])
    #axis[1, 1].scatter(np.arange(steps_count), t.x2_class[0], s=15)
    #axis[1, 1].plot(np.arange(steps_count), t.x2_class[0])
#
    #axis[1, 2].scatter(t.x1_class[0], t.x2_class[0], s=15)
    #axis[1, 2].scatter(t.x1_class[1], t.x2_class[1], s=15)
    #axis[1, 2].scatter(t.x1_class[2], t.x2_class[2], s=15)
    
    plt.show()