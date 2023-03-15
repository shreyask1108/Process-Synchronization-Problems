# Process-Synchronization-Problems

## Producer Consumer Problem

### Problem Statement

The producer-consumer synchronization problem is a classical synchronization problem in computer science, particularly in operating system design and parallel computing.<br>

In this problem, there are two types of processes: producers and consumers. Producers generate data or items and add them to a shared buffer or queue, while consumers consume or remove data from the buffer. The issue arises when multiple producers and consumers share the same buffer, and there is a risk of data race conditions, deadlocks, or data inconsistency.<br>

The primary goal of synchronization is to ensure that the producers and consumers access the shared buffer in a mutually exclusive and orderly fashion to prevent race conditions or data corruption. It requires implementing mutual exclusion mechanisms such as locks, semaphores, or monitors to ensure that only one process at a time can access the shared resource. There are various synchronization techniques that can be employed to solve this problem, such as using semaphores, mutexes, or conditional variables.<br>

The three types of buffer involved in this problem:

•	Bounded Buffer

•	Infinite Buffer

•	Cyclic Buffer


### Starve free and Deadlock free Solution

We will be using the following parameters to solve the problem:

Variables

•	in = 0        //buffer index where item will be next produced

•	out = 0      //buffer index where item will be next consumed

Semaphores

•	full = 0     //Counting semaphore that keeps the count of complete buffers

•	empty = N  //Counting semaphore that keeps the count of empty buffers

•	mx = 0      //Binary lock to allow the mutual exclusion of shared variables

<br>

## Readers–Writers Problem

### Problem Statement

The problem involves multiple processes, some of which want to read a single shared resource, while others want to write to it. The problem can be challenging to solve because it requires a balance between allowing multiple readers to access the resource concurrently while ensuring that only one writer can access it at a time.

The simulation should satisfy the following conditions:
                                          
•	Any number of readers should be able to read simultaneously.

•	No two writers can execute simultaneously.

•	Simultaneous reading and writing cannot happen.

•	The system should not be idle at any time if some process is waiting.

•	The system should be deadlock free.

•	The system should be starve free.


### Starve free and Deadlock free Solution

We will be using the following parameters to solve the problem:

•	fifo_queue: A binary semaphore to make the solution starve free by maintaining a queue of processes to execute them in FIFO order.

•	shared_resource: A binary semaphore to achieve mutual exclusion among readers and writers, such that the given conditions are met.

•	readcount: Number of readers currently reading.

•	readcount_mutex: A mutex lock to avoid simultaneous access of readcount variable.





## Dining Philosophers Problem

### Problem Statement

The dining philosopher's problem is the classical problem of synchronization which says that Five philosophers are sitting around a circular table and their job is to think and eat alternatively. A bowl of noodles is placed at the center of the table along with five chopsticks for each of the philosophers. To eat a philosopher needs both a right and a left chopstick. A philosopher can only eat if both the immediate left and right chopsticks of the philosopher are available. In case both the immediate left and right chopsticks of the philosopher are not available then the philosopher puts down their (either left or right) chopstick and starts thinking again.


The simulation should satisfy the following conditions:

•	Deadlock-Free: One of the primary challenges in the Dining Philosophers problem is avoiding deadlock, where each philosopher is holding onto one fork and waiting for the other to become available.

•	Starvation-Free: Starvation is another challenge that can occur in the Dining Philosophers problem, where a philosopher is unable to eat because they are unable to acquire both forks. 

•	Concurrency: The Dining Philosophers problem requires coordination between multiple concurrent processes, which can be challenging to implement correctly.

•	Resource utilization: The Dining Philosophers problem involves a shared resource (the forks) that must be utilized efficiently to ensure that all philosophers have a fair chance to eat.

### Starve free and Deadlock free Solution

We will be using the following parameters to solve the problem:

•	N = 5  // number of philosophers

•	THINKING = 2

•	HUNGRY = 1

•	EATING = 0

•	state[N] // state[I] - represents current state of philosophers

•	int phil // identifies a philosopher thread

•	int a[N] // philosopher number

•	chopsticks[N] = array of binary semaphores (indicates if a chopstick is picked up or put down)





## Sleeping Barbers Problem

### Problem Statement

The Sleeping Barbers Problem with multiple barbers and multiple customers is a variation of the original problem. In this version of the problem, there are multiple barbers working in the barbershop, each with their own barber chair, and multiple customers waiting in the waiting chairs.

The simulation should satisfy the following conditions:

•	Customers are not turned away if there are empty chairs available.

•	Each barber is cutting hair if there are customers waiting and there is an empty barber chair available.

•	The number of customers waiting is limited to the number of available chairs in the waiting area.

•	Each barber serves one customer at a time and is not interrupted while cutting hair.

•	Customers leave the waiting area after being served by a barber.

### Starve free and Deadlock free Solution

We will be using the following parameters to solve the problem:

•	Barbers:- Counting semaphore that keeps the count of sleeping barbers initialized to the total number of barbers.

•	Customers:- Counting semaphore that keeps the count of current customers initialized to zero.

•	Mutex:- Binary lock to allow the mutual exclusion of shared variables

•	Customer_count:- keeps count of total customers inside the shop





## Cigarette Smokers Problem

### Problem Statement

The cigarette smokers problem is a classic synchronization problem in Operating Systems. The problem is often used to illustrate the issues related to concurrent execution of multiple processes/threads and the need for synchronization mechanisms.

•	The problem involves an agent and two smokers who share a table.

•	The agent randomly places two different ingredients on the table.

•	Each smoker has an infinite supply of one of the remaining ingredients.

•	The smoker with the missing ingredient must wait until the agent places the required ingredients on the table.

•	Once the ingredients are placed, the smoker picks them up, makes a cigarette, and smokes it.

•	The problem is to design a solution that ensures only one smoker can smoke at a time, and that the agent must wait until the previous smoker has finished before placing new ingredients on the table.the previous smoker has finished before placing new ingredients on the table.

The simulation should satisfy the following conditions:

•	Only one smoker can smoke at a time.

•	The dealer provides the necessary resources to the smoker who is not smoking at the time.

•	Each smoker can only smoke if they have both necessary resources.
         
 ### Starve free and Deadlock free Solution
 
We will be using the following parameters to solve the problem:

Use semaphores to represent the different ingredients and a mutex to represent the smoking process.

The agent randomly chooses two ingredients and signals the corresponding semaphores.

Each smoker waits for the appropriate semaphore to be signalled and acquires the mutex to ensure that only one smoker is smoking at a time.

The smoker makes a cigarette and signals the smoking semaphore to allow the next smoker to smoke.

The smoker signals the appropriate agent semaphore to allow the agent to place new ingredients on the table.

The agent waits on a mutex to ensure that it does not place new ingredients on the table until the previous smoker has finished smoking.



## Rivers Crossing Problem

### Problem Statement

There is a rowboat that is used by both Linux hackers and Microsoft employees (serfs) to cross a river. The ferry can hold exactly four people; it won’t leave the shore with more or fewer. To guarantee the safety of the passengers, it is not permissible to put one hacker in the boat with three serfs, or to put one serf with three hackers. Any other combination is safe.


### Starve free and Deadlock free Solution

We will be using the following parameters to solve the problem:

Global variables:

•	barrier = Semaphore (4)

•	mutex = Semaphore (1)

•	int hackers = 0

•	int serfs = 0

•	hackerQueue = Semaphore (0)

•	serfQueue = Semaphore (0)

Local variable used in Hackers() and Serfs() functions:

•	local bool isCaptain = False




## H2O Problem

### Problem Statement

Hydrogen and oxygen atoms keep arriving randomly to form water molecules. Whenever there are two free hydrogen atoms and one free oxygen atom, we pair them up to form a water molecule. Simulate this situation.

### Starve Free and Deadlock Free Solution

We will use the following parameters to solve this problem:
•	
•	H_queue: A binary semaphore to keep track of waiting Hydrogen atoms by maintaining a queue of them.

•	O_queue: A binary semaphore to keep track of waiting Oxygen atoms by maintaining a queue of them.

•	waiting_H: Number of waiting Hydrogen atoms.

•	waiting_O: Number of waiting Oxygen atoms.

•	WH_mutex: A mutex lock to avoid simultaneous access of waiting_H variable.

•	WO_mutex: A mutex lock to avoid simultaneous access of waiting_O variable.

