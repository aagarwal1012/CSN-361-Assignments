import os
import csv

qlens = ['1','2','5','10','100']
bandwidths = ['1Kb', '10Kb', '100Kb', '1Mb', '10Mb']

file1 = open('prob1.csv','w')
w = csv.writer(file1)

for qlen in qlens:
    for bandwidth in bandwidths:
        with open('prob1.tcl','r') as file:
            data = file.readlines()
        l = []

        l.append(qlen)
        data[2] = "set ql1 " + qlen + "\n"

        l.append(qlen)
        data[3] = "set ql2 " + qlen + "\n"

        l.append(bandwidth)
        data[5] = "set bw1 " + bandwidth + "\n"

        l.append(bandwidth)
        data[6] = "set bw2 " + bandwidth + "\n"

        with open('prob1.tcl','w') as file:
            file.writelines(data)
        
        os.system("ns prob1.tcl")

        with open('packet_loss.txt','r') as file:
            data = file.readlines()

        print((data))
        l.append(data[1])
        l.append(data[2])
        l.append(data[4])
        l.append(data[5])

        w.writerow(l)

file1.close()