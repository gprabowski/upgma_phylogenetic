#!/usr/bin/python
f = open("tree_clustalo.newick")
t = "".join(f.readlines()).replace("\n", "")
f.close()
o = open("tree_clustalo.newick", "w")
o.write(t)
o.close()
