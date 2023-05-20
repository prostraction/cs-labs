import matplotlib.pyplot as plt
import numpy as np
import copy

#   Начальные значения
variant = 9
np.random.seed(variant)

# Параметры обучения
delta_C1 = 0.45
train_speed = 0.55
epochs = int(np.random.uniform(7, 12))
objects_count = int(np.random.uniform(15, 25))
item_size = int(epochs * objects_count)
err_probability = np.random.normal(0, 1)
batch_size = 7

# Изображения
sym_count = 8
symbols = [[0,0,1,0,0,1,0,0,1,0,0,1,0,0,1],
            [1,1,1,0,0,1,1,1,1,1,0,0,1,1,1],
            [1,1,1,0,0,1,1,1,1,0,0,1,1,1,1],
            [1,0,1,1,0,1,1,1,1,0,0,1,0,0,1],
            [1,1,1,1,0,0,1,1,1,0,0,1,1,1,1],
            [1,1,1,1,0,0,1,1,1,1,0,1,1,1,1],
            [1,1,1,0,0,1,0,0,1,0,0,1,0,0,1],
            [1,1,1,1,0,1,1,1,1,1,0,1,1,1,1],
            [1,1,1,1,0,1,1,1,1,0,0,1,1,1,1],
            [1,1,1,1,0,1,1,0,1,1,0,1,1,1,1],
            [0,0,0,0,1,0,1,1,1,0,1,0,0,0,0],
            [0,0,0,0,0,0,1,1,1,0,0,0,0,0,0],
            [0,0,0,1,0,1,0,1,0,1,0,1,0,0,0],
            [0,0,0,0,0,1,0,1,0,1,0,0,0,0,0],
            [0,1,0,1,0,1,1,1,1,1,0,1,1,0,1],
            [1,1,1,1,0,0,1,0,0,1,0,0,1,0,0],
            [1,1,1,1,0,0,1,1,0,1,0,0,1,1,1],
            [1,0,1,1,0,1,1,1,0,1,0,1,1,0,1],
            [1,0,1,1,0,1,1,1,1,1,0,1,1,0,1],
            [1,1,1,1,0,1,1,0,1,1,0,1,1,0,1],
            [1,1,1,1,0,1,1,1,1,1,0,0,1,0,0],
            [1,1,1,1,0,0,1,0,0,1,0,0,1,1,1],
            [1,1,1,0,1,0,0,1,0,0,1,0,0,1,0],
            [1,0,1,1,0,1,1,1,1,0,0,1,1,1,1],
            [1,0,1,1,0,1,0,1,0,1,0,1,1,0,1],
            [1,0,0,1,0,0,1,1,1,1,0,1,1,1,1],
            [1,1,0,0,0,1,0,1,1,0,0,1,1,1,0],
            [1,1,1,1,0,1,1,0,1,0,1,1,1,0,1],
            [0,0,1,0,0,1,1,1,1,1,0,1,1,1,1],
            [1,1,0,1,0,1,1,0,1,1,0,1,1,1,0],
            [1,0,0,1,0,0,1,1,1,1,0,1,1,0,1],
            [1,1,1,1,0,0,1,1,0,1,0,0,1,0,0],
            [0,1,1,0,1,0,1,1,1,0,1,0,0,1,0],
            [0,0,1,0,0,1,0,0,1,1,0,1,1,1,1],
            [0,1,0,0,0,0,0,1,0,0,1,0,0,1,1],
            [1,0,0,1,0,0,1,0,0,1,0,0,1,1,1],
            [1,1,1,1,0,1,1,0,1,1,1,0,1,0,1],
            [1,0,1,1,0,1,1,0,1,1,0,1,0,1,0],
            [1,0,1,1,0,1,0,1,0,0,1,0,0,1,0],
            [1,1,1,0,0,1,0,1,0,1,0,0,1,1,1]]

class Task:
    def __init__(self):
        self.sym_prob = np.random.uniform(0, 1, len(symbols))
        
        self.sym_ind = []
        for i in range(len(self.sym_prob)):
            if self.sym_prob[i] < 0.25:
                self.sym_ind.append(i)
        self.sym_ind = self.sym_ind[:sym_count]        
        
        self.sym = []
        for i in range(0, sym_count):
            self.sym.append(symbols[self.sym_ind[i]])
        print(self.sym)
        
        self.d = []
        for i in range(0, sym_count):
            self.d.append(np.zeros(item_size))
        for i in range(0, sym_count):
            for j in range(item_size):
                self.d[i][j] = 1 if j % sym_count == i else -1
        
        self.dw = []
        for i in range(0, 16):
            self.dw.append(np.zeros(item_size))
        self.x2 = np.zeros(item_size)
        self.y2 = np.zeros(item_size)
        self.e2 = np.zeros(item_size)
        self.err_count = np.zeros(item_size)
    def solve(self):
        pass
        

if __name__ == "__main__":
    t = Task()
    t.solve()
        