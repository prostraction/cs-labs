import matplotlib.pyplot as plt
import numpy as np

class Task:
    def __init__(self):
        self.variant = 9
        np.random.seed(self.variant)
        self.item_size = 100
        self.x = np.arange(1, self.item_size + 1)
        self.noise = np.random.uniform(-1, 1, self.item_size)
        self.damping = np.random.uniform(0.6, 0.9)

    def generate_linear_trend(self):
        a = np.random.normal(3, 0.3)
        b = np.random.normal(0.1, 0.01)
        k = np.random.normal(7, 0.7)
        z = a + b * self.x + k * self.noise
        damping = np.zeros(self.item_size)
        damping[1] = z[0]
        for i in range(2, self.item_size):
            damping[i] = (1 - self.damping) * z[i - 1] + self.damping * damping[i - 1]
        return z, damping

    def generate_sinusoidal_trend(self):
        a = np.random.normal(3, 0.3)
        b = np.random.normal(0.3, 0.03)
        c = np.random.normal(5, 0.5)
        d = np.random.normal(0.1, 0.01)
        e = np.random.normal(0.15, 0.015)
        k = np.random.normal(30, 3)
        z = a + b * self.x + k * self.noise + c * np.sin(e * (self.x - d))
        damping = np.zeros(self.item_size)
        damping[1] = z[0]
        for i in range(2, self.item_size):
            damping[i] = (1 - self.damping) * z[i - 1] + self.damping * damping[i - 1]
        return z, damping

    def generate_exponential_trend(self):
        a = np.random.normal(3, 0.3)
        b = np.random.normal(0.3, 0.03)
        c = np.random.normal(0.1, 0.01)
        k = np.random.normal(0.1, 0.01)
        z = a - b * np.exp(-c * self.x) + k * self.noise
        damping = np.zeros(self.item_size)
        damping[1] = z[0]
        for i in range(2, self.item_size):
            damping[i] = (1 - self.damping) * z[i - 1] + self.damping * damping[i - 1]
        return z, damping

    def generate_piecewise_linear_trend(self):
        x_1 = int(np.random.uniform(15, 35))
        x_2 = x_1 + int(np.random.uniform(15, 35))
        x_3 = x_2 + int(np.random.uniform(15, 35))

        b1 = np.random.normal(0.3, 0.03)
        b2 = np.random.normal(0.1, 0.01)
        b3 = np.random.normal(0.2, 0.02)
        b4 = np.random.normal(0.05, 0.005)

        a1 = np.random.normal(3, 0.3)
        a2 = a1 + b1 * x_1 - b2 * x_1
        a3 = a2 + b2 * x_2 - b3 * x_2
        a4 = a3 + b3 * x_3 - b4 * x_3

        k = np.random.normal(7, 0.7)

        line_part = np.piecewise(
            self.x,
            [
                self.x < x_1,
                (self.x >= x_1) & (self.x < x_2),
                (self.x >= x_2) & (self.x < x_3),
                self.x >= x_3,
            ],
            [
                lambda x: a1 + b1 * x,
                lambda x: a2 + b2 * x,
                lambda x: a1 + b1 * x,
                lambda x: a4 + b4 * x,
            ],
        )
        z = line_part + k * self.noise
        damping = np.zeros(self.item_size)
        damping[1] = z[0]
        for i in range(2, self.item_size):
            damping[i] = (1 - self.damping) * z[i - 1] + self.damping * damping[i - 1]
        return z, damping

    def generate_piecewise_constant_trend(self):
        x_1 = int(np.random.uniform(15, 35))
        x_2 = x_1 + int(np.random.uniform(15, 35))
        x_3 = x_2 + int(np.random.uniform(15, 35))

        b1 = np.random.normal(0.3, 0.03)
        b2 = np.random.normal(0.1, 0.01)
        b3 = np.random.normal(0.2, 0.02)
        b4 = np.random.normal(0.05, 0.005)

        a1 = np.random.normal(3, 0.3)
        a2 = a1 + b1 * x_1 - b2 * x_1
        a3 = a2 + b2 * x_2 - b3 * x_2
        a4 = a3 + b3 * x_3 - b4 * x_3

        k = np.random.normal(7, 0.7)

        kus_pos = np.piecewise(
            self.x,
            [
                self.x < x_1,
                (self.x >= x_1) & (self.x < x_2),
                (self.x >= x_2) & (self.x < x_3),
                self.x >= x_3,
            ],
            [lambda x: a1, lambda x: a2, lambda x: a1, lambda x: a4],
        )
        z = kus_pos + k * self.noise
        damping = np.zeros(self.item_size)
        damping[1] = z[0]
        for i in range(2, self.item_size):
            damping[i] = (1 - self.damping) * z[i - 1] + self.damping * damping[i - 1]
        return z, damping

    def plot_results(self):
        plot_1 = self.generate_linear_trend()
        plot_2 = self.generate_sinusoidal_trend()
        plot_3 = self.generate_exponential_trend()
        plot_4 = self.generate_piecewise_linear_trend()
        plot_5 = self.generate_piecewise_constant_trend()

        fig, axes = plt.subplots(2, 3)
        fig.set_figheight(9)
        fig.set_figwidth(14)

        axes[0, 0].plot(self.x, self.noise, label="Шум")
        axes[0, 0].plot(self.x, plot_1[0], label="z")
        axes[0, 0].plot(self.x[1:], plot_1[1][1:], label="Затухания")
        axes[0, 0].plot(
            self.x,
            np.poly1d(np.polyfit(self.x, plot_1[0], 3))(self.x),
            linestyle="--",
            label="Линейная линия тренда",
        )
        axes[0, 0].set_title("Линейный тренд")
        axes[0, 0].legend()

        axes[0, 1].plot(self.x, plot_2[0], label="Функция")
        axes[0, 1].plot(self.x[1:], plot_2[1][1:], label="Затухания")
        axes[0, 1].plot(
            self.x,
            np.poly1d(np.polyfit(self.x, plot_2[0], 3))(self.x),
            linestyle="--",
            label="Полимиальная линия тренда",
        )
        axes[0, 1].set_title("Синусоидальный тренд")
        axes[0, 1].legend()

        axes[0, 2].plot(self.x, plot_3[0], label="Функция")
        axes[0, 2].plot(self.x[1:], plot_3[1][1:], label="Затухания")
        axes[0, 2].plot(
            self.x,
            np.poly1d(np.polyfit(self.x, plot_3[0], 3))(self.x),
            linestyle="--",
            label="Логарифмическая линия тренда",
        )
        axes[0, 2].set_title("Экспоненциальный тренд")
        axes[0, 2].legend()

        axes[1, 0].plot(self.x, self.noise, label="Шум")
        axes[1, 0].plot(self.x, plot_4[0], label="Кус.лин.")
        axes[1, 0].plot(self.x, plot_4[0], label="Функция")
        axes[1, 0].plot(self.x[1:], plot_4[1][1:], label="Затухания")
        axes[1, 0].plot(
            self.x,
            np.poly1d(np.polyfit(self.x, plot_4[0], 3))(self.x),
            linestyle="--",
            label="Полимиальная линия тренда",
        )
        axes[1, 0].set_title("Кусочно-линейный тренд")
        axes[1, 0].legend()

        axes[1, 1].plot(self.x, plot_5[1], label="Кус.пост.")
        axes[1, 1].plot(self.x, plot_5[0], label="Функция")
        axes[1, 1].plot(self.x[1:], plot_5[1][1:], label="Затухания")
        axes[1, 1].plot(
            self.x,
            np.poly1d(np.polyfit(self.x, plot_5[0], 3))(self.x),
            linestyle="--",
            label="Линейная линия тренда",
        )
        axes[1, 1].plot(
            self.x,
            np.poly1d(np.polyfit(self.x, plot_5[0], 5))(self.x),
            linestyle="--",
            label="Полимиальная линия тренда",
        )
        axes[1, 1].set_title("Кусочно-постоянный тренд")
        axes[1, 1].legend()

        plt.show()

task = Task()
task.plot_results()
