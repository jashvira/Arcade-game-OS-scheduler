# Arcade-game-OS-scheduler

For example, suppose that your program is compiled into ./scheduler, and you also have ./data_1111.txt in the current directory. Then
  ./scheduler data_1111.txt out_1111.txt
should produce out_1111.txt in this directory, and this file should describe a valid scheduling with respect to input in data_1111.txt. For example, demand of each customer in data_1111.txt must be satisfied in out_1111.txt.

Use the provided "compute_stats.cpp" to compute properties of your scheduling, such as total wait time
This program also checks whether the scheduling is valid
This program accepts two command line arguments: input data file and the output (scheduling) file generated for this input
