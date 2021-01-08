#!/usr/bin/sh
./opal_aligner -o 10 -e 1 -x 2 -f pam100.mat -a NW intuition_representants.fasta intuition_representants.fasta
mv avsa_results.txt intuit_results.txt
mv avsa_edit_dist.txt intuit_edit_distance.txt

./opal_aligner -o 10 -e 1 -x 2 -f pam100.mat -a NW max_first.fasta max_first.fasta
mv avsa_results.txt first_results.txt
mv avsa_edit_dist.txt max_first_edit_distance.txt

./opal_aligner -o 10 -e 1 -x 2 -f pam100.mat -a NW max_middle.fasta max_middle.fasta
mv avsa_results.txt max_middle_results.txt
mv avsa_edit_dist.txt max_middle_edit_distance.txt

./opal_aligner -o 10 -e 1 -x 2 -f pam100.mat -a NW max_last.fasta max_last.fasta
mv avsa_results.txt max_last_results.txt
mv avsa_edit_dist.txt max_last_edit_distance.txt

./opal_aligner -o 10 -e 1 -x 2 -f pam100.mat -a NW avg_first.fasta avg_first.fasta
mv avsa_results.txt avg_first_results.txt
mv avsa_edit_dist.txt avg_first_edit_distance.txt

./opal_aligner -o 10 -e 1 -x 2 -f pam100.mat -a NW avg_middle.fasta avg_middle.fasta
mv avsa_results.txt avg_middle_results.txt
mv avsa_edit_dist.txt avg_middle_edit_distance.txt

./opal_aligner -o 10 -e 1 -x 2 -f pam100.mat -a NW avg_last.fasta avg_last.fasta
mv avsa_results.txt avg_last_results.txt
mv avsa_edit_dist.txt avg_last_edit_distance.txt
