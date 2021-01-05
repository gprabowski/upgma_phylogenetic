#Pawel Grabowski align sequences all to all, (representants of clusters identified by me)
from needleman_wunsh import make_alignment
from multiprocessing import Array 

representants = open("representants.fasta")
representants = "".join(representants.readlines())
representants = [x.split("\n",1)[1].replace("\n","") for x in representants.split(">") if x != '']
results = [[-1 for x in range(len(representants))] for y in range(len(representants))]
out_matrix = open("align_result.csv", "w")
arr = Array('i', len(representants)**2 )
l_idx = len(representants)
for idx, elem in enumerate(representants[0:1]):
    for idx2, elem2 in enumerate(representants[5:6]):
        make_alignment(elem, elem, idx, idx2, l_idx, -10, -1, arr)
