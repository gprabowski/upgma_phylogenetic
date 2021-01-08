import sys

if(len(sys.argv) != 3):
    raise ValueError("not enough args")

i = open(sys.argv[1])
itext = i.readlines()
s = open("reduced_sequences.fasta")
o = open(sys.argv[2], "w")

lines = s.readlines()
sequences = dict()
for line in range(len(lines)):
    if(lines[line][0] == '>'):
        curr = lines[line].split(' ')[0][1:]
        sequences[curr] = lines[line]
        line += 1
        while(line < len(lines) and lines[line][0] != '>'):
            sequences[curr] += lines[line]
            line += 1
        line -= 1
ret = ""
for elem in itext:
    ret += sequences[elem.replace('\n', '').replace('\t', '')]

o.write(ret)
i.close()
s.close()
o.close()
