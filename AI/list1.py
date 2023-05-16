import matplotlib.pyplot as plt
import numpy as np
import copy

#   1.1     Начальные значения
variant = 9
np.random.seed(variant)

# Параметры обучения
delta_C1 = 0.5 #np.random.uniform(0.2, 0.8)
train_speed = 0.9 #np.random.uniform(0.1, 0.9)
epochs = int(np.random.uniform(7, 12))
objects_count = int(np.random.uniform(15, 25))
item_size = int(epochs * objects_count)

class Task:
    # Класс 1
    x1_c1 = np.random.uniform(0, 10)
    x2_c1 = np.random.uniform(5, 15)
    x3_c1 = np.random.uniform(0, 10)
    x4_c1 = np.random.uniform(5, 15)
    x5_c1 = np.random.uniform(0, 10)
    sko_c1 = np.random.uniform(1, 3)
    # Класс 2
    x1_c2 = np.random.uniform(0, 10)
    x2_c2 = np.random.uniform(5, 15)
    x3_c2 = np.random.uniform(0, 10)
    x4_c2 = np.random.uniform(5, 15)
    x5_c2 = np.random.uniform(0, 10)
    sko_c2 = np.random.uniform(1, 3)
    #   1.2     Случайные числа
    r1 = np.random.normal(0, 1, item_size)
    r2 = np.random.normal(0, 1, item_size)
    r3 = np.random.normal(0, 1, item_size)
    r4 = np.random.normal(0, 1, item_size)
    r5 = np.random.normal(0, 1, item_size)
    sigma = np.random.sample(item_size)
    # Переменные для обучения
    learn_d = np.zeros(item_size)
    learn_x0 = np.ones(item_size)
    learn_x1 = np.zeros(item_size)
    learn_x2 = np.zeros(item_size)
    learn_x3 = np.zeros(item_size)
    learn_x4 = np.zeros(item_size)
    learn_x5 = np.zeros(item_size)
    x1_class1 = np.zeros(item_size)
    x2_class1 = np.zeros(item_size)
    x1_class2 = np.zeros(item_size)
    x2_class2 = np.zeros(item_size)
    def __init__(self):
        self.s_wx = np.zeros(item_size)
        self.y_predict = np.zeros(item_size)
        self.e_err_predict = np.zeros(item_size)
        self.e_count_predict = np.zeros(item_size)

        self.learn_d = np.zeros(item_size)
        self.learn_x0 = np.ones(item_size)
        self.learn_x1 = np.zeros(item_size)
        self.learn_x2 = np.zeros(item_size)
        self.learn_x3 = np.zeros(item_size)
        self.learn_x4 = np.zeros(item_size)
        self.learn_x5 = np.zeros(item_size)
        self.x1_class1 = np.zeros(item_size)
        self.x2_class1 = np.zeros(item_size)
        self.x1_class2 = np.zeros(item_size)
        self.x2_class2 = np.zeros(item_size)

        self.dw0 = np.zeros(item_size)
        self.dw1 = np.zeros(item_size)
        self.dw2 = np.zeros(item_size) 
        self.dw3 = np.zeros(item_size)
        self.dw4 = np.zeros(item_size)
        self.dw5 = np.zeros(item_size)

        self.w0 = np.zeros(item_size)
        self.w1 = np.zeros(item_size)
        self.w2 = np.zeros(item_size)
        self.w3 = np.zeros(item_size)
        self.w4 = np.zeros(item_size)
        self.w5 = np.zeros(item_size)

        self.epochs_err_count = np.zeros(epochs+1)
        self.epochs_w0 = np.zeros(epochs+1)
        self.epochs_w1 = np.zeros(epochs+1)
        self.epochs_w2 = np.zeros(epochs+1)
        self.epochs_w3 = np.zeros(epochs+1)
        self.epochs_w4 = np.zeros(epochs+1)
        self.epochs_w5 = np.zeros(epochs+1)
        self.x1_graph = 0
        self.x2_graph = 0

    def make_batch(self):
        t1 = self.r1[0:objects_count]
        for i in range(0, epochs):
            for j in range(0, objects_count):
                self.r1[j+i*objects_count] = t1[j]
        t2 = self.r2[0:objects_count]
        for i in range(0, epochs):
            for j in range(0, objects_count):
                self.r2[j+i*objects_count] = t2[j]
        t3 = self.sigma[0:objects_count]
        for i in range(0, epochs):
            for j in range(0, objects_count):
                self.sigma[j+i*objects_count] = t3[j]
        t5 = self.learn_d[0:objects_count]
        for i in range(0, epochs):
            for j in range(0, objects_count):
                self.learn_d[j+i*objects_count] = t5[j]

    #   1.3     Обучающая выборка
    def fill_learn_data(self):
        # d    
        for i in range(0, self.learn_d.size):
            self.learn_d[i] = 1 if self.sigma[i] < delta_C1 else -1

        # X1
        for i in range(0, self.learn_x1.size):
            self.learn_x1[i] = (self.x1_c1 + self.sko_c1 * self.r1[i]) if self.learn_d[i] == 1 else (self.x1_c2 + self.sko_c2 * self.r1[i])
        # X2
        for i in range(0, self.learn_x2.size):
            self.learn_x2[i] = (self.x2_c1 + self.sko_c1 * self.r2[i]) if self.learn_d[i] == 1 else (self.x2_c2 + self.sko_c2 * self.r2[i])
        # X3
        for i in range(0, self.learn_x2.size):
            self.learn_x3[i] = (self.x3_c1 + self.sko_c1 * self.r3[i]) if self.learn_d[i] == 1 else (self.x3_c2 + self.sko_c2 * self.r3[i])
        # X4
        for i in range(0, self.learn_x2.size):
            self.learn_x4[i] = (self.x4_c1 + self.sko_c1 * self.r4[i]) if self.learn_d[i] == 1 else (self.x4_c2 + self.sko_c2 * self.r4[i])
        # X5
        for i in range(0, self.learn_x2.size):
            self.learn_x5[i] = (self.x5_c1 + self.sko_c1 * self.r5[i]) if self.learn_d[i] == 1 else (self.x5_c2 + self.sko_c2 * self.r5[i])

    #   1.4 Два класса
    def specify_class(self):
        # Класс 1 x1
        for i in range(0, item_size):
            self.x1_class1[i] = self.learn_x1[i] if self.learn_d[i] == 1 else -100

        # Класс 1 x2
        for i in range(0, item_size):
            self.x2_class1[i] = self.learn_x2[i] if self.learn_d[i] == 1 else -100

        # Класс 2 x1
        for i in range(0, item_size):
            self.x1_class2[i] = self.learn_x1[i] if self.learn_d[i] == -1 else -100

        # Класс 2 x2
        for i in range(0, item_size):
            self.x2_class2[i] = self.learn_x2[i] if self.learn_d[i] == -1 else -100

    #   1.5 Обучение перцептрона
    # Первая итерация
    def first_iter(self):
        w_first = [0.1, 0.1, 0.1]
        self.s_wx[0] = np.nanprod(np.dstack((np.array([self.learn_x0[0], self.learn_x1[0], self.learn_x2[0]]), w_first)), 2).sum(1)
        self.y_predict[0] = 1 if self.s_wx[0] >= self.sigma[0] else -1
        self.e_err_predict[0] = (self.y_predict[0]-self.learn_d[0])/2
        self.e_count_predict[0] = 1 if self.e_err_predict[0] != 0 else 0
        self.dw0[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x0[0]
        self.dw1[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x1[0]
        self.dw2[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x2[0]
        self.w0[0] = w_first[0] + self.dw0[0]
        self.w1[0] = w_first[1] + self.dw1[0]
        self.w2[0] = w_first[2] + self.dw2[0]

    def first_iter_list4(self):
        w_first = [0.1, 0.1, 0.1, 0.1, 0.1, 0.1]
        self.s_wx[0] = np.nanprod(np.dstack((
            np.array([self.learn_x0[0], self.learn_x1[0], self.learn_x2[0], self.learn_x3[0], self.learn_x4[0], self.learn_x5[0]]), 
            w_first)), 2).sum(1)
        self.y_predict[0] = 1 if self.s_wx[0] >= self.sigma[0] else -1
        self.e_err_predict[0] = (self.y_predict[0]-self.learn_d[0])/2
        self.e_count_predict[0] = 1 if self.e_err_predict[0] != 0 else 0
        self.dw0[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x0[0]
        self.dw1[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x1[0]
        self.dw2[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x2[0]
        self.dw3[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x3[0]
        self.dw4[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x4[0]
        self.dw5[0] = 0 if self.e_err_predict[0] == 0 else train_speed * self.e_err_predict[0] * self.learn_x5[0]
        self.w0[0] = w_first[0] + self.dw0[0]
        self.w1[0] = w_first[1] + self.dw1[0]
        self.w2[0] = w_first[2] + self.dw2[0]
        self.w3[0] = w_first[3] + self.dw3[0]
        self.w4[0] = w_first[4] + self.dw4[0]
        self.w5[0] = w_first[5] + self.dw5[0]

    def iter_list3(self):
        for i in range(0, item_size):
            self.w0[i] = 0
            self.dw0[i] = 0

    # Все итерации
    def all_iter(self):
        for i in range(1, item_size):
            self.s_wx[i] = np.nanprod(np.dstack((np.array([self.learn_x0[i], self.learn_x1[i], self.learn_x2[i]]), np.array([self.w0[i-1], self.w1[i-1], self.w2[i-1]]))), 2).sum(1)
            self.y_predict[i] = 1 if self.s_wx[i] >= self.sigma[i] else -1
            self.e_err_predict[i] = (self.learn_d[i]-self.y_predict[i])/2
            self.e_count_predict[i] = 1 if self.e_err_predict[i] != 0 else 0
            self.dw0[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x0[i]
            self.dw1[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x1[i]
            self.dw2[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x2[i]
            self.w0[i] = self.w0[i-1] + self.dw0[i]
            self.w1[i] = self.w1[i-1] + self.dw1[i]
            self.w2[i] = self.w2[i-1] + self.dw2[i]

    def all_iter_list4(self):
        for i in range(1, item_size):
            self.s_wx[i] = np.nanprod(np.dstack((
                np.array([self.learn_x0[i], self.learn_x1[i], self.learn_x2[i], self.learn_x3[i], self.learn_x4[i], self.learn_x5[i]]), 
                np.array([self.w0[i-1], self.w1[i-1], self.w2[i-1], self.w3[i-1], self.w4[i-1], self.w5[i-1]]))), 2).sum(1)
            self.y_predict[i] = 1 if self.s_wx[i] >= self.sigma[i] else -1
            self.e_err_predict[i] = (self.learn_d[i]-self.y_predict[i])/2
            self.e_count_predict[i] = 1 if self.e_err_predict[i] != 0 else 0
            self.dw0[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x0[i]
            self.dw1[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x1[i]
            self.dw2[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x2[i]
            self.dw3[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x3[i]
            self.dw4[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x4[i]
            self.dw5[i] = 0 if self.e_err_predict[i] == 0 else train_speed * self.e_err_predict[i] * self.learn_x5[i]
            self.w0[i] = self.w0[i-1] + self.dw0[i]
            self.w1[i] = self.w1[i-1] + self.dw1[i]
            self.w2[i] = self.w2[i-1] + self.dw2[i]
            self.w3[i] = self.w3[i-1] + self.dw3[i]
            self.w4[i] = self.w4[i-1] + self.dw4[i]
            self.w5[i] = self.w5[i-1] + self.dw5[i]

    #   Результаты обучения
    def results(self):
        for i in range(0, epochs+1):
            self.epochs_err_count[i] = sum(self.e_count_predict[i*objects_count:(i+1)*objects_count])
            self.epochs_w0[i] = self.w0[i*(objects_count-1)]
            self.epochs_w1[i] = self.w1[i*(objects_count-1)]
            self.epochs_w2[i] = self.w2[i*(objects_count-1)]
        self.x1_graph = ( self.epochs_w0[epochs]/self.epochs_w2[epochs] - self.epochs_w1[epochs]/self.epochs_w2[epochs]) * (-5)
        self.x2_graph = (-self.epochs_w0[epochs]/self.epochs_w2[epochs] - self.epochs_w1[epochs]/self.epochs_w2[epochs]) * (15)

    #   Результаты обучения
    def results_list3(self):
        for i in range(0, epochs+1):
            self.epochs_err_count[i] = sum(self.e_count_predict[i*objects_count:(i+1)*objects_count])
            self.epochs_w0[i] = 0
            self.epochs_w1[i] = self.w1[i*(objects_count-1)]
            self.epochs_w2[i] = self.w2[i*(objects_count-1)]
        self.x1_graph = ( self.epochs_w0[epochs]/self.epochs_w2[epochs] - self.epochs_w1[epochs]/self.epochs_w2[epochs]) * (-5)
        self.x2_graph = (-self.epochs_w0[epochs]/self.epochs_w2[epochs] - self.epochs_w1[epochs]/self.epochs_w2[epochs]) * (15)

    def results_list4(self):
        for i in range(0, epochs+1):
            self.epochs_err_count[i] = sum(self.e_count_predict[i*objects_count:(i+1)*objects_count])
            self.epochs_w0[i] = self.w0[i*(objects_count-1)]
            self.epochs_w1[i] = self.w1[i*(objects_count-1)]
            self.epochs_w2[i] = self.w2[i*(objects_count-1)]
            self.epochs_w3[i] = self.w3[i*(objects_count-1)]
            self.epochs_w4[i] = self.w4[i*(objects_count-1)]
            self.epochs_w5[i] = self.w5[i*(objects_count-1)]

    def set_epochs(self, e):
        self.epochs = e


if __name__ == "__main__":
    t1 = Task()
    t1.fill_learn_data()
    t1.specify_class()
    t1.first_iter()
    t1.all_iter() 
    t1.results()

    t2 = Task()
    t2.make_batch()
    t2.fill_learn_data()
    t2.specify_class()
    
    
    t3 = copy.copy(t2)

    t2.first_iter()
    t2.all_iter()
    t2.results()

    t3.first_iter()
    t3.all_iter()
    t3.iter_list3()
    t3.results_list3()

    t4 = Task()
    t4.fill_learn_data()
    t4.specify_class()
    t4.first_iter_list4()
    t4.all_iter_list4()
    t4.results_list4()

    X = np.arange(len(t1.r1))
    Y1 = t1.r1
    Y2 = t1.r2
    Y3 = t1.learn_x1
    Y4 = t1.learn_x2
    X5 = t1.x1_class1
    Y5 = t1.x2_class1
    X6 = t1.x1_class2
    Y6 = t1.x2_class2

    print(epochs)

    figure, axis = plt.subplots(2, 4)

    axis[0, 0].scatter(t1.x1_class1, t1.x2_class1, s=5)
    axis[0, 0].scatter(t1.x1_class2, t1.x2_class2, s=5)
    axis[0, 0].plot([-5, 15], [t1.x1_graph, t1.x2_graph])
    axis[0, 0].set_xlim(-5, 20)
    axis[0, 0].set_ylim(-5, 20)
    axis[0, 0].set_title('Title 1')

    axis[0, 1].scatter(t2.x1_class1, t2.x2_class1, s=5)
    axis[0, 1].scatter(t2.x1_class2, t2.x2_class2, s=5)
    axis[0, 1].plot([-5, 15], [t2.x1_graph, t2.x2_graph])
    
    axis[0, 1].set_xlim(-5, 20)
    axis[0, 1].set_ylim(-5, 20)
    axis[0, 1].set_title('Title 2')

    axis[0, 2].scatter(t3.x1_class1, t3.x2_class1, s=5)
    axis[0, 2].scatter(t3.x1_class2, t3.x2_class2, s=5)
    axis[0, 2].plot([-5, 15], [t3.x1_graph, t3.x2_graph])
    axis[0, 2].set_xlim(-5, 20)
    axis[0, 2].set_ylim(-5, 20)
    axis[0, 2].set_title('Title 3')

    axis[0, 3].plot(np.arange(epochs+1), t4.epochs_w0)
    axis[0, 3].plot(np.arange(epochs+1), t4.epochs_w1)
    axis[0, 3].plot(np.arange(epochs+1), t4.epochs_w2)
    axis[0, 3].plot(np.arange(epochs+1), t4.epochs_w3)
    axis[0, 3].plot(np.arange(epochs+1), t4.epochs_w4)
    axis[0, 3].plot(np.arange(epochs+1), t4.epochs_w5)
    axis[0, 3].set_xlim(0, epochs)
    axis[0, 3].set_ylim(-50, 50)
    axis[0, 3].set_title('Title 3_1 :)')

    axis[1, 3].plot(np.arange(epochs+1), t4.epochs_err_count)
    axis[1, 3].set_xlim(0, epochs)
    axis[1, 3].set_ylim(0, 20)
    axis[1, 3].set_title('Title 3_1_2 :)')

    axis[1, 0].plot(np.arange(epochs+1), t1.epochs_err_count)
    axis[1, 0].set_title('Title 4')

    axis[1, 1].plot(np.arange(epochs+1), t2.epochs_err_count)
    axis[1, 1].set_title('Title 5')

    axis[1, 2].plot(np.arange(epochs+1), t3.epochs_err_count)
    axis[1, 2].set_title('Title 6')

    plt.tight_layout()
    plt.show()