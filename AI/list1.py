import matplotlib.pyplot as plt
import numpy as np

#   1.1     Начальные значения
variant = 30
np.random.seed(variant)

class Task:
    # Класс 1
    x1_c1 = np.random.uniform(0, 10)
    x2_c1 = np.random.uniform(5, 15)
    sko_c1 = np.random.uniform(1, 3)

    # Класс 2
    x1_c2 = np.random.uniform(0, 10)
    x2_c2 = np.random.uniform(5, 15)
    sko_c2 = np.random.uniform(1, 3)

    # Параметры обучения
    delta_C1 = np.random.uniform(0.2, 0.8)
    train_speed = np.random.uniform(0.1, 0.9)
    epochs = int(np.random.uniform(7, 12))
    objects_count = int(np.random.uniform(15, 25))
    item_size = int(epochs * objects_count)

    #   1.2     Случайные числа
    r1 = np.random.normal(0, 1, item_size)
    r2 = np.random.normal(0, 1, item_size)
    sigma = np.random.sample(item_size)

    # Переменные для обучения
    learn_d = np.zeros(item_size)
    learn_x0 = np.ones(item_size)
    learn_x1 = np.zeros(item_size)
    learn_x2 = np.zeros(item_size)

    x1_class1 = np.zeros(item_size)
    x2_class1 = np.zeros(item_size)
    x1_class2 = np.zeros(item_size)
    x2_class2 = np.zeros(item_size)

    s_wx = np.zeros(item_size)
    y_predict = np.zeros(item_size)
    e_err_predict = np.zeros(item_size)
    e_count_predict = np.zeros(item_size)
    dw0 = np.zeros(item_size)
    dw1 = np.zeros(item_size)
    dw2 = np.zeros(item_size)
    w0 = np.zeros(item_size)
    w1 = np.zeros(item_size)
    w2 = np.zeros(item_size)

    epochs_err_count = np.zeros(epochs+1)
    epochs_w0 = np.zeros(epochs+1)
    epochs_w1 = np.zeros(epochs+1)
    epochs_w2 = np.zeros(epochs+1)
    x1_graph = 0
    x2_graph = 0

    #   1.3     Обучающая выборка
    def fill_learn_data(self):
        # d    
        for i in range(0, self.learn_d.size):
            self.learn_d[i] = 1 if self.sigma[i] < self.delta_C1 else -1

        # X1
        for i in range(0, self.learn_x1.size):
            self.learn_x1[i] = (self.x1_c1 + self.sko_c1 * self.r1[i]) if self.learn_d[i] == 1 else (self.x1_c2 + self.sko_c2 * self.r1[i])
        # X2
        for i in range(0, self.learn_x2.size):
            self.learn_x2[i] = (self.x2_c1 + self.sko_c1 * self.r2[i]) if self.learn_d[i] == 1 else (self.x2_c2 + self.sko_c2 * self.r2[i])

    #   1.4 Два класса
    def specify_class(self):
        # Класс 1 x1
        for i in range(0, self.item_size):
            self.x1_class1[i] = self.learn_x1[i] if self.learn_d[i] == 1 else -100

        # Класс 1 x2
        for i in range(0, self.item_size):
            self.x2_class1[i] = self.learn_x2[i] if self.learn_d[i] == 1 else -100

        # Класс 2 x1
        for i in range(0, self.item_size):
            self.x1_class2[i] = self.learn_x1[i] if self.learn_d[i] == -1 else -100

        # Класс 2 x2
        for i in range(0, self.item_size):
            self.x2_class2[i] = self.learn_x2[i] if self.learn_d[i] == -1 else -100

    #   1.5 Обучение перцептрона
    # Первая итерация
    def first_iter(self):
        w_first = [0.1, 0.1, 0.1]
        self.s_wx[0] = np.nanprod(np.dstack((np.array([self.learn_x0[0], self.learn_x1[0], self.learn_x2[0]]), w_first)), 2).sum(1)
        self.y_predict[0] = 1 if self.s_wx[0] >= self.sigma[0] else -1
        self.e_err_predict[0] = (self.y_predict[0]-self.learn_d[0])/2
        self.e_count_predict[0] = 1 if self.e_err_predict[0] != 0 else 0
        self.dw0[0] = 0 if self.e_err_predict[0] == 0 else self.train_speed * self.e_err_predict[0] * self.learn_x0[0]
        self.dw1[0] = 0 if self.e_err_predict[0] == 0 else self.train_speed * self.e_err_predict[0] * self.learn_x1[0]
        self.dw2[0] = 0 if self.e_err_predict[0] == 0 else self.train_speed * self.e_err_predict[0] * self.learn_x2[0]
        self.w0[0] = w_first[0] + self.dw0[0]
        self.w1[0] = w_first[1] + self.dw1[0]
        self.w2[0] = w_first[2] + self.dw2[0]

    # Все итерации
    def all_iter(self):
        for i in range(1, self.item_size):
            self.s_wx[i] = np.nanprod(np.dstack((np.array([self.learn_x0[i], self.learn_x1[i], self.learn_x2[i]]), np.array([self.w0[i-1], self.w1[i-1], self.w2[i-1]]))), 2).sum(1)
            self.y_predict[i] = 1 if self.s_wx[i] >= self.sigma[i] else -1
            self.e_err_predict[i] = (self.learn_d[i]-self.y_predict[i])/2
            self.e_count_predict[i] = 1 if self.e_err_predict[i] != 0 else 0
            self.dw0[i] = 0 if self.e_err_predict[i] == 0 else self.train_speed * self.e_err_predict[i] * self.learn_x0[i]
            self.dw1[i] = 0 if self.e_err_predict[i] == 0 else self.train_speed * self.e_err_predict[i] * self.learn_x1[i]
            self.dw2[i] = 0 if self.e_err_predict[i] == 0 else self.train_speed * self.e_err_predict[i] * self.learn_x2[i]
            self.w0[i] = self.w0[i-1] + self.dw0[i]
            self.w1[i] = self.w1[i-1] + self.dw1[i]
            self.w2[i] = self.w2[i-1] + self.dw2[i]

    #   Результаты обучения
    def results(self):
        global x1_graph, x2_graph
        for i in range(0, self.epochs+1):
            self.epochs_err_count[i] = sum(self.e_count_predict[i*self.objects_count:(i+1)*self.objects_count])
            self.epochs_w0[i] = self.w0[i*(self.objects_count-1)]
            self.epochs_w1[i] = self.w1[i*(self.objects_count-1)]
            self.epochs_w2[i] = self.w2[i*(self.objects_count-1)]
        x1_graph = ( self.epochs_w0[self.epochs]/self.epochs_w2[self.epochs] - self.epochs_w1[self.epochs]/self.epochs_w2[self.epochs]) * (-5)
        x2_graph = (-self.epochs_w0[self.epochs]/self.epochs_w2[self.epochs] - self.epochs_w1[self.epochs]/self.epochs_w2[self.epochs]) * (15)


if __name__ == "__main__":
    t1 = Task()
    t1.fill_learn_data()
    t1.specify_class()
    t1.first_iter()
    t1.all_iter()
    t1.results()

    X = np.arange(len(t1.r1))
    Y1 = t1.r1
    Y2 = t1.r2
    Y3 = t1.learn_x1
    Y4 = t1.learn_x2
    X5 = t1.x1_class1
    Y5 = t1.x2_class1
    X6 = t1.x1_class2
    Y6 = t1.x2_class2

    figure, axis = plt.subplots(2, 3)

    axis[0, 0].scatter(t1.x1_class1, t1.x2_class1, s=5)
    axis[0, 0].scatter(t1.x1_class2, t1.x2_class2, s=5)
    axis[0, 0].plot([-5, 15], [x1_graph, x2_graph])
    axis[0, 0].set_xlim(-5, 20)
    axis[0, 0].set_ylim(-5, 20)
    axis[0, 0].set_title('Title 1')

    axis[0, 1].plot(X, Y2)
    axis[0, 1].set_title('Title 2')

    axis[0, 2].plot(X, Y3)
    axis[0, 2].set_title('Title 3')

    axis[1, 0].plot(np.arange(t1.epochs+1), t1.epochs_err_count)
    axis[1, 0].set_title('Title 4')

    axis[1, 1].scatter(X5, Y5, s=5)
    axis[1, 1].set_xlim(-5, 20)
    axis[1, 1].set_ylim(-5, 20)
    axis[1, 1].set_title('Title 5')

    axis[1, 2].plot(x1_graph, x1_graph)
    axis[1, 2].set_xlim(-5, 20)
    axis[1, 2].set_ylim(-5, 20)
    axis[1, 2].set_title('Title 6')

    plt.tight_layout()
    plt.show()