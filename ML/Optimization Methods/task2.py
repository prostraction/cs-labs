import matplotlib.pyplot as plt
import numpy as np
import copy
import math

variant = 9
np.random.seed(variant)


class Task:
    def __init__(self, steps):
        self.steps = steps
        self.steps_f = 20
        self.c = []
        self.a = []
        self.x_first = []

        self.func_val = []
        self.rand_degrees = []
        self.x1_coord = []
        self.x2_coord = []
        self.min_ind = np.zeros(self.steps)
        self.x1 = np.zeros(self.steps)
        self.x2 = np.zeros(self.steps)
        self.r = 2  # np.random.uniform(1, 3)
        self.R = np.random.uniform(0.2, 0.9)
        self.f = np.zeros(self.steps)
        self.f_350_plus = np.zeros(self.steps_f)
        self.f_350_minus = np.zeros(self.steps_f)
        self.f_200_plus = np.zeros(self.steps_f)
        self.f_200_minus = np.zeros(self.steps_f)
        self.f_50_plus = np.zeros(self.steps_f)
        self.f_50_minus = np.zeros(self.steps_f)

        for _ in range(0, 6):
            self.c.append(np.random.uniform(0.5, 10))
        for _ in range(0, 2):
            self.a.append(np.random.uniform(2, 10))
        for _ in range(0, 2):
            self.x_first.append(np.random.uniform(5, 20))
        for _ in range(0, 10):
            self.rand_degrees.append(np.random.uniform(0, 6.28, steps))
            self.x1_coord.append(np.zeros(steps))
            self.x2_coord.append(np.zeros(steps))
            self.func_val.append(np.zeros(steps))

    #   Лист 1-2-3
    def solve(self):
        for i in range(0, self.steps):
            self.x1[i] = (
                self.x_first[0]
                if i == 0
                else self.x1[i - 1]
                - (self.x1[i - 1] - self.x1_coord[int(self.min_ind[i - 1])][i - 1])
                * self.R
                / self.r
            )
            self.x2[i] = (
                self.x_first[1]
                if i == 0
                else self.x2[i - 1]
                - (self.x2[i - 1] - self.x2_coord[int(self.min_ind[i - 1])][i - 1])
                * self.R
                / self.r
            )
            self.f[i] = self.c[1] * math.pow((self.x1[i] - self.a[0]), 2) + self.c[
                2
            ] * math.pow((self.x2[i] - self.a[1]), 2)
            min_val = 9999999999
            for j in range(0, 10):
                self.x1_coord[j][i] = self.x1[i] + self.r * math.cos(
                    self.rand_degrees[j][i]
                )
                self.x2_coord[j][i] = self.x2[i] + self.r * math.sin(
                    self.rand_degrees[j][i]
                )
                self.func_val[j][i] = self.c[1] * math.pow(
                    (self.x1_coord[j][i] - self.a[0]), 2
                ) + self.c[2] * math.pow((self.x2_coord[j][i] - self.a[1]), 2)
                if self.func_val[j][i] < min_val:
                    min_val = self.func_val[j][i]
                    self.min_ind[i] = j

    # Угол зондирования лист 2
    def prep_list_2(self):
        for i in range(0, self.steps):
            for j in range(0, 10):
                self.rand_degrees[j][i] = 0.628 * (j + 1)

    # Угол зондирования лист 3
    def prep_list_3(self):
        self.rand_degrees_list_3 = np.random.normal(0.5, 1, self.steps)
        for i in range(0, self.steps):
            for j in range(0, 10):
                self.rand_degrees[j][i] = 0.628 * (j + 1) + self.rand_degrees_list_3[i]

    #   Движение по градиенту для каждого листа
    def results(self):
        for i in range(0, self.steps_f):
            sqrt_val = (
                1
                / math.pow(self.a[1], 2)
                * (350 - self.c[1] * math.pow(((i - 1) - self.a[0]), 2))
            )
            self.f_350_plus[i] = np.nan if sqrt_val < 0 else self.a[1] + sqrt_val
            self.f_350_minus[i] = np.nan if sqrt_val < 0 else self.a[1] - sqrt_val
            sqrt_val = (
                1
                / math.pow(self.a[1], 2)
                * (200 - self.c[1] * math.pow(((i - 1) - self.a[0]), 2))
            )
            self.f_200_plus[i] = np.nan if sqrt_val < 0 else self.a[1] + sqrt_val
            self.f_200_minus[i] = np.nan if sqrt_val < 0 else self.a[1] - sqrt_val
            sqrt_val = (
                1
                / math.pow(self.a[1], 2)
                * (50 - self.c[1] * math.pow(((i - 1) - self.a[0]), 2))
            )
            self.f_50_plus[i] = np.nan if sqrt_val < 0 else self.a[1] + sqrt_val
            self.f_50_minus[i] = np.nan if sqrt_val < 0 else self.a[1] - sqrt_val


if __name__ == "__main__":
    figure, axis = plt.subplots(3, 3)
    figure.set_figheight(8)
    figure.set_figwidth(13)
    #   Список для хранения листов
    t = []
    t.append(Task(20))  # Лист 1
    t.append(copy.deepcopy(t[0]))  # Лист 2
    t.append(copy.deepcopy(t[0]))  # Лист 3
    #   Решение каждого листа
    t[1].prep_list_2()
    t[2].prep_list_3()
    t[0].solve()
    t[1].solve()
    t[2].solve()

    #   Для каждого листа
    for i in range(0, 3):
        #   Движение по градиенту для каждого графика
        t[i].results()
        #   Сходимость F
        axis[0, i].plot(np.arange(t[i].steps), t[i].f[: t[i].steps], label="F")
        axis[0, i].legend()
        #   Сходимость
        axis[1, i].scatter(np.arange(t[i].steps), t[i].x1[: t[i].steps], s=5)
        axis[1, i].scatter(np.arange(t[i].steps), t[i].x2[: t[i].steps], s=5)
        axis[1, i].plot(np.arange(t[i].steps), t[i].x1[: t[i].steps], label="x1")
        axis[1, i].plot(np.arange(t[i].steps), t[i].x2[: t[i].steps], label="x2")
        axis[1, i].set_title("Сходимость по x1, x2")
        axis[1, i].legend()
        #   Начальные точки
        arr1 = []
        arr2 = []
        for j in range(0, 10):
            arr1.append(t[i].x1_coord[j][0])
            arr2.append(t[i].x2_coord[j][0])

        axis[2, i].scatter(arr1, arr2, s=15)
        #   Движение по градиенту
        axis[2, i].scatter(np.arange(t[i].steps_f), t[i].f_350_plus, s=5)
        axis[2, i].scatter(np.arange(t[i].steps_f), t[i].f_350_minus, s=5)
        axis[2, i].scatter(np.arange(t[i].steps_f), t[i].f_200_plus, s=5)
        axis[2, i].scatter(np.arange(t[i].steps_f), t[i].f_200_minus, s=5)
        axis[2, i].scatter(np.arange(t[i].steps_f), t[i].f_50_plus, s=5)
        axis[2, i].scatter(np.arange(t[i].steps_f), t[i].f_50_minus, s=5)
        axis[2, i].plot(np.arange(t[i].steps_f), t[i].f_350_plus)
        axis[2, i].plot(np.arange(t[i].steps_f), t[i].f_350_minus)
        axis[2, i].plot(np.arange(t[i].steps_f), t[i].f_200_plus)
        axis[2, i].plot(np.arange(t[i].steps_f), t[i].f_200_minus)
        axis[2, i].plot(np.arange(t[i].steps_f), t[i].f_50_plus)
        axis[2, i].plot(np.arange(t[i].steps_f), t[i].f_50_minus)
        axis[2, i].plot(t[i].x1, t[i].x2, label="x")
        axis[2, i].scatter(t[i].x1[t[i].steps - 1], t[i].x2[t[i].steps - 1], s=40)
        axis[2, i].set_title("Движение по градиенту")
        axis[2, i].legend()
    #   Заголовки для сходимости ЦФ
    axis[0, 0].set_title("Сход. ЦФ (уг. зондирования случайный)")
    axis[0, 1].set_title("Сход. ЦФ (уг. зонд. = 0.628 * ind)")
    axis[0, 2].set_title("Сход. ЦФ (уг. зонд. = 0.628 * ind + rand)")
    plt.tight_layout()
    plt.show()
