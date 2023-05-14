import matplotlib.pyplot as plt
import numpy as np

#   1.1     Начальные значения
variant = 30
np.random.seed(variant)

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
def fill_learn_data():
    # d    
    for i in range(0, learn_d.size):
        learn_d[i] = 1 if sigma[i] < delta_C1 else -1

    # X1
    for i in range(0, learn_x1.size):
        learn_x1[i] = (x1_c1 + sko_c1 * r1[i]) if learn_d[i] == 1 else (x1_c2 + sko_c2 * r1[i])
    # X2
    for i in range(0, learn_x2.size):
        learn_x2[i] = (x2_c1 + sko_c1 * r2[i]) if learn_d[i] == 1 else ( x2_c2 + sko_c2 * r2[i])

#   1.4 Два класса
def specify_class():
    # Класс 1 x1
    for i in range(0, item_size):
        x1_class1[i] = learn_x1[i] if learn_d[i] == 1 else -100

    # Класс 1 x2
    for i in range(0, item_size):
        x2_class1[i] = learn_x2[i] if learn_d[i] == 1 else -100

    # Класс 2 x1
    for i in range(0, item_size):
        x1_class2[i] = learn_x1[i] if learn_d[i] == -1 else -100

    # Класс 2 x2
    for i in range(0, item_size):
        x2_class2[i] = learn_x2[i] if learn_d[i] == -1 else -100

#   1.5 Обучение перцептрона

# Первая итерация
def first_iter():
    w_first = [0.1, 0.1, 0.1]
    s_wx[0] = np.nanprod(np.dstack((np.array([learn_x0[0], learn_x1[0], learn_x2[0]]), w_first)), 2).sum(1)
    y_predict[0] = 1 if s_wx[0] >= sigma[0] else -1
    e_err_predict[0] = (y_predict[0]-learn_d[0])/2
    e_count_predict[0] = 1 if e_err_predict[0] != 0 else 0
    dw0[0] = 0 if e_err_predict[0] == 0 else train_speed * e_err_predict[0] * learn_x0[0]
    dw1[0] = 0 if e_err_predict[0] == 0 else train_speed * e_err_predict[0] * learn_x1[0]
    dw2[0] = 0 if e_err_predict[0] == 0 else train_speed * e_err_predict[0] * learn_x2[0]
    w0[0] = w_first[0] + dw0[0]
    w1[0] = w_first[1] + dw1[0]
    w2[0] = w_first[2] + dw2[0]

# Все итерации
def all_iter():
    for i in range(1, item_size):
        s_wx[i] = np.nanprod(np.dstack((np.array([learn_x0[i], learn_x1[i], learn_x2[i]]), np.array([w0[i-1], w1[i-1], w2[i-1]]))), 2).sum(1)
        y_predict[i] = 1 if s_wx[i] >= sigma[i] else -1
        e_err_predict[i] = (learn_d[i]-y_predict[i])/2
        e_count_predict[i] = 1 if e_err_predict[i] != 0 else 0
        dw0[i] = 0 if e_err_predict[i] == 0 else train_speed * e_err_predict[i] * learn_x0[i]
        dw1[i] = 0 if e_err_predict[i] == 0 else train_speed * e_err_predict[i] * learn_x1[i]
        dw2[i] = 0 if e_err_predict[i] == 0 else train_speed * e_err_predict[i] * learn_x2[i]
        w0[i] = w0[i-1] + dw0[i]
        w1[i] = w1[i-1] + dw1[i]
        w2[i] = w2[i-1] + dw2[i]

#   Результаты обучения
def results():
    global x1_graph, x2_graph
    for i in range(0, epochs+1):
        epochs_err_count[i] = sum(e_count_predict[i*objects_count:(i+1)*objects_count])
        epochs_w0[i] = w0[i*(objects_count-1)]
        epochs_w1[i] = w1[i*(objects_count-1)]
        epochs_w2[i] = w2[i*(objects_count-1)]
    x1_graph = (epochs_w0[epochs]/epochs_w2[epochs] - epochs_w1[epochs]/epochs_w2[epochs]) * (-5)
    x2_graph = (-epochs_w0[epochs]/epochs_w2[epochs] - epochs_w1[epochs]/epochs_w2[epochs]) * (15)


if __name__ == "__main__":
    fill_learn_data()
    specify_class()
    first_iter()
    all_iter()
    results()

    X = np.arange(len(r1))
    Y1 = r1
    Y2 = r2
    Y3 = learn_x1
    Y4 = learn_x2

    X5 = x1_class1
    Y5 = x2_class1

    X6 = x1_class2
    Y6 = x2_class2

    figure, axis = plt.subplots(2, 3)

    axis[0, 0].scatter(x1_class1, x2_class1, s=5)
    axis[0, 0].scatter(x1_class2, x2_class2, s=5)
    axis[0, 0].plot([-5, 15], [x1_graph, x2_graph])
    axis[0, 0].set_xlim(-5, 20)
    axis[0, 0].set_ylim(-5, 20)
    axis[0, 0].set_title('Title 1')

    axis[0, 1].plot(X, Y2)
    axis[0, 1].set_title('Title 2')

    axis[0, 2].plot(X, Y3)
    axis[0, 2].set_title('Title 3')

    axis[1, 0].plot(np.arange(epochs+1), epochs_err_count)
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