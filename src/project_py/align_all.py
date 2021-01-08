#Pawel Grabowski align sequences all to all, (representants of clusters identified by me)
from needleman_wunsh import make_alignment
from multiprocessing import Process, Value, Array

representants = open("representants.fasta")
representants = "".join(representants.readlines())
representants = [x.split("\n",1)[1].replace("\n","") for x in representants.split(">") if x != '']
results = [[-1 for x in range(len(representants))] for y in range(len(representants))]
out_matrix = open("align_result.csv", "w")
arr = Array('i', len(representants)**2 )
l_idx = len(representants)
processes = []
for idx, elem in enumerate(representants):
    for idx2, elem2 in enumerate(representants):
        if(idx2 >= idx):
            processes.append(Process(target=make_alignment, args=(elem, elem, idx, idx2, l_idx, -10, -1, arr)))
            processes[-1].start()

print("All threads started")

for idx in range(0, l_idx):
    for idx2 in range(0, l_idx):
        if(idx2 >= idx):
            processes[l_idx * idx + idx2].join()
            results[idx2][idx] = arr[l_idx * idx + idx2]
            results[idx][idx2] = results[idx2][idx]
            print("joined " + str(idx) + " " + str(idx2))
for idx in range(0, l_idx):
    for idx2 in range(0, l_idx):
        out_matrix.write(str(results[idx][idx2]) + ",")
    out_matrix.write("\n")
out_matrix.close()
