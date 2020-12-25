#Pawel Grabowski align sequences all to all, (representants of clusters identified by me)
from needleman_wunsh import make_alignment

representants = open("representants.fasta")
representants = "".join(representants.readlines())
representants = [x.split("\n",1)[1].replace("\n","") for x in representants.split(">") if x != '']
results = [[0 for x in range(len(representants))] for y in range(len(representants))]
out_matrix = open("align_result.csv", "w")
for idx, elem in enumerate(representants):
    for idx2, elem2 in enumerate(representants):
        print("works " + str(idx) + " " + str(idx2))
        results[idx][idx2] = make_alignment(elem, elem, -10, -1)[2]
        out_matrix.write(str(results[idx][idx2]) + ",")
out_matrix.close()
