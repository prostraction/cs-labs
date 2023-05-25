import matplotlib.pyplot as plt
from matplotlib.widgets import Button
from scipy.stats import norm, binom, poisson
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
        self.r_const = 0.05#np.random.uniform(0.001, 0.05)
        self.dx_const = 0.001
        self.x1 = np.zeros(self.steps)
        self.x2 = np.zeros(self.steps)
        self.df_x1 = np.zeros(self.steps)
        self.df_x2 = np.zeros(self.steps)
        self.r = np.zeros(self.steps)
        self.f = np.zeros(self.steps)
        self.df = np.zeros(self.steps)
        self.dx = np.zeros(self.steps)
        self.f_350_plus = np.zeros(self.steps_f)
        self.f_350_minus = np.zeros(self.steps_f)
        self.f_200_plus = np.zeros(self.steps_f)
        self.f_200_minus = np.zeros(self.steps_f)
        self.f_50_plus = np.zeros(self.steps_f)
        self.f_50_minus = np.zeros(self.steps_f)
        #   Лист 4
        self.f_range = 10 #int(np.random.uniform(5, 10))
        self.k_fract = 7
        #   Лист 5
        self.b1 = np.zeros(self.steps)
        self.b2 = np.zeros(self.steps)

        for _ in range(0, 6):
           self.c.append(np.random.uniform(0.5, 10))
        for _ in range(0, 2):
           self.a.append(np.random.uniform(2, 10))
        for _ in range(0, 2):
           self.x_first.append(np.random.uniform(5, 20))
    #   Лист 1
    def solve_list_1(self):
        for i in range(0, self.steps):
            self.r[i] = self.r_const
            self.x1[i] = self.x_first[0] if i == 0 else self.x1[i-1] - self.r[i] * self.df_x1[i-1]
            self.x2[i] = self.x_first[1] if i == 0 else self.x2[i-1] - self.r[i] * self.df_x2[i-1]
            self.df_x1[i] = self.c[1] * 2 * self.x1[i] - 2 * self.c[1] * self.a[0]
            self.df_x2[i] = self.c[2] * 2 * self.x2[i] - 2 * self.c[2] * self.a[1]
            self.f[i] = self.c[1] * math.pow((self.x1[i] - self.a[0]), 2) + self.c[2] * math.pow((self.x2[i] - self.a[1]), 2)
            self.df[i] = 0 if i == 0 else abs(self.f[i] - self.f[i-1])
            self.dx[i] = 0 if i == 0 else math.sqrt(math.pow(self.x1[i] - self.x1[i-1], 2) + math.pow(self.x2[i] - self.x2[i-1], 2))
    #   Лист 2
    def solve_list_2(self):
        for i in range(0, self.steps):
            self.r[i] = self.r_const
            self.x1[i] = self.x_first[0] if i == 0 else self.x1[i-1] - self.r[i] * self.df_x1[i-1]
            self.x2[i] = self.x_first[1] if i == 0 else self.x2[i-1] - self.r[i] * self.df_x2[i-1]
            if i > 0:
                self.df_x1[i] = ((self.c[1]*math.pow(self.x1[i]+self.dx_const-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]-self.a[1], 2))-(self.c[1]*math.pow(self.x1[i]-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]-self.a[1], 2)))/self.dx_const
                self.df_x2[i] = ((self.c[1]*math.pow(self.x1[i]-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]+self.dx_const-self.a[1], 2))-(self.c[1]*math.pow(self.x1[i]-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]-self.a[1], 2)))/self.dx_const
            else:
                self.df_x1[i] = 0
                self.df_x2[i] = 0
            self.f[i] = self.c[1] * math.pow((self.x1[i] - self.a[0]), 2) + self.c[2] * math.pow((self.x2[i] - self.a[1]), 2)
            self.df[i] = 0 if i == 0 else abs(self.f[i] - self.f[i-1])
            self.dx[i] = 0 if i == 0 else math.sqrt(math.pow(self.x1[i] - self.x1[i-1], 2) + math.pow(self.x2[i] - self.x2[i-1], 2))
    #   Лист 3
    def solve_list_3(self):
        for i in range(0, self.steps):
            self.r[i] = self.r_const
            self.x1[i] = self.x_first[0] if i == 0 else self.x1[i-1] - self.r[i] * self.df_x1[i-1]
            self.x2[i] = self.x_first[1] if i == 0 else self.x2[i-1] - self.r[i] * self.df_x2[i-1]
            if i > 0:
                self.df_x1[i] = ((self.c[1]*math.pow(self.x1[i]+self.dx_const-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]-self.a[1], 2))-(self.c[1]*math.pow(self.x1[i]-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]-self.a[1], 2)))/self.dx_const
                self.df_x2[i] = ((self.c[1]*math.pow(self.x1[i]-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]+self.dx_const-self.a[1], 2))-(self.c[1]*math.pow(self.x1[i]-self.a[0], 2)+self.c[2]*math.pow(self.x2[i]-self.a[1], 2)))/self.dx_const
            else:
                self.df_x1[i] = 0
                self.df_x2[i] = 0
            self.f[i] = ((self.c[0]) 
                         + (self.c[1] * (self.x1[i] - self.a[0]))
                         + (self.c[2] * (self.x2[i] - self.a[1]))
                         + (self.c[3] * math.pow(self.x1[i] - self.a[0], 2))
                         + (self.c[4] * (self.x1[i] - self.a[0]) * self.x2[i] - self.a[1])
                         + (self.c[5] * math.pow(self.x2[i] - self.a[1], 2)))
            self.df[i] = 0 if i == 0 else abs(self.f[i] - self.f[i-1])
            self.dx[i] = 0 if i == 0 else math.sqrt(math.pow(self.x1[i] - self.x1[i-1], 2) + math.pow(self.x2[i] - self.x2[i-1], 2))
    #   Лист 4
    def solve_list_4(self):
        for i in range(0, self.steps):
            self.x1[i] = self.x_first[0] if i == 0 else self.x1[i-1] - self.r[i-1] * self.df_x1[i-1]
            self.x2[i] = self.x_first[1] if i == 0 else self.x2[i-1] - self.r[i-1] * self.df_x2[i-1] 
            self.df_x1[i] = self.c[1] * 2 * self.x1[i] - 2 * self.c[1] * self.a[0]
            self.df_x2[i] = self.c[2] * 2 * self.x2[i] - 2 * self.c[2] * self.a[1]
            self.f[i] = self.c[1] * math.pow((self.x1[i] - self.a[0]), 2) + self.c[2] * math.pow((self.x2[i] - self.a[1]), 2)
            self.r[i] = ((self.r_const) / math.sqrt(math.pow(self.df_x1[i], 2) + math.pow(self.df_x2[i], 2))) 
            if self.f[i] <= self.f_range:
                self.r[i] = self.r[i] / self.k_fract
            self.df[i] = 0 if i == 0 else abs(self.f[i] - self.f[i-1])
            self.dx[i] = 0 if i == 0 else math.sqrt(math.pow(self.x1[i] - self.x1[i-1], 2) + math.pow(self.x2[i] - self.x2[i-1], 2))
    #   Лист 5
    def solve_list_5(self):
        for i in range(0, self.steps):
            self.x1[i] = self.x_first[0] if i == 0 else self.x1[i-1] - self.r[i-1] * self.df_x1[i-1]
            self.x2[i] = self.x_first[1] if i == 0 else self.x2[i-1] - self.r[i-1] * self.df_x2[i-1]
            self.b1[i] = math.pow(self.c[1], 2) * math.pow(self.x1[i] - self.a[0], 2)
            self.b2[i] = math.pow(self.c[2], 2) * math.pow(self.x2[i] - self.a[1], 2)
            self.df_x1[i] = self.c[1] * 2 * self.x1[i] - 2 * self.c[1] * self.a[0]
            self.df_x2[i] = self.c[2] * 2 * self.x2[i] - 2 * self.c[2] * self.a[1]
            self.f[i] = self.c[1] * math.pow((self.x1[i] - self.a[0]), 2) + self.c[2] * math.pow((self.x2[i] - self.a[1]), 2)
            val_div = (2 * (self.b1[i] * self.c[1] + self.b2[i] * self.c[2]))
            self.r[i] = self.r[i-1] if val_div == 0 else (self.b1[i] + self.b2[i]) / val_div
            self.df[i] = 0 if i == 0 else abs(self.f[i] - self.f[i-1])
            self.dx[i] = 0 if i == 0 else math.sqrt(math.pow(self.x1[i] - self.x1[i-1], 2) + math.pow(self.x2[i] - self.x2[i-1], 2))
    #   Движение по градиенту для каждого листа
    def results(self):
        for i in range(0, self.steps_f):
            sqrt_val = (1/math.pow(self.a[1], 2) * (350 - self.c[1] * math.pow(((i - 1) - self.a[0]), 2)))
            self.f_350_plus[i] = np.nan if sqrt_val < 0 else self.a[1] + sqrt_val
            self.f_350_minus[i] = np.nan if sqrt_val < 0 else self.a[1] - sqrt_val
            sqrt_val = (1/math.pow(self.a[1], 2) * (200 - self.c[1] * math.pow(((i - 1) - self.a[0]), 2)))
            self.f_200_plus[i] = np.nan if sqrt_val < 0 else self.a[1] + sqrt_val
            self.f_200_minus[i] = np.nan if sqrt_val < 0 else self.a[1] - sqrt_val
            sqrt_val = (1/math.pow(self.a[1], 2) * (50 - self.c[1] * math.pow(((i - 1) - self.a[0]), 2)))
            self.f_50_plus[i] = np.nan if sqrt_val < 0 else self.a[1] + sqrt_val
            self.f_50_minus[i] = np.nan if sqrt_val < 0 else self.a[1] - sqrt_val

if __name__ == "__main__":
    figure, axis = plt.subplots(3, 5)
    figure.set_figheight(8)
    figure.set_figwidth(13)
    #   Список для хранения листов
    t = []
    t.append(Task(100))             # Лист 1
    t.append(copy.deepcopy(t[0]))   # Лист 2
    t.append(copy.deepcopy(t[0]))   # Лист 3
    t.append(Task(1000))            # Лист 4
    t.append(copy.deepcopy(t[0]))   # Лист 5
    #   Решение каждого листа
    t[0].solve_list_1()
    t[1].solve_list_2()
    t[2].solve_list_3()
    t[3].solve_list_4()
    t[4].solve_list_5()
    t[4].steps = 40
    
    #   Для каждого листа
    for i in range(0, 5):
        #   Движение по градиенту для каждого графика
        t[i].results()
        #   Сходимость F
        axis[0, i].plot(np.arange(t[i].steps), t[i].f[:t[i].steps], label = 'F')
        axis[0, i].legend()
        #   Сходимость
        axis[1, i].scatter(np.arange(t[i].steps), t[i].x1[:t[i].steps], s=5)
        axis[1, i].scatter(np.arange(t[i].steps), t[i].x2[:t[i].steps], s=5)
        axis[1, i].plot(np.arange(t[i].steps), t[i].x1[:t[i].steps], label = 'x1')
        axis[1, i].plot(np.arange(t[i].steps), t[i].x2[:t[i].steps], label = 'x2')
        axis[1, i].set_title('Сходимость по x1, x2')
        axis[1, i].legend()
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
        axis[2, i].plot(t[i].x1, t[i].x2, label = 'x')
        axis[2, i].scatter(t[i].x1[t[i].steps-1], t[i].x2[t[i].steps-1], s=40)
        axis[2, i].set_title('Движение по градиенту')
        axis[2, i].legend()
    #   Заголовки для сходимости ЦФ
    axis[0, 0].set_title('Сход. ЦФ')
    axis[0, 1].set_title('Сход. ЦФ (dx = 0.001)')
    axis[0, 2].set_title('Сход. ЦФ (F2)')
    axis[0, 3].set_title('Сход. ЦФ (r != const)')
    axis[0, 4].set_title('Сход. ЦФ (наискор. спуск)')
    plt.tight_layout()
    plt.show()
