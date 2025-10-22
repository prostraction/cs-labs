import os
import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms
import numpy as np
from PIL import Image, ImageOps, ImageGrab
import win32gui
from tkinter import *
import tkinter as tk

class CNN(nn.Module):
    def __init__(self):
        super(CNN, self).__init__()
        self.conv1 = nn.Conv2d(1, 16, 3, padding=1)
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(16, 32, 3, padding=1)
        self.fc1 = nn.Linear(32 * 7 * 7, 128)
        self.fc2 = nn.Linear(128, 10)

    def forward(self, x):
        x = self.pool(nn.functional.relu(self.conv1(x)))
        x = self.pool(nn.functional.relu(self.conv2(x)))
        x = x.view(-1, 32 * 7 * 7)
        x = nn.functional.relu(self.fc1(x))
        x = self.fc2(x)
        return x
    
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model = CNN().to(device)
optimizer = optim.Adam(model.parameters(), lr=0.001)
criterion = nn.CrossEntropyLoss()

def predict_digit(img, model):
    # изменение размера изображений на 28x28
    img = img.resize((28, 28))
    # конвертируем rgb в grayscale
    img = img.convert('L')
    img = ImageOps.invert(img)
    img = np.array(img)
    # изменение размерности для поддержки модели ввода и нормализации
    img = img.reshape(1, 1, 28, 28)
    img = torch.tensor(img, dtype=torch.float32)
    img = img / 255.
    img = img.to(device)
    # предсказание цифры
    with torch.no_grad():
        output = model(img)
        _, predicted = torch.max(output, 1)
        confidence = torch.nn.functional.softmax(output, dim=1)[0][predicted[0]].item()
    return predicted.item(), confidence

def train(model, train_loader, optimizer, criterion, epoch):
    model.train()
    for batch_idx, (data, target) in enumerate(train_loader):
        data, target = data.to(device), target.to(device)
        optimizer.zero_grad()
        output = model(data)
        loss = criterion(output, target)
        loss.backward()
        optimizer.step()
        if batch_idx % 100 == 0:
            print('Train Epoch: {} [{}/{} ({:.0f}%)]\tLoss: {:.6f}'.format(
                epoch, batch_idx * len(data), len(train_loader.dataset),
                100. * batch_idx / len(train_loader), loss.item()))

class App(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        
        self.x = self.y = 0
        
        # Создание элементов
        self.canvas = tk.Canvas(self, width=300, height=300, bg = "white", cursor="cross")
        self.label = tk.Label(self, text="Думаю..", font=("Helvetica", 48))
        self.classify_btn = tk.Button(self, text = "Распознать", command=self.classify_handwriting)
        self.button_clear = tk.Button(self, text = "Очистить", command=self.clear_all)
        
        # Сетка окна
        self.canvas.grid(row=0, column=0, pady=2, sticky=W, )
        self.label.grid(row=0, column=1,pady=2, padx=2)
        self.classify_btn.grid(row=1, column=1, pady=2, padx=2)
        self.button_clear.grid(row=1, column=0, pady=2)
        
        # self.canvas.bind("<Motion>", self.start_pos)
        self.canvas.bind("<B1-Motion>", self.draw_lines)
        
    def clear_all(self):
        self.canvas.delete("all")
        
    def classify_handwriting(self):
        HWND = self.canvas.winfo_id() 
        rect = win32gui.GetWindowRect(HWND) # получаем координату холста
        im = ImageGrab.grab(rect)
        
        digit, acc = predict_digit(im, model)
        self.label.configure(text= str(digit)+', '+ str(int(acc*100))+'%')
        
    def draw_lines(self, event):
        self.x = event.x
        self.y = event.y
        r=8
        self.canvas.create_oval(self.x-r, self.y-r, self.x + r, self.y + r, fill='black')
            
if __name__ == '__main__':
    transform = transforms.Compose([
        transforms.ToTensor(),
        transforms.RandomResizedCrop(size=(28, 28), antialias=True)#,
        #transforms.Normalize((0.1307,), (0.3081,))
    ])

    train_loader = torch.utils.data.DataLoader(
        datasets.MNIST(root='./data', train=True, download=True, transform=transform),
        batch_size=64, shuffle=True)
    if os.path.exists("mnist_cnn.pth"):
        model.load_state_dict(torch.load("mnist_cnn.pth"))
    else:
        for epoch in range(1, 31):
            train(model, train_loader, optimizer, criterion, epoch)
            if epoch == 30:
                torch.save(model.state_dict(), "mnist_cnn.pth")
    app = App()
    mainloop()
