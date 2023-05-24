import matplotlib.pyplot as plt
from matplotlib.widgets import Button
from scipy.stats import norm, binom, poisson
import numpy as np
import copy

#   Начальные значения
variant = 9
np.random.seed(variant)
item_size = 100

#   Решение задачи
class Task:
    #   Начальные значения
    def __init__(self):
        self.x = list(range(1, 101))
        self.y = np.zeros(item_size)
        self.dev = variant / 100
        self.pocket_count = 12
    #   Посчитать количество карманов
    def count_pockets(self):
        self.pocket_value = np.zeros((self.pocket_count + 1), dtype=int)
        for i in range(0, item_size):
            for j in range(0, self.pocket_value.size):
                if self.y[i] <= self.pocket_list[j]:
                    self.pocket_value[j] += 1
                    break
    #   Для графиков на стр 1-5
    def y_graph(self):
        self.f_y = copy.copy(self.rel_speq)
        for i in range(1, self.f_y.size):
            self.f_y[i] = self.f_y[i - 1] + self.f_y[i]
    #   Страница 1
    def solve_list_1(self):
        self.y = np.random.normal(variant, self.dev, item_size)
        self.pocket_count = 12
        self.min = self.y.min()
        self.max = self.y.max()
        self.pocket = (self.max - self.min) / self.pocket_count
        self.pocket_list = np.zeros(self.pocket_count + 1)
        self.pocket_list[0] = self.min
        for i in range(1, self.pocket_count + 1):
            self.pocket_list[i] = self.pocket_list[i - 1] + self.pocket

        self.count_pockets()
        self.rel_speq = copy.copy(self.pocket_value) / item_size
        self.norm_dist = np.arange(float(self.pocket_count + 1)) * 0
        for i in range(0, self.norm_dist.size):
            self.norm_dist[i] = norm.pdf(self.pocket_list[i], variant, self.dev)
        self.nr = copy.copy(self.norm_dist) / sum(self.norm_dist)
    #   Страница 2
    def solve_list_2(self):
        self.y = np.random.uniform(variant, variant * 2, item_size)
        self.pocket = 1
        self.pocket_count = variant
        self.min = variant + 1
        self.max = variant * 2
        self.pocket_list = list(range(self.min, self.max + 1))
        self.count_pockets()
        self.pocket_value = self.pocket_value[:variant]
        self.rel_speq = copy.copy(self.pocket_value) / item_size
        self.theory = copy.copy(self.pocket_value) * 0 + 1 / variant
        self.y_graph()
    #   Страница 3
    def solve_list_3(self):
        for i in range(0, item_size):
            self.y = np.random.binomial(1, 0.17, item_size)
        self.pocket_list = [0, 1]
        self.pocket_value = np.zeros((2), dtype=int)
        self.pocket_count = 2
        for i in range(0, item_size):
            if self.y[i] == 0:
                self.pocket_value[0] += 1
            else:
                self.pocket_value[1] += 1
        self.rel_speq = copy.copy(self.pocket_value) / item_size
        self.theory = [1 - self.dev, self.dev]
        self.f_y = copy.copy(self.rel_speq)
        self.f_y[1] = self.f_y[1] + self.f_y[0]
    #   Страница 4
    def solve_list_4(self):
        self.m = 5
        for i in range(0, item_size):
            self.y = np.random.binomial(self.m, 0.17, item_size)
        self.pocket = 1
        self.pocket_count = self.m
        self.min = 0
        self.max = self.m
        self.pocket_list = list(range(self.min, self.max + 1))
        self.count_pockets()
        self.rel_speq = copy.copy(self.pocket_value) / item_size
        self.theory = np.arange(float(self.pocket_count + 1)) * 0
        for i in range(0, self.m):
            self.theory[i] = binom.pmf(self.pocket_list[i], self.m, self.dev)
        self.y_graph()
    #   Страница 5
    def solve_list_5(self):
        self.y = np.random.poisson(variant, item_size)
        self.min = 0
        self.max = max(self.y)
        self.pocket = 1
        self.pocket_count = self.max
        self.pocket_list = list(range(self.min, self.max + 1))
        self.count_pockets()
        self.rel_speq = copy.copy(self.pocket_value) / item_size
        self.theory = np.arange(float(self.pocket_count + 1)) * 0
        for i in range(0, self.pocket_count):
            self.theory[i] = poisson.pmf(self.pocket_list[i], sum(self.y) / item_size)
        self.y_graph()
    #   Страница 6
    def solve_list_6(self):
        self.item_size2 = 200
        self.rand_num = np.random.rand(10)
        self.d1 = variant
        self.d2 = variant * 2
        self.d3 = variant * 3
        self.d4 = 200 - variant
        self.e1 = self.rand_num.max()
        self.e2 = np.mean(self.rand_num)
        self.e3 = self.rand_num.min()

        self.x = list(range(1, 201))
        self.y = np.random.rand(self.item_size2)
        self.msa_d1 = np.zeros(self.item_size2 - self.d1)
        self.msa_d2 = np.zeros(self.item_size2 - self.d2)
        self.msa_d3 = np.zeros(self.item_size2 - self.d3)
        self.msa_d4 = np.zeros(self.item_size2 - self.d4)
        self.ecsp_e1 = np.zeros(self.item_size2 - 1)
        self.ecsp_e2 = np.zeros(self.item_size2 - 1)
        self.ecsp_e3 = np.zeros(self.item_size2 - 1)

        for i in range(0, self.item_size2):
            if i + 1 >= self.d1:
                self.msa_d1[i - self.d1] = 0
                for j in range(0, self.d1):
                    self.msa_d1[i - self.d1] += self.y[i - j]
                self.msa_d1[i - self.d1] /= self.d1
            if i + 1 >= self.d2:
                self.msa_d2[i - self.d2] = 0
                for j in range(0, self.d2):
                    self.msa_d2[i - self.d2] += self.y[i - j]
                self.msa_d2[i - self.d2] /= self.d2
            if i + 1 >= self.d3:
                self.msa_d3[i - self.d3] = 0
                for j in range(0, self.d3):
                    self.msa_d3[i - self.d3] += self.y[i - j]
                self.msa_d3[i - self.d3] /= self.d3
            if i + 1 >= self.d4:
                self.msa_d4[i - self.d4] = 0
                for j in range(0, self.d4):
                    self.msa_d4[i - self.d4] += self.y[i - j]
                self.msa_d4[i - self.d4] /= self.d4
            if i == 1:
                self.ecsp_e1[i - 1] = self.y[i] * self.e1
                self.ecsp_e2[i - 1] = self.y[i] * self.e2
                self.ecsp_e3[i - 1] = self.y[i] * self.e3
            if i > 1:
                self.ecsp_e1[i - 1] = (
                    self.y[i] * self.e1 + (1 - self.e1) * self.ecsp_e1[i - 2]
                )
                self.ecsp_e2[i - 1] = (
                    self.y[i] * self.e2 + (1 - self.e2) * self.ecsp_e2[i - 2]
                )
                self.ecsp_e3[i - 1] = (
                    self.y[i] * self.e3 + (1 - self.e3) * self.ecsp_e3[i - 2]
                )

        self.average = np.zeros(8)
        self.dispers = np.zeros(8)
        self.sko = np.zeros(8)

        self.average[0] = np.mean(self.y)
        self.average[1] = np.mean(self.msa_d1)
        self.average[2] = np.mean(self.msa_d2)
        self.average[3] = np.mean(self.msa_d3)
        self.average[4] = np.mean(self.msa_d4)
        self.average[5] = np.mean(self.ecsp_e1)
        self.average[6] = np.mean(self.ecsp_e2)
        self.average[7] = np.mean(self.ecsp_e3)

        self.square_deviation = lambda x: (x - np.mean(self.y)) ** 2
        self.dispers[0] = sum(map(self.square_deviation, self.y)) / (
            self.item_size2 - 1
        )
        self.square_deviation = lambda x: (x - np.mean(self.msa_d1)) ** 2
        self.dispers[1] = sum(map(self.square_deviation, self.msa_d1)) / (
            self.item_size2 - 1
        )
        self.square_deviation = lambda x: (x - np.mean(self.msa_d2)) ** 2
        self.dispers[2] = sum(map(self.square_deviation, self.msa_d2)) / (
            self.item_size2 - 1
        )
        self.square_deviation = lambda x: (x - np.mean(self.msa_d3)) ** 2
        self.dispers[3] = sum(map(self.square_deviation, self.msa_d3)) / (
            self.item_size2 - 1
        )
        self.square_deviation = lambda x: (x - np.mean(self.msa_d4)) ** 2
        self.dispers[4] = sum(map(self.square_deviation, self.msa_d4)) / (
            self.item_size2 - 1
        )
        self.square_deviation = lambda x: (x - np.mean(self.ecsp_e1)) ** 2
        self.dispers[5] = sum(map(self.square_deviation, self.ecsp_e1)) / (
            self.item_size2 - 1
        )
        self.square_deviation = lambda x: (x - np.mean(self.ecsp_e2)) ** 2
        self.dispers[6] = sum(map(self.square_deviation, self.ecsp_e2)) / (
            self.item_size2 - 1
        )
        self.square_deviation = lambda x: (x - np.mean(self.ecsp_e3)) ** 2
        self.dispers[7] = sum(map(self.square_deviation, self.ecsp_e3)) / (
            self.item_size2 - 1
        )

        for i in range(0, 8):
            self.sko[i] = self.dispers[i] ** (0.5)

#   Отрисовка графиоков на всех страницах
class Drawer:
    def __init__(self):
        self.page = 0
        self.max_pages = 6
        self.axes_page = figure.add_axes([0.4, 0.01, 0.15, 0.04])
        self.axes_page.axis("off")
        self.draw_page(0)
    #   Выбрать следующую страницу
    def select_next_page(self, event):
        if self.page + 1 < self.max_pages:
            self.page += 1
            for i in range(0, 4):
                axis[i].clear()
            self.draw_page(self.page)
            plt.draw()
    #   Выбрать предыдущую страницу
    def select_prev_page(self, event):
        if self.page > 0:
            self.page -= 1
            for i in range(0, 4):
                axis[i].clear()
            self.draw_page(self.page)
            plt.draw()
    #   Отрисовка выбранной страницы
    def draw_page(self, page):
        self.axes_page.set_title("Страница " + str(self.page + 1))
        if page == 0:
            self.draw_page_1()
        if page == 1:
            self.draw_page_2()
        if page == 2:
            self.draw_page_3()
        if page == 3:
            self.draw_page_4()
        if page == 4:
            self.draw_page_5()
        if page == 5:
            self.draw_page_6()
    #   Отрисовка для страницы 1
    def draw_page_1(self):
        axis[0].plot(t1.x, t1.y)
        axis[1].hist(t1.y, edgecolor="black", bins=t1.pocket_count)
        axis[2].hist(
            t1.y,
            weights=np.ones_like(t1.y) / len(t1.y),
            edgecolor="black",
            bins=t1.pocket_count,
        )
        axis[2].plot(t1.pocket_list, t1.nr)
        axis[3].plot(t1.pocket_list, t1.f_y)

        axis[0].set_title("Нормальное распределение")
        axis[1].set_title("Гистограмма")
        axis[2].set_title("Гистограмма")
        axis[3].set_title("F(y)")
    #   Отрисовка для страницы 2
    def draw_page_2(self):
        axis[0].plot(t2.x, t2.y)
        axis[1].hist(t2.y, edgecolor="black", bins=t2.pocket_count)
        axis[2].hist(
            t2.y,
            weights=np.ones_like(t2.y) / len(t2.y),
            edgecolor="black",
            bins=t2.pocket_count,
        )
        axis[2].plot(t2.pocket_list, t2.theory)
        axis[3].plot(t2.pocket_list, t2.f_y)

        axis[0].set_title("Равномерное распределение")
        axis[1].set_title("Гистограмма")
        axis[2].set_title("Гистограмма")
        axis[3].set_title("F(y)")
    #   Отрисовка для страницы 3
    def draw_page_3(self):
        axis[0].plot(t3.x, t3.y)
        axis[1].hist(t3.y, edgecolor="black", bins=t3.pocket_count)
        axis[2].hist(
            t3.y,
            weights=np.ones_like(t3.y) / len(t3.y),
            edgecolor="black",
            bins=t3.pocket_count,
        )
        axis[2].plot(t3.pocket_list, t3.theory)
        axis[3].plot(t3.pocket_list, t3.f_y)
        axis[0].set_title("Бернулли")
        axis[1].set_title("Гистограмма")
        axis[2].set_title("Гистограмма")
        axis[3].set_title("F(y)")
    #   Отрисовка для страницы 4
    def draw_page_4(self):
        axis[0].plot(t4.x, t4.y)
        axis[1].hist(t4.y, edgecolor="black", bins=t4.pocket_count)
        axis[2].hist(
            t4.y,
            weights=np.ones_like(t4.y) / len(t4.y),
            edgecolor="black",
            bins=t4.pocket_count,
        )
        axis[2].plot(t4.pocket_list, t4.theory)
        axis[3].plot(t4.pocket_list, t4.f_y)
        axis[0].set_title("Биноминальное")
        axis[1].set_title("Гистограмма")
        axis[2].set_title("Гистограмма")
        axis[3].set_title("F(y)")
    #   Отрисовка для страницы 5
    def draw_page_5(self):
        axis[0].plot(t5.x, t5.y)
        axis[1].hist(t5.y, edgecolor="black", bins=t5.pocket_count)
        axis[2].hist(
            t5.y,
            weights=np.ones_like(t5.y) / len(t5.y),
            edgecolor="black",
            bins=t5.pocket_count,
        )
        axis[2].plot(t5.pocket_list, t5.theory)
        axis[3].plot(t5.pocket_list, t5.f_y)
        axis[0].set_title("Распределение Пуассона")
        axis[1].set_title("Гистограмма")
        axis[2].set_title("Гистограмма")
        axis[3].set_title("F(y)")
    #   Отрисовка для страницы 6
    def draw_page_6(self):
        axis[0].plot(t6.x, t6.y, label="Фактический")
        axis[0].plot(list(range(t6.d1 + 1, 201)), t6.msa_d1, label="Сглаживание d1")
        axis[0].plot(list(range(t6.d2 + 1, 201)), t6.msa_d2, label="Сглаживание d2")
        axis[0].plot(list(range(t6.d3 + 1, 201)), t6.msa_d3, label="Сглаживание d3")
        axis[0].plot(list(range(t6.d4 + 1, 201)), t6.msa_d4, label="Сглаживание d4")
        axis[1].plot(t6.x, t6.y, label="Фактический")
        axis[1].plot(list(range(2, 201)), t6.ecsp_e1, label="Прогноз e1")
        axis[1].plot(list(range(2, 201)), t6.ecsp_e2, label="Прогноз e2")
        axis[1].plot(list(range(2, 201)), t6.ecsp_e3, label="Прогноз e3")
        axis[2].plot(1, t6.average[0], ":o", label="Среднее СП")
        axis[2].plot(1, t6.dispers[0], ":o", label="Дисп. СП")
        axis[2].plot(1, t6.sko[0], ":o", label="СКО СП")
        axis[2].plot(
            list(range(2, 6)),
            np.array([t6.average[i] for i in range(1, 5)]),
            label="Среднее МСС",
        )
        axis[2].plot(
            list(range(2, 6)),
            np.array([t6.dispers[i] for i in range(1, 5)]),
            label="Дисп. МСС",
        )
        axis[2].plot(
            list(range(2, 6)),
            np.array([t6.sko[i] for i in range(1, 5)]),
            label="СКО МСС",
        )
        axis[2].plot(
            list(range(6, 9)),
            np.array([t6.average[i] for i in range(5, 8)]),
            label="Среднее МЭС",
        )
        axis[2].plot(
            list(range(6, 9)),
            np.array([t6.dispers[i] for i in range(5, 8)]),
            label="Дисп. МЭС",
        )
        axis[2].plot(
            list(range(6, 9)),
            np.array([t6.sko[i] for i in range(5, 8)]),
            label="СКО МЭС",
        )
        axis[0].set_title("Метод скользящего среднего")
        axis[0].legend()
        axis[1].set_title("Экспоненциальное сглаживание")
        axis[1].legend()
        axis[2].set_title("Стат. хар-ки сглаженных CП")
        axis[2].legend()


if __name__ == "__main__":
    t1 = Task()
    t1.solve_list_1()
    t1.y_graph()

    t2 = Task()
    t2.solve_list_2()
    t2.y_graph()

    t3 = Task()
    t3.solve_list_3()
    t3.y_graph()

    t4 = Task()
    t4.solve_list_4()
    t4.y_graph()

    t5 = Task()
    t5.solve_list_5()
    t5.y_graph()

    t6 = Task()
    t6.solve_list_6()

    figure, axis = plt.subplots(1, 4)
    figure.set_figheight(7)
    figure.set_figwidth(14)

    draw_callback = Drawer()
    axes_prev_page = figure.add_axes([0.1, 0.01, 0.15, 0.04])
    axes_next_page = figure.add_axes([0.75, 0.01, 0.15, 0.04])
    but_prev_page = Button(axes_prev_page, "Предыдущая страница")
    but_next_page = Button(axes_next_page, "Следующая страница")
    but_prev_page.on_clicked(draw_callback.select_prev_page)
    but_next_page.on_clicked(draw_callback.select_next_page)

    plt.subplots_adjust(left=0.05, bottom=0.12, top=0.95, right=0.95)
    plt.show()
