import math
import re
import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import os
import sys

def app_path():
    if hasattr(sys, 'frozen'):
        return os.path.dirname(sys.executable)  # 使用pyinstaller打包后的exe目录
    return os.path.dirname(__file__)  # 没打包前的py目录


def is_all_zeros(Sequence):
    for i in Sequence:
        if i != 0:
            return False
    return True


def sort_max_index(Sequence, maxv):
    temp = Sequence[:]
    index = []  # 将度序列由大到小排列
    for i in range(maxv):  # 最大度
        index.append(temp.index(max(temp)))  # 将序列的最大值的index存入
        temp[index[i]] = 0  # 当前最大值复制为0，搜寻次大值
    return index


def Draw_Graph(Sequence, R=2):
    # 画出顶点, R决定画布的大小
    num = len(Sequence)
    alpha = []
    xarr = []
    yarr = []
    for i in range(0, num):
        alpha.append((2 * math.pi / num) * i)
        xarr.append(R * math.cos(alpha[i]))
        yarr.append(R * math.sin(alpha[i]))
    # plt.plot(xarr, yarr, 'ro')
    # plt.axis([-1.3*R, 1.3*R, -1.3*R, 1.3*R])
    a.plot(xarr, yarr, 'ro')
    a.axis('off')  # 不显示刻度
    # a.axis([-1.3*R, 1.3*R, -1.3*R, 1.3*R])

    Sequence.sort(reverse=True)  # 将序列排序，从大到小
    while not is_all_zeros(Sequence):
        maxv = max(Sequence)  # 最大度
        maxi = Sequence.index(maxv)  # 最大度顶点的下标
        Sequence[maxi] = 0  # TH4 delete d1 ===>  d1=0
        for i in sort_max_index(Sequence, maxv):  # d2---d(d1+1)的index
            if Sequence[i] > 0:
                # plt.plot([xarr[maxi], xarr[i]], [yarr[maxi], yarr[i]], linewidth='1.0', color='green')  # 最大度点和其余>0度之间连线
                a.plot([xarr[maxi], xarr[i]], [yarr[maxi], yarr[i]], linewidth='1.0',
                       color='green')  # 最大度点和其余>0度之间连线
                Sequence[i] = Sequence[i] - 1  # d2---d(d1+1) 减一
            else:
                return False
    return True


def check_sequence():
    global CLEAR
    if CLEAR:
        CLEAR = False
        a.cla()
        a.axis('off')  # 不显示刻度
        canvas.draw()
        # entry.delete(0, "end")  # 清空输入框
        button.config(text='检查序列')
        label.config(text="请输入一个有限非负整数序列，用逗号分隔：")
    else:
        sequence = entry.get()  # 获取输入的序列
        sequence = sequence.strip()  # 去除前后的空格

        try:
            sequence = [int(x) for x in re.split(',|，', sequence)]  # 将输入的序列字符串转换为整数列表
            if min(sequence) < 0:
                label.config(text="有负数，请重新输入一个有限非负整数序列，用逗号分隔：")
                return
        except ValueError:
            label.config(text="无效的输入，请重新输入一个有限非负整数序列，用逗号分隔：")  # 输入不是有效的整数序列
            return

        if Draw_Graph(sequence, 2):
            # plt.show()
            canvas.draw()
            CLEAR = True
            button.config(text='清除下方的图序列')
            label.config(text="该序列为图序列，下方给出示例图")

        else:
            label.config(text="序列：{}不是图序列".format(entry.get().strip()))
            a.cla()
            a.axis('off')  # 不显示刻度
    return


CLEAR = False  # 全局变量

# 创建窗口
window = tk.Tk()
window.title("判断是否为图序列")
window.geometry("800x600")

# 创建标签和输入框
label = tk.Label(window, text="请输入一个有限非负整数序列，用逗号分隔：")
label.pack()  # 将部件放置到主窗口中

entry = tk.Entry(window)
entry.pack()  # 将部件放置到主窗口中

# 创建子图
f = Figure(figsize=(5, 4), dpi=100)
a = f.add_subplot(111)  # 添加子图:1行1列第1
a.axis('off')  # 不显示刻度
# plt.xticks(())  # 不显示坐标轴
# plt.yticks(())

# 创建按钮
button = tk.Button(window, text="检查序列", command=check_sequence)
# label.config(text="请输入一个有限非负整数序列，用逗号分隔：")
button.pack()  # 将部件放置到主窗口中

# 将绘制的图形显示到tkinter:创建属于root的canvas画布,并将图f置于画布上
canvas = FigureCanvasTkAgg(f, master=window)
# canvas.draw()  # 注意show方法已经过时了,这里改用draw
canvas.get_tk_widget().pack(side=tk.TOP,  # 上对齐
                            fill=tk.BOTH,  # 填充方式
                            expand=tk.YES)  # 随窗口大小调整而调整

# 运行窗口,进入消息循环
window.mainloop()