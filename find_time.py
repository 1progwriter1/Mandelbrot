with open("time_no_avx.txt") as f:
    times = [int(i) for i in f.readlines()]

times.sort()

print(times[len(times) // 2] / pow(10, 9))