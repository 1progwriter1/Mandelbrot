with open("Results/avx_ticks_O3.txt") as f:
    ticks = [float(i) for i in f.readlines()]

with open("Results/avx_time_O3.txt") as f:
    times = [float(i) for i in f.readlines()]

aver = sum(times) / len(times)
print("time = ", aver)
aver_ticks = sum(ticks) / len(ticks)
print("ticks = ", aver_ticks / pow(10, 9), " accuracy = ", max(max(ticks) - aver_ticks, aver_ticks - min(ticks)) / pow(10, 9))