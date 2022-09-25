# Arcade-game-OS-scheduler

For example, suppose that your program is compiled into ./scheduler, and you also have ./data_1111.txt in the current directory. Then
  ./scheduler data_1111.txt out_1111.txt
should produce out_1111.txt in this directory, and this file should describe a valid scheduling with respect to input in data_1111.txt. For example, demand of each customer in data_1111.txt must be satisfied in out_1111.txt.

Use the provided "compute_stats.cpp" to compute properties of your scheduling, such as total wait time
This program also checks whether the scheduling is valid
This program accepts two command line arguments: input data file and the output (scheduling) file generated for this input

Data Format:
An input data file is a simulated list of arriving customers. Input data files will be named "data_*.txt", where asterisk will be replaced by a random number (not relevant for you). Each row in such a file describes customer ID, priority (0 for high, 1 for regular), arrival time, and the desired amount of play time. 

Example:

c00 0 6 23
c01 1 20 13
c02 1 28 17
c03 1 38 28
...
In the first line, customer c00 arrived at time 6. This customer is a member and has a high priority. This customer wishes to play for 23 units of time in total. Note that 0 denotes high priority, while 1 denotes regular priority.

An output data file is a log of machine usage as a result of a particular scheduling approach. For a given input file, an output file can be generated using the program in "scheduler.cpp". Each such output file is also called a scheduling.

Each row of the output file describes who was occupying the machine at a particular time slot (starting from 0). If the machine was not occupied, the row will list -1, otherwise it will show customer ID (without letter "c").

Example:

0 -1
1 -1
2 -1
3 -1
4 -1
5 -1
6 0
7 0
...
In this example, the machine was unoccupied in time slots 0, ..., 5. Then starting from time slot 6, the machine was used by customer 0.
