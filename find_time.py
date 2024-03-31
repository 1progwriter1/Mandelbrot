with open("time_no_avx.txt") as f:
    times = [int(i) for i in f.readlines()]

times.sort()
aver = sum(times) / len(times)
print(aver)
print(max(max(times) - aver, aver - min(times)) / 100)