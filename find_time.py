with open("Results/arrays_ticks_O3_loops.txt") as f:
    ticks = [float(i) for i in f.readlines()]

with open("Results/arrays_time_O3_loops.txt") as f:
    times = [float(i) for i in f.readlines()]

aver = sum(times) / len(times)
print("time = ", aver)
aver_ticks = sum(ticks) / len(ticks)
print("ticks = ", aver_ticks / pow(10, 9), " accuracy = ", max(max(ticks) - aver_ticks, aver_ticks - min(ticks)) / pow(10, 9))