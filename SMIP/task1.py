import matplotlib.pyplot as plt
from matplotlib.widgets import Button, TextBox
import numpy as np
import copy

class draw_graphs:
    def __init__(self):
        self.page = 0
        self.max_pages = 4
        self.axes_page = figure.add_axes([0.4, 0.01, 0.15, 0.04])
        self.axes_page.axis('off')
        self.draw_page(0)

    def select_next_page(self, event):
        if self.page+1 < self.max_pages:
            self.page += 1
            for i in range (0, 2):
                for j in range (0, 4):
                    axis[i, j].clear()
            self.draw_page(self.page)
            plt.draw()

    def select_prev_page(self, event):
        if self.page > 0:
            self.page -= 1
            for i in range (0, 2):
                for j in range (0, 4):
                    axis[i, j].clear()
            self.draw_page(self.page)
            plt.draw()

    def draw_page(self, page):
        self.axes_page.set_title('Страница ' + str(self.page+1))
        if page == 0:
            self.draw_page_1()
        if page == 1:
            self.draw_page_2()
        if page == 2:
            self.draw_page_3()
        if page == 3:
            self.draw_page_4()

    def draw_page_1(self):
        x = np.arange(0,1*np.pi,0.1)
        y = np.sin(x)
        for i in range (0, 2):
            for j in range (0, 4):
                axis[i, j].plot(x, y)
    def draw_page_2(self):
        x = np.arange(0,2*np.pi,0.1)
        y = np.sin(x)
        for i in range (0, 2):
            for j in range (0, 4):
                axis[i, j].plot(x, y)
    def draw_page_3(self):
        x = np.arange(0,3*np.pi,0.1)
        y = np.sin(x)
        for i in range (0, 2):
            for j in range (0, 4):
                axis[i, j].plot(x, y)
    def draw_page_4(self):
        x = np.arange(0,4*np.pi,0.1)
        y = np.sin(x)
        for i in range (0, 2):
            for j in range (0, 4):
                axis[i, j].plot(x, y)

if __name__ == "__main__":
    figure, axis = plt.subplots(2, 4)
    figure.set_figheight(7)
    figure.set_figwidth(14)

    draw_callback = draw_graphs()
    axes_prev_page = figure.add_axes([0.1, 0.01, 0.15, 0.04])
    axes_next_page = figure.add_axes([0.75, 0.01, 0.15, 0.04])
    but_prev_page = Button(axes_prev_page, 'Предыдущая страница')
    but_next_page = Button(axes_next_page, 'Следующая страница')
    but_prev_page.on_clicked(draw_callback.select_prev_page)
    but_next_page.on_clicked(draw_callback.select_next_page)


    #for i in range(0, 4):
    #    for j in range(0, 4):
    #        axis[i, j].set_title('123123123123')
#
    #axis
    
    #plt.tight_layout()
    plt.subplots_adjust(left = 0.05, bottom = 0.12, top = 0.95, right = 0.95)#, hspace = 0.4)
    plt.show()