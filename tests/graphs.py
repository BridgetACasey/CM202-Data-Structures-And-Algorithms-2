import numpy as np
import csv 
import pandas as pd
from matplotlib import pyplot as plt
from scipy.stats import sem,t 

file = pd.read_csv('mandelbrot-data-master.csv')

cpu = file.CPU
vm = file.VM
setup = file.SETUP
calc = file.CALC

threads = [3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]

plt.title('Setup Time')
plt.xlabel('Thread Count')
plt.ylabel('Time (ms)')
plt.grid(color = 'black', linestyle = ':')
plt.plot(threads, setup, color = 'blue')
plt.savefig('setup-linechart.png')

plt.show()
