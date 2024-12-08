
# Arcade Game OS Scheduler

## Project Description

This project implements an arcade game operating system scheduler. The scheduler allocates machine time to customers based on priority, arrival time, and requested playtime. It utilizes a combination of priority scheduling, aging, and shortest job first (SJF) techniques to minimize wait time and ensure fair access to the arcade machine.

## Usage

### Running the Scheduler

For example, suppose the compiled program is named `scheduler`, and the input file is `data_1111.txt`. Running the program as:

```bash
./scheduler data_1111.txt out_1111.txt
```

should produce an output file `out_1111.txt` containing a valid scheduling based on the input data.

### Validation and Statistics

`compute_stats`, can validate the scheduling and compute metrics such as total wait time. Use it as follows:

```bash
./compute_stats data_1111.txt out_1111.txt
```

### Data Formats

#### Input File

Input data files describe a simulated list of arriving customers, with each row in the following format:

```
<customer_id> <priority> <arrival_time> <play_time>
```

- `customer_id`: Customer identifier (e.g., `c00`).
- `priority`: `0` for high priority, `1` for regular priority.
- `arrival_time`: Time when the customer arrives.
- `play_time`: Desired playtime.

Example:

```
c00 0 6 23
c01 1 20 13
c02 1 28 17
```

#### Output File

Output files describe the machine's usage over time slots, starting from `0`. Each row indicates the occupying customer or `-1` if the machine is unoccupied:

```
<time_slot> <customer_id or -1>
```

Example:

```
0 -1
1 -1
2 -1
3 -1
4 -1
5 -1
6 0
7 0
```

### Algorithm Details

The scheduler uses a combination of priority scheduling, aging, and SJF. The main features include:
- **Time Allowance:** Round-robin with a time quantum of 8 units.
- **Aging Mechanism:** Prevents starvation by prioritizing regular customers who wait beyond a threshold (100 units).
- **Multi-Queue System:** Utilizes separate queues for new arrivals, played customers, and long-wait customers.

The sequence of scheduling is as follows:
1. Regular customers waiting beyond the threshold.
2. High-priority customers who have not yet played.
3. High-priority customers with remaining playtime.
4. New regular customers who have not yet played.
5. Regular customers with remaining playtime.
