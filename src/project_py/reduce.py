#!/usr/bin/python3
# Script used by Pawel Grabowski to reduce the number
# of sequences by deleting duplicates and those
# containing X in them 

sfile = open("sequences.fasta", "r")
out = open("reduced_sequences.fasta", "w")
text = "".join(sfile.readlines())
text = text.split(">")
text = text[1:]
seq_set = set()
for elem in text:
    val = elem.split("\n", 1)[1].replace("\n ", "")
    if(val not in seq_set and 'X' not in val):
        seq_set.add(val)
        out.write(">" + elem)
print("Reducing finished, final size is " + str(len(seq_set)))
sfile.close()
out.close()

