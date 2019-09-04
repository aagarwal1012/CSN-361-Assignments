import os
import csv

qlens = ['1','2','5','10','100']
bandwidths = ['1Kb', '10Kb', '100Kb', '1Mb', '10Mb']

file1 = open('prob2.csv','w')
w = csv.writer(file1)

for qlen in qlens:
    for bandwidth in bandwidths:
        with open('prob2.tcl','r') as file:
            data = file.readlines()
        l = []

        l.append(qlen)
        data[12] = "set ql02 " + qlen + "\n"

        l.append(qlen)
        data[13] = "set ql23 " + qlen + "\n"

        l.append(bandwidth)
        data[14] = "set bw02 " + bandwidth + "\n"

        l.append(bandwidth)
        data[15] = "set bw23 " + bandwidth + "\n"

        with open('prob2.tcl','w') as file:
            file.writelines(data)
        
        os.system("ns prob2.tcl")

        with open('packet_loss.txt','r') as file:
            data = file.readlines()

        print((data))
        l.append(data[1])
        l.append(data[2])
        l.append(data[4])
        l.append(data[5])

        w.writerow(l)

file1.close()