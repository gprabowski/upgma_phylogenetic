#!/usr/bin/sh

./upgma 18 \; intuit_edit_distance.txt intuit.newick intuition_names.txt

./upgma 9 \; max_first_edit_distance.txt max_first.newick representants_max_first.txt
./upgma 9 \; max_middle_edit_distance.txt max_middle.newick representants_max_middle.txt
./upgma 9 \; max_last_edit_distance.txt max_last.newick representants_max_last.txt

./upgma 11 \; avg_first_edit_distance.txt avg_first.newick representants_avg_first.txt
./upgma 11 \; avg_middle_edit_distance.txt avg_middle.newick representants_avg_middle.txt
./upgma 11 \; avg_last_edit_distance.txt avg_last.newick representants_avg_last.txt
