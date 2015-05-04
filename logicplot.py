import csv
import matplotlib.pyplot as plt

lenHigh = 8*10**-5
lenLow = 6.67*10**-5

with open('logic.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)
    time = []
    samples = []

    for row in reader:
        time.append(round(float(row[0]),7))
        samples.append(round(float(row[1]),7))

    diffs = []
    for point in range(len(time)):
        if point > 0:
            diff = (round(time[point]-time[point-1],7),samples[point-1])   # tuple with time difference and bit type
            diffs.append(diff)

    # normalize to account for the different lengths of high and low
    processed = []
    for diff in diffs:
        if diff[1] == 1:
            processed.append((int(round(diff[0] / lenHigh)),1))
        else:
            processed.append((int(round(diff[0] / lenLow)),0))

    regen = []
    for run in processed:
        regen += [run[1]]*run[0]

    samplestr = ' '.join(str(i) for i in regen)
    substr = ' '.join(str(i) for i in regen[548:1088])
    print samplestr.count(substr)

    print regen[4563:4583]
    print regen[3955:3975]

    plt.plot(range(len(regen)),regen,'b')
    plt.xlabel("Time",fontsize=18)
    plt.ylabel("Logic Levels",fontsize=18)
    plt.title("RFID Readings",fontsize=24)
    plt.grid(True)
    plt.show()