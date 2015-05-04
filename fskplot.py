import csv
import matplotlib.pyplot as plt

# NOTE: change this to reflect the sampling period for this data
timescale = 0.0002

with open('fsk.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)
    time = []
    samples = []

    for row in reader:
        time.append(round(float(row[0]),7))
        samples.append(round(float(row[1]),7))

    last = 0
    bits = []
    for point in range(len(time)):
        if samples[point] == 1:
            diff = time[point]-last
            if diff > 7.2*10**-5:
                bits.append(1)
            else:
                bits.append(0)
            last = time[point]

    samplestr = ' '.join(str(i) for i in bits)
    substr = ' '.join(str(i) for i in bits[548:1088])
    print samplestr.count(substr)

    print bits[548:1088]

    # print sorted(diffs)
    # print bits

    # plt.plot(range(len(bits)),bits)
    # plt.xlabel("Time",fontsize=18)
    # plt.ylabel("Logic Levels",fontsize=18)
    # plt.title("RFID Readings",fontsize=24)
    # plt.grid(True)
    # plt.show()