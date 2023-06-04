import matplotlib.pyplot as plt
import numpy as np
import copy


class Task:
    def __init__(self):
        self.variant = 18
        np.random.seed(self.variant)
        self.item_size = 100

        self.x = np.arange(1, self.item_size + 1)
        self.p_a = np.random.uniform(0, 1)
        self.p_a_arr = np.full(self.item_size, self.p_a)
        self.noise = np.random.uniform(0, 1, self.item_size)
        self.event = np.where(self.noise < self.p_a, 1, 0)
        self.accumulation = copy.copy(self.event)
        for i in range(1, self.item_size):
            self.accumulation[i] = self.accumulation[i - 1] + self.event[i]
        self.quotient = self.accumulation / self.x
        self.p_i = np.random.uniform(0, 1, 4)
        self.sum_p = np.sum(self.p_i)
        self.p_i = self.p_i / self.sum_p

    def solve(self):
        self.calculate_p_accumulation()
        self.calculate_magic_numbers()
        self.calculate_magic_value()
        self.calculate_F()
        self.calculate_f()

        np.random.seed(self.variant * 2)
        self.generate_p_i()
        self.generate_p_matrix()
        self.calculate_inter_matrix()
        self.generate_noise()
        self.calculate_state()
        self.calculate_F2()
        self.calculate_f2()

    def calculate_p_accumulation(self):
        self.p_accumulation = copy.copy(self.p_i)
        for i in range(1, 4):
            self.p_accumulation[i] = self.p_accumulation[i] + self.p_accumulation[i - 1]

    def calculate_magic_numbers(self):
        self.magic_number = np.arange(0, 4)
        self.magic_number[0] = self.variant * 100 + 23
        self.magic_number[1] = self.magic_number[0] + self.magic_number[0] / 100
        self.magic_number[2] = self.magic_number[1] + self.magic_number[0] % 100 / 10
        self.magic_number[3] = self.magic_number[2] + self.magic_number[0] % 10

    def calculate_magic_value(self):
        self.magic_value = np.zeros(self.item_size)
        for i in range(self.item_size):
            for j in range(4):
                if self.noise[i] <= self.p_accumulation[j]:
                    self.magic_value[i] = self.magic_number[j]
                    break

    def calculate_F(self):
        self.F = []
        for i in range(4):
            self.F.append(np.zeros(self.item_size, dtype=int))

        for i in range(4):
            if self.magic_value[0] == self.magic_number[i]:
                self.F[i][0] = 1

        for i in range(4):
            for j in range(1, self.item_size):
                if self.magic_value[j] == self.magic_number[i]:
                    self.F[i][j] = self.F[i][j - 1] + 1
                else:
                    self.F[i][j] = self.F[i][j - 1]

    def calculate_f(self):
        self.f = []
        for i in range(4):
            self.f.append(np.zeros(self.item_size))

        for i in range(4):
            for j in range(self.item_size):
                self.f[i][j] = self.F[i][j] / self.x[j]

    def generate_p_i(self):
        self.p_i = np.random.uniform(0, 1, 3)
        self.sum_p = np.sum(self.p_i)
        self.p_i = self.p_i / self.sum_p

    def generate_p_matrix(self):
        self.p_matrix = []
        for i in range(3):
            self.p_matrix.append(np.random.uniform(0, 1, 3))
        for i in range(3):
            self.sum_p = np.sum(self.p_matrix[i])
            self.p_matrix[i] = self.p_matrix[i] / self.sum_p

    def calculate_inter_matrix(self):
        self.inter_matrix = copy.deepcopy(self.p_matrix)
        for i in range(0, 3):
            self.inter_matrix[i][1] += self.inter_matrix[i][0]
            self.inter_matrix[i][2] += self.inter_matrix[i][1]

    def generate_noise(self):
        self.noise = np.random.uniform(0, 1, self.item_size + 1)

    def calculate_state(self):
        self.state = np.arange(0, self.item_size + 1)
        for i in range(0, self.item_size + 1):
            if self.noise[i] <= self.p_i[0]:
                self.state[i] = 1
            elif self.noise[i] <= self.p_i[1]:
                self.state[i] = 2
            else:
                self.state[i] = 3

    def calculate_F2(self):
        self.F2 = []
        for i in range(0, 3):
            self.F2.append(np.zeros(self.item_size + 1, dtype=int))

        for i in range(0, 3):
            for j in range(1, self.item_size + 1):
                if self.state[j] == i + 1:
                    self.F2[i][j] = self.F2[i][j - 1] + 1
                else:
                    self.F2[i][j] = self.F2[i][j - 1]

    def calculate_f2(self):
        self.f2 = []
        for i in range(0, 3):
            self.f2.append(np.zeros(self.item_size + 1))

        for i in range(0, 3):
            for j in range(1, self.item_size + 1):
                self.f2[i][j] = self.F2[i][j] / self.x[j - 1]

    def plot(self):
        fig, axis = plt.subplots(2, 3)
        fig.set_figheight(7)
        fig.set_figwidth(14)
        axis[0, 0].plot(self.x, self.event, label="События")
        axis[0, 0].plot(self.x, self.quotient, label="Частота")
        axis[0, 0].plot(self.x, self.p_a_arr, label="P(A)")
        axis[0, 1].plot(self.x, self.magic_value)
        axis[0, 2].plot(self.x, self.f[0], label="f1")
        axis[0, 2].plot(self.x, self.f[1], label="f2")
        axis[0, 2].plot(self.x, self.f[2], label="f3")
        axis[0, 2].plot(self.x, self.f[3], label="f4")
        axis[1, 0].plot(self.x, self.f2[0][1:], label="f1")
        axis[1, 0].plot(self.x, self.f2[1][1:], label="f2")
        axis[1, 0].plot(self.x, self.f2[2][1:], label="f3")
        axis[1, 1].plot(self.x, self.state[1:])

        axis[0, 0].set_title("Равномерное распределение")
        axis[0, 0].legend()
        axis[0, 1].set_title("Случайный процесс")
        axis[0, 2].set_title("Сходимость оценок")
        axis[0, 2].legend()
        axis[1, 0].set_title("Сходимость оценок")
        axis[1, 0].legend()
        axis[1, 1].set_title("Реализация марковской цели")

        plt.show()


if __name__ == "__main__":
    task = Task()
    task.solve()
    task.plot()
