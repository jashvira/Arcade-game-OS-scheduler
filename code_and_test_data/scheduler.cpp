#include <iostream>
#include <fstream>
#include <deque>
#include <queue>
#include <vector>
#include <algorithm>

// std is a namespace: https://www.cplusplus.com/doc/oldtutorial/namespaces/
const int TIME_ALLOWANCE = 8; // allow to use up to this number of time slots at once
const int PRINT_LOG = 0;      // print detailed execution trace
const int priority_threshold = 100; //an arbitrary value, not an optimal value

class Customer
{
public:
    std::string name;
    int priority;
    int arrival_time;
    int slots_remaining; // how many time slots are still needed
    int playing_since;
    int age;

    Customer(std::string par_name, int par_priority, int par_arrival_time, int par_slots_remaining)
    {
        name = par_name;
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_remaining = par_slots_remaining;
        playing_since = -1;
        age = 0;
    }
};

class Event
{
public:
    int event_time;
    int customer_id; // each event involes exactly one customer
    int priority;

    Event(int par_event_time, int par_customer_id, int par_priority)
    {
        priority = par_priority;
        event_time = par_event_time;
        customer_id = par_customer_id;
    }
};

void initialize_system(
    std::ifstream &in_file,
    std::deque<Event> &arrival_events,
    std::vector<Customer> &customers)
{
    std::string name;
    int priority, arrival_time, slots_requested;

    // read input file line by line
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    int customer_id = 0;
    while (in_file >> name >> priority >> arrival_time >> slots_requested)
    {
        Customer customer_from_file(name, priority, arrival_time, slots_requested);
        customers.push_back(customer_from_file);

        // new customer arrival event
        Event arrival_event(arrival_time, customer_id, priority);
        arrival_events.push_back(arrival_event);

        customer_id++;
    }
}

void print_state(
    std::ofstream &out_file,
    int current_time,
    int current_id,
    const std::deque<Event> &arrival_events)
// const std::deque<int> &customer_queue)
{
    out_file << current_time << " " << current_id << '\n';
    if (PRINT_LOG == 0)
    {
        return;
    }
    std::cout << current_time << ", " << current_id << '\n';
    for (int i = 0; i < arrival_events.size(); i++)
    {
        std::cout << "\t" << arrival_events[i].event_time << ", " << arrival_events[i].customer_id << ", ";
    }
    // std::cout << '\n';
    // for (int i = 0; i < customer_queue.size(); i++)
    // {
    //     std::cout << "\t" << customer_queue[i] << ", ";
    // }
    // std::cout << '\n';
}

// process command line arguments
// https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Provide input and output file names." << std::endl;
        return -1;
    }
    std::ifstream in_file(argv[1]);
    std::ofstream out_file(argv[2]);
    if ((!in_file) || (!out_file))
    {
        std::cerr << "Cannot open one of the files." << std::endl;
        return -1;
    }

    // deque: https://www.geeksforgeeks.org/deque-cpp-stl/
    // vector: https://www.geeksforgeeks.org/vector-in-cpp-stl/
    std::deque<Event> arrival_events; // new customer arrivals
    std::vector<Customer> customers;  // information about each customer

    // read information from file, initialize events queue
    initialize_system(in_file, arrival_events, customers);

    int current_id = -1;                                     // who is using the machine now, -1 means nobody
    int time_out = -1;                                       // time when current customer will be preempted
    std::deque<std::pair<int, int> > priority_queue; //priority customers enter here when they first arrive
    std::deque<std::pair<int, int> > priority_played_queue; //priority customers are shifted here once they have played for the first time
    std::deque<std::pair<int, int> > customer_queue; // regular customers enter here when they first arrive
    std::deque<std::pair<int, int> > customer_played_queue; //regular customers are shifted here once they have played for the first time
    std::priority_queue<std::pair<int, int> > long_wait_queue; //regular customers are shifted here if they wait too long for the first chance

    // step by step simulation of each time slot
    bool all_done = false;
    for (int current_time = 0; !all_done; current_time++)
    {
        
        //shift the regular customers to long_wait_queue if they are waiting for too long
        for (int i = 0; i <customer_queue.size(); i++)
        { 
            int age_customer_id = customer_queue[i].second; 
            customers[age_customer_id].age ++;
            
            if (customers[age_customer_id].age >= priority_threshold)
            {
                customer_queue.erase(customer_queue.begin() + i);
                long_wait_queue.push(std::make_pair(customers[age_customer_id].age, age_customer_id));
            } 
        }


        // welcome newly arrived customers
        while (!arrival_events.empty() && (current_time == arrival_events[0].event_time))
        {   
            if (arrival_events[0].priority == 0)
            {
                int temp_customer_id = arrival_events[0].customer_id;
                priority_queue.push_back(std::make_pair(customers[temp_customer_id].slots_remaining, temp_customer_id));
            }
            else
            {
                int temp_customer_id = arrival_events[0].customer_id;
                customer_queue.push_back(std::make_pair(customers[temp_customer_id].slots_remaining, temp_customer_id));
            }

            arrival_events.pop_front();
        }
        // check if we need to take a customer off the machine
        if (current_id >= 0)
        {
            if (current_time == time_out)
            {
                int last_run = current_time - customers[current_id].playing_since;
                customers[current_id].slots_remaining -= last_run;
                // customer is not done yet, waiting for the next chance to play
                if (customers[current_id].slots_remaining > 0)
                {
                    //if the customer is a priority customer, shift them to priority_played_queue since they have already played
                    if (customers[current_id].priority == 0)
                    {
                        priority_played_queue.push_back(std::make_pair(customers[current_id].slots_remaining, current_id));
                        std::sort(priority_played_queue.begin(), priority_played_queue.end());
                    }
                    //if the customer is a regular customer, shift them to regular_played_queue since they have already played
                    else
                    {
                        customer_played_queue.push_back(std::make_pair(customers[current_id].slots_remaining, current_id));
                        std::sort(customer_played_queue.begin(), customer_played_queue.end());
                    }
                }
                current_id = -1; // the machine is free now
            }
        }
        // if machine is empty, schedule a new customer
        if (current_id == -1)
        {
            //first check if any regular customer is waiting for too long
            if (!long_wait_queue.empty()) // is anyone waiting?
            {
                current_id = long_wait_queue.top().second;
                long_wait_queue.pop();
                if (TIME_ALLOWANCE > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE;
                }
                customers[current_id].playing_since = current_time;
            }
            // then check if any high priority customer is yet to play
            else if (!priority_queue.empty()) // is anyone waiting?
            {
                current_id = priority_queue.front().second;
                priority_queue.pop_front();
                if (TIME_ALLOWANCE > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE;
                }
                customers[current_id].playing_since = current_time;
            }
            // then check if any high priority customer, who has already player but not completed his overall slotted time, is remaining
            else if (!priority_played_queue.empty())
            {
                current_id = priority_played_queue.front().second;
                priority_played_queue.pop_front();
                if (TIME_ALLOWANCE > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE;
                }
                customers[current_id].playing_since = current_time;
            }
            // then check if any regular customer is yet to play
            else if (!customer_queue.empty()) // is anyone waiting?
            {
                current_id = customer_queue.front().second;
                customer_queue.pop_front();
                if (TIME_ALLOWANCE > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE;
                }
                customers[current_id].playing_since = current_time;
            }
            // then check if any regular customer, who has already player but not completed his overall slotted time, is remaining
            else if (!customer_played_queue.empty()) // is anyone waiting?
            {
                current_id = customer_played_queue.front().second;
                customer_played_queue.pop_front();
                if (TIME_ALLOWANCE > customers[current_id].slots_remaining)
                {
                    time_out = current_time + customers[current_id].slots_remaining;
                }
                else
                {
                    time_out = current_time + TIME_ALLOWANCE;
                }
                customers[current_id].playing_since = current_time;
            }
        }
        print_state(out_file, current_time, current_id, arrival_events);

        // exit loop when there are no new arrivals, no waiting and no playing customers
        all_done = (arrival_events.empty() && long_wait_queue.empty() && customer_queue.empty() && customer_played_queue.empty() && priority_queue.empty() && priority_played_queue.empty() && (current_id == -1));
    }

    return 0;
}
