import matplotlib.pyplot as plt
import numpy as np


class Task:
    def __init__(self, variant=9, item_size=101, tau=30):
        self.variant = variant
        self.item_size = item_size
        self.tau = tau
        self.initialize()

    def initialize(self):
        np.random.seed(self.variant)
        self.iter = np.arange(0, self.item_size)
        self.a = np.random.normal(0.1, 0.07)
        self.b = np.random.normal(1, 0.2)
        self.noise = np.random.uniform(-1, 1, self.item_size)

    def compute_ar(self, shift):
        arrp = np.zeros(self.item_size)
        arrp[:shift] = self.compute_initial_values(shift)
        for i in range(shift, self.item_size):
            arrp[i] = self.a * np.sum(arrp[i - shift : i]) + self.b * self.noise[i]
        return arrp

    def compute_initial_values(self, shift):
        values = np.zeros(shift)
        for i in range(shift):
            values[i] = self.a * np.sum(values[:i]) + self.b * self.noise[i]
        return values

    def compute_nacf(self, arrp):
        nacf = np.zeros(self.tau)
        for i in range(self.tau):
            nacf[i] = np.corrcoef(
                arrp[: self.item_size - self.tau],
                arrp[i : self.item_size - self.tau + i],
            )[0, 1]
        return nacf

    def compute_ccf(self, arrp_1, arrp_2):
        ccf = np.zeros(2 * self.tau + 1)
        for i in range(self.tau + 1):
            ccf[i] = np.corrcoef(
                arrp_1[i : i + self.tau], arrp_2[self.tau : 2 * self.tau]
            )[0, 1]
        for i in range(self.tau + 1, 2 * self.tau + 1):
            ccf[i] = np.corrcoef(
                arrp_1[self.tau : 2 * self.tau], arrp_2[i : i + self.tau]
            )[0, 1]
        return ccf

    def plot(self, arrp_1, arrp_3, arrp_5, nacf_1, nacf_3, nacf_5, ccf):
        x = np.arange(-30, 31)
        zt1 = np.polyfit(self.iter[: self.tau], nacf_1, 5)
        zt2 = np.polyfit(self.iter[: self.tau], nacf_3, 5)
        zt3 = np.polyfit(self.iter[: self.tau], nacf_5, 5)
        p1 = np.poly1d(zt1)
        p2 = np.poly1d(zt2)
        p3 = np.poly1d(zt3)

        figure, axis = plt.subplots(2, 4)
        figure.set_figheight(7)
        figure.set_figwidth(14)

        axis[0, 0].plot(self.iter, arrp_1)
        axis[0, 1].plot(self.iter, arrp_3)
        axis[0, 2].plot(self.iter, arrp_5)
        axis[1, 0].plot(self.iter[: self.tau], nacf_1)
        axis[1, 0].plot(self.iter[: self.tau], p1(self.iter[: self.tau]))
        axis[1, 1].plot(self.iter[: self.tau], nacf_3)
        axis[1, 1].plot(self.iter[: self.tau], p2(self.iter[: self.tau]))
        axis[1, 2].plot(self.iter[: self.tau], nacf_5)
        axis[1, 2].plot(self.iter[: self.tau], p3(self.iter[: self.tau]))
        axis[0, 3].plot(self.iter, self.noise)
        axis[1, 3].plot(x, ccf)

        axis[0, 0].set_title("АРСП_1")
        axis[0, 1].set_title("АРСП_3")
        axis[0, 2].set_title("АРСП_5")
        axis[0, 3].set_title("Шум")
        axis[1, 0].set_title("НАКФ_1")
        axis[1, 1].set_title("НАКФ_3")
        axis[1, 2].set_title("НАКФ_5")
        axis[1, 3].set_title("ВКФ_1-3")

        plt.show()
        print(self.a)

    def solve(self):
        arrp_1 = self.compute_ar(1)
        arrp_3 = self.compute_ar(3)
        arrp_5 = self.compute_ar(5)
        nacf_1 = self.compute_nacf(arrp_1)
        nacf_3 = self.compute_nacf(arrp_3)
        nacf_5 = self.compute_nacf(arrp_5)
        ccf = self.compute_ccf(arrp_1, arrp_3)
        self.plot(arrp_1, arrp_3, arrp_5, nacf_1, nacf_3, nacf_5, ccf)


if __name__ == "__main__":
    task = Task()
    task.solve()
