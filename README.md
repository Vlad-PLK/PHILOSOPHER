# Dining Philosophers Problem 🍝

**A multithreaded solution to the classic Dining Philosophers synchronization problem in C.**

[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![POSIX Threads](https://img.shields.io/badge/pthreads-supported-green.svg)](https://en.wikipedia.org/wiki/POSIX_Threads)
[![Mutex](https://img.shields.io/badge/synchronization-mutex-blue.svg)](https://en.wikipedia.org/wiki/Mutual_exclusion)
[![42 School](https://img.shields.io/badge/42-School-000000.svg)](https://42.fr/)

## 🌟 Features

- ✅ **Multithreading**: Each philosopher runs in a separate thread
- ✅ **Mutex Synchronization**: Deadlock-free fork management
- ✅ **Race Condition Prevention**: Protected shared resources
- ✅ **Precise Timing**: Microsecond-level time management
- ✅ **Death Detection**: Real-time monitoring of philosopher states
- ✅ **Configurable**: Support for 1-200 philosophers
- ✅ **Optional Meal Limit**: Simulation ends when all philosophers eat enough
- ✅ **Data Race Free**: Thread-safe operations with mutexes

## 📋 Table of Contents

- [The Problem](#-the-problem)
- [The Solution](#-the-solution)
- [Quick Start](#-quick-start)
- [Usage](#-usage)
- [Implementation Details](#-implementation-details)
- [Project Structure](#-project-structure)
- [Examples](#-examples)
- [Technical Challenges](#-technical-challenges)

## 🤔 The Problem

The **Dining Philosophers Problem** is a classic synchronization and concurrency problem in computer science, originally formulated by Edsger Dijkstra in 1965.

### The Scenario

```
         🍝
         
    🍴       🍴
  👨 —————— 👨
   |         |
   |    🍝   |
   |         |
  👨 —————— 👨
    🍴       🍴
    
      🍝
```

- **N philosophers** sit around a circular table
- Each philosopher alternates between **thinking**, **eating**, and **sleeping**
- There is a **fork** between each pair of adjacent philosophers
- To eat, a philosopher needs **two forks** (left and right)
- Philosophers don't speak to each other (no communication)
- Philosophers don't know when others are about to die

### The Challenge

**Avoid deadlock**: If all philosophers pick up their left fork simultaneously, none can pick up their right fork, causing deadlock.

**Avoid starvation**: Ensure each philosopher gets a chance to eat before dying.

**Prevent data races**: Multiple threads accessing shared resources (forks) must be synchronized.

## 💡 The Solution

This implementation uses:

1. **One thread per philosopher**: Each philosopher runs independently
2. **One mutex per fork**: Protects fork state (available/taken)
3. **Additional mutexes**: For death detection, meal counting, and printing
4. **Even/odd strategy**: Odd-numbered philosophers wait slightly before starting to prevent deadlock
5. **Precise timing**: `gettimeofday()` for microsecond-accurate time tracking

### Key Strategies

- **Mutex locks** prevent simultaneous fork access
- **Staggered start** (even vs odd philosophers) reduces contention
- **Time-based death detection** monitors elapsed time since last meal
- **Atomic meal counting** tracks when all philosophers have eaten enough

## 🚀 Quick Start

### Prerequisites

- **GCC** or compatible C compiler
- **POSIX-compliant system** (Linux, macOS, Unix)
- **pthread library** (usually included)
- **Make** build tool

### Compilation

```bash
# Clone the repository
git clone <repository-url>
cd PHILOSOPHER

# Compile the program
make

# The executable 'philo' will be created
```

### Running

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]
```

**Arguments:**
- `number_of_philosophers`: Number of philosophers and forks (1-200)
- `time_to_die`: Time in milliseconds before a philosopher dies without eating
- `time_to_eat`: Time in milliseconds it takes to eat
- `time_to_sleep`: Time in milliseconds spent sleeping
- `number_of_meals`: (Optional) Simulation stops when all philosophers eat this many times

## 💻 Usage

### Basic Examples

```bash
# 5 philosophers, die after 800ms, eat for 200ms, sleep for 200ms
./philo 5 800 200 200

# Same as above, but stop after each philosopher eats 7 times
./philo 5 800 200 200 7

# Stress test with 200 philosophers
./philo 200 800 200 200

# One philosopher (impossible scenario - only one fork)
./philo 1 800 200 200
```

### Understanding the Output

```bash
$ ./philo 4 410 200 200
0 ms 1 is thinking
0 ms 3 is thinking
0 ms 1 has taken a fork
0 ms 1 has taken a fork
0 ms 1 is eating
5 ms 3 has taken a fork
5 ms 3 has taken a fork
5 ms 3 is eating
200 ms 1 is sleeping
205 ms 3 is sleeping
400 ms 1 is thinking
400 ms 2 is thinking
405 ms 3 is thinking
405 ms 3 has taken a fork
...
```

**Output Format**: `<timestamp_ms> <philosopher_id> <action>`

**Actions:**
- `is thinking` - Philosopher is thinking
- `has taken a fork` - Philosopher picked up a fork
- `is eating` - Philosopher is eating (has both forks)
- `is sleeping` - Philosopher is sleeping (released forks)
- `died` - Philosopher died from starvation

### Expected Behavior

**No philosopher should die** (unless impossible):
```bash
# These should run indefinitely without death
./philo 5 800 200 200
./philo 4 410 200 200
```

**Philosopher dies** (impossible scenario):
```bash
# Only one fork available - philosopher must die
./philo 1 800 200 200
# Output: 800 ms 1 died
```

**Meal limit reached** (simulation ends):
```bash
# Each philosopher eats 5 times, then simulation stops
./philo 5 800 200 200 5
```

## 🏗️ Implementation Details

### Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Main Program                             │
├─────────────────────────────────────────────────────────────┤
│  1. Parse arguments and validate input                      │
│  2. Initialize shared data structure                        │
│  3. Create mutexes (forks + control mutexes)               │
│  4. Spawn philosopher threads                               │
│  5. Wait for threads to complete (join)                     │
│  6. Clean up mutexes and resources                          │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│              Philosopher Thread (per philosopher)           │
├─────────────────────────────────────────────────────────────┤
│  Loop until death or meal limit:                            │
│    1. THINK (wait state)                                    │
│    2. TRY TO EAT                                            │
│       ├─ Lock left fork mutex                               │
│       ├─ Lock right fork mutex                              │
│       ├─ Update last meal time                              │
│       ├─ Sleep for time_to_eat                              │
│       ├─ Unlock forks                                       │
│       └─ Increment meal count                               │
│    3. SLEEP                                                 │
│       └─ Sleep for time_to_sleep                            │
│    4. Check if time_to_die exceeded                         │
│       └─ Mark as dead if starved                            │
└─────────────────────────────────────────────────────────────┘
```

### Data Structures

#### Main Structure (`t_philo`)
```c
typedef struct s_philo {
    pthread_mutex_t   mutex_tab[201];      // Fork mutexes (one per fork)
    pthread_mutex_t   dead_mutex;          // Protects death state
    pthread_mutex_t   meal_mutex;          // Protects meal counting
    pthread_mutex_t   print_mutex;         // Prevents mixed output
    t_philo_perso     tab[201];            // Array of philosopher data
    t_phil_args       data;                // Program arguments
    int               is_dead;             // Global death flag
    int               all_meals;           // All reached meal limit
    int              *forks_tab;           // Fork availability array
} t_philo;
```

#### Individual Philosopher (`t_philo_perso`)
```c
typedef struct s_philo_perso {
    pthread_t       thread_index;       // Thread ID
    int             is_thinking;        // Current thinking state
    int             meals;              // Meals eaten so far
    int             index;              // Philosopher ID (0-based)
    long long int   starting_time;      // Time of last meal
    long long int   real_time;          // Simulation start time
    struct s_philo *main_phi;           // Pointer to shared data
} t_philo_perso;
```

### Thread Synchronization

#### Fork Management
```c
// Taking forks (eating)
pthread_mutex_lock(&mutex_tab[left_fork]);
pthread_mutex_lock(&mutex_tab[right_fork]);
// ... eat ...
pthread_mutex_unlock(&mutex_tab[left_fork]);
pthread_mutex_unlock(&mutex_tab[right_fork]);
```

#### Deadlock Prevention
- **Odd philosophers** start with a small delay (5ms)
- **Even philosophers** start immediately
- This staggers fork acquisition and prevents circular wait

#### Death Detection
- Each philosopher tracks `starting_time` (last meal time)
- Main loop checks: `current_time - starting_time > time_to_die`
- Uses `dead_mutex` to safely update global death flag

### Time Management

```c
long long int ft_current_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int ft_usleep(unsigned int time, t_philo_perso *philo) {
    long long int start = ft_current_time();
    while (ft_current_time() - start < time) {
        usleep(100);  // Sleep in small increments
        // Check for death/completion between sleeps
    }
}
```

## 📁 Project Structure

```
PHILOSOPHER/
├── philo.h                    # Header file with structures and prototypes
├── main.c                     # Entry point and argument validation
├── philo.c                    # Main logic: thread creation, simulation loop
├── ft_thread_functions.c      # Thread utilities: join, mutex init/destroy, printf
├── ft_actions.c               # Philosopher actions: eat, sleep, think
├── ft_utils.c                 # Utility functions: atoi, time, usleep
├── Makefile                   # Build configuration
└── README.md                  # This file
```

### File Descriptions

| File | Purpose |
|------|---------|
| `philo.h` | Data structures, function prototypes, includes |
| `main.c` | Argument parsing, input validation |
| `philo.c` | Thread creation, philosopher lifecycle, death detection |
| `ft_thread_functions.c` | Mutex management, thread joining, synchronized printing |
| `ft_actions.c` | Eating, sleeping, thinking logic with fork acquisition |
| `ft_utils.c` | Time functions, string conversion, custom usleep |
| `Makefile` | Compilation rules and cleanup targets |

## 🔧 Technical Challenges

### 1. Race Conditions

**Problem**: Multiple threads accessing shared data simultaneously.

**Solution**:
- One mutex per shared resource (fork)
- Additional mutexes for death flag, meal count, and output
- All shared data access wrapped in mutex locks

### 2. Deadlock Prevention

**Problem**: All philosophers grab left fork, wait forever for right fork.

**Solution**:
- Staggered start: odd philosophers wait 5ms before beginning
- Reduces simultaneous fork grabbing
- Ensures at least some philosophers can eat initially

### 3. Precise Timing

**Problem**: Standard `sleep()` is too coarse (seconds), `usleep()` can overshoot.

**Solution**:
- Custom `ft_usleep()` using `gettimeofday()` and short `usleep(100)` loops
- Checks death condition between sleep intervals
- Microsecond-level precision for timing

### 4. Data Races

**Problem**: Reading/writing shared variables without synchronization.

**Solution**:
- `meal_mutex`: Protects meal counting
- `dead_mutex`: Protects death flag
- `print_mutex`: Prevents interleaved console output

### 5. Death Detection Accuracy

**Problem**: Philosopher might die while another is eating.

**Solution**:
- Continuous time checking: `current_time - starting_time <= time_to_die`
- Death check in every action (eat, sleep, think)
- Exit immediately when death detected

### 6. Meal Limit Synchronization

**Problem**: Determining when all philosophers have eaten enough meals.

**Solution**:
- Atomic meal counter per philosopher
- Check all counters with `meal_mutex` locked
- Set global `all_meals` flag when complete

## 🧪 Testing

### Test Cases

```bash
# Standard tests (no one should die)
./philo 5 800 200 200           # Basic 5 philosophers
./philo 4 410 200 200           # Tight timing
./philo 4 310 200 100           # Very tight timing

# Edge cases
./philo 1 800 200 200           # One philosopher (should die)
./philo 2 800 200 200           # Two philosophers (minimum viable)
./philo 200 800 200 200         # Maximum philosophers

# Meal limits
./philo 5 800 200 200 7         # Stop after 7 meals each
./philo 4 410 200 200 10        # Tight timing with meal limit

# Impossible scenarios (should die)
./philo 1 400 200 200           # Not enough time to eat
./philo 5 200 300 100           # Time to die < time to eat
```

### Expected Results

✅ **No death**: Philosophers alternate indefinitely  
✅ **Clean output**: No mixed/garbled lines  
✅ **Instant death detection**: Death reported within 10ms  
✅ **Meal limit**: Simulation stops when all eat enough  
✅ **No leaks**: All mutexes destroyed, memory freed  

### Debugging

```bash
# Check for data races with Thread Sanitizer
gcc -fsanitize=thread -g philo.c ... -o philo
./philo 5 800 200 200

# Check for memory leaks with Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200

# Monitor thread behavior
# Add debug prints (with print_mutex) to trace execution
```

## 💡 Key Learnings

- **Multithreading**: Creating, managing, and synchronizing POSIX threads
- **Mutexes**: Preventing race conditions and protecting shared resources
- **Deadlock Prevention**: Strategies like resource ordering and staggered timing
- **Concurrency**: Understanding parallel execution and thread communication
- **Precise Timing**: Working with microsecond-level time precision
- **Resource Management**: Proper cleanup of threads and mutexes
- **Debugging**: Identifying and fixing race conditions and deadlocks

## 🎓 42 School Project

This project is part of the 42 School curriculum, focusing on:

### Learning Objectives

- **Thread Programming**: Creating and managing POSIX threads
- **Synchronization Primitives**: Mutexes, atomic operations
- **Concurrency Problems**: Understanding classic CS problems
- **Race Condition Prevention**: Thread-safe programming
- **Resource Management**: Proper initialization and cleanup
- **Debugging Multithreaded Programs**: Tools and techniques

### Project Constraints

✅ One thread per philosopher  
✅ One fork between each philosopher pair  
✅ Mutexes to protect forks  
✅ No global variables (data in shared structure)  
✅ No data races  
✅ No philosopher should die (when possible)  
✅ Death reported within 10ms of occurrence  

## 📚 Resources & References

- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Mutex Tutorial](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)
- [Thread Synchronization](https://www.cs.cmu.edu/~410-s05/lectures/L31_Synchronization.pdf)
- [Deadlock Prevention](https://en.wikipedia.org/wiki/Deadlock#Prevention)

## 🤝 Credits

**Author**: Vlad Polojie (vpolojie)  
**School**: 42 School  
**Project**: Philosophers  
**Year**: 2023

## 📝 License

This project is part of the 42 School curriculum and follows the school's academic policies.

---

**Quote**: *"Philosophy is the art of asking questions. Multithreading is the art of answering them simultaneously."* 🧵