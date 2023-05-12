import matplotlib.pyplot as plt
import numpy as np

#   1.1     Начальные значения
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
r1 = np.random.sample(item_size)
r2 = np.random.sample(item_size)
sigma = np.random.sample(item_size)
#   1.3     Обучающая выборка
learn_d = np.zeros(item_size)
for i in range(0, learn_d.size):
    if sigma[i] < delta_C1:
        learn_d[i] = 1
    else:
        learn_d[i] = -1
# X0
learn_x0 = np.ones(item_size)
# X1
learn_x1 = np.zeros(item_size)
for i in range(0, learn_x1.size):
    if learn_d[i] == 1:
        learn_x1[i] = x1_c1 + sko_c1 * r1[i]
    else:
        learn_x1[i] = x1_c2 + sko_c2 * r1[i]
# X2
learn_x2 = np.zeros(item_size)
for i in range(0, learn_x2.size):
    if learn_d[i] == 1:
        learn_x2[i] = x2_c1 + sko_c1 * r2[i]
    else:
        learn_x2[i] = x2_c2 + sko_c2 * r2[i]
#   1.4 Два класса
x1_class1 = np.zeros(item_size)
x2_class1 = np.zeros(item_size)
x1_class2 = np.zeros(item_size)
x2_class2 = np.zeros(item_size)
# Класс 1 x1
for i in range(0, item_size):
    if learn_d[i] == 1:
        x1_class1[i] = learn_x1[i]
    else:
        x1_class1[i] = -100
# Класс 1 x2
for i in range(0, item_size):
    if learn_d[i] == 1:
        x2_class1[i] = learn_x2[i]
    else:
        x2_class1[i] = -100
# Класс 2 x1
for i in range(0, item_size):
    if learn_d[i] == -1:
        x1_class2[i] = learn_x1[i]
    else:
        x1_class2[i] = -100
# Класс 2 x2
for i in range(0, item_size):
    if learn_d[i] == -1:
        x2_class2[i] = learn_x2[i]
    else:
        x2_class2[i] = -100
#   1.5 Обучение перцептрона
w_first = [0.2, 0.2, 0.2]
s_wx = np.zeros(item_size)
s_wx[0] = np.nanprod(np.dstack((np.array([learn_x0[0], learn_x1[0], learn_x2[0]]), w_first)), 2).sum(1)
print(s_wx[0])

X = np.arange(len(r1))

print(r1)

Y1 = r1
Y2 = r2
Y3 = sigma
Y4 = learn_x1
Y5 = learn_x2
Y6 = learn_d

figure, axis = plt.subplots(2, 3)

axis[0, 0].plot(X, Y1)
axis[0, 0].set_title('Title 1')
axis[0, 1].plot(X, Y2)
axis[0, 1].set_title('Title 2')
axis[0, 2].plot(X, Y3)
axis[0, 2].set_title('Title 3')
axis[1, 0].plot(X, Y4)
axis[1, 0].set_title('Title 4')
axis[1, 1].plot(X, Y5)
axis[1, 1].set_title('Title 5')
axis[1, 2].plot(X, Y6)
axis[1, 2].set_title('Title 6')

plt.tight_layout()
plt.show()
