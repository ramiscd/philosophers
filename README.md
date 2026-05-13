*This project has been created as part of the 42 curriculum by rdamasce.*

---

# philosophers

## Description

The **Dining Philosophers Problem** is a classic synchronization challenge in computer science, originally formulated by Edsger Dijkstra in 1965. This project implements a simulation of N philosophers sitting at a round table, each alternating between thinking, eating, and sleeping.

The goal is to prevent two critical concurrent programming problems:

- **Deadlock** — a circular wait where every philosopher holds one fork and waits for the other indefinitely, freezing the simulation forever.
- **Data Race** — multiple threads reading and writing shared data simultaneously without synchronization, producing undefined behavior.

Each philosopher is an independent POSIX thread. The forks on the table are shared resources protected by mutexes. A dedicated monitor thread watches for starvation, stopping the simulation if any philosopher exceeds `time_to_die` without eating, or if all philosophers have reached the optional meal limit.

Key design decisions:
- **Lock ordering by memory address** in `philo_eat()` breaks the deadlock cycle by ensuring every pair of forks is always acquired in the same global order.
- **Per-philosopher `meal_lock`** protects `last_meal` and `meals_eaten` against data races between the philosopher thread (writer) and the monitor thread (reader).
- **Even-ID philosophers delay 1 ms** at startup to stagger fork contention and avoid thundering herd at time zero.

## Instructions

**Requirements:** `gcc`, `make`, and a POSIX-compliant system (Linux / macOS).

```bash
# Clone and enter the repository
git clone <repo-url> philosophers
cd philosophers

# Compile
make

# Run
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [meals_limit]
```

All time values are in **milliseconds**.

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` | Time in ms a philosopher can go without eating before dying |
| `time_to_eat` | Time in ms a philosopher spends eating |
| `time_to_sleep` | Time in ms a philosopher spends sleeping |
| `meals_limit` | *(Optional)* Simulation stops when every philosopher has eaten this many times |

**Examples:**

```bash
# 5 philosophers — should run indefinitely without anyone dying
./philo 5 800 200 200

# Stops after each philosopher eats 7 times
./philo 5 800 200 200 7

# 1 philosopher — must die after time_to_die ms
./philo 1 800 200 200
```

**Cleanup:**

```bash
make clean    # remove object files
make fclean   # remove object files and binary
make re       # full recompile
```

**Debug with Valgrind / Helgrind:**

```bash
# Check for data races
valgrind --tool=helgrind ./philo 4 410 200 200

# Check for memory leaks
valgrind --leak-check=full ./philo 5 800 200 200 7
```

## Resources

### References

- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [`pthread_mutex_lock` man page — Linux man-pages](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [`gettimeofday` man page — Linux man-pages](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [Helgrind: a thread error detector — Valgrind documentation](https://valgrind.org/docs/manual/hg-manual.html)
- *Operating System Concepts* — Silberschatz, Galvin & Gagne (Chapter 6: Synchronization)

### AI Usage

Claude (Anthropic) was used during this project for the following tasks:

- **Resolving the Helgrind-detected lock ordering issue** — the assistant identified that acquiring forks in a consistent global order (by memory address) breaks the circular-wait condition and eliminates the deadlock warning.
- **Debugging the monitor shutdown sequence** — helped reason through why `pthread_join` on the monitor must precede joining philosopher threads to guarantee a clean exit.
- **Generating `GUIDE.md`** — a full in-depth technical guide (in Brazilian Portuguese) covering thread/mutex concepts, struct architecture diagrams, data flow maps, and function connectivity, written to serve as a personal study reference for the project.

No AI was used to write core simulation logic (`routine`, `philo_eat`, `monitor_routine`) or the synchronization strategy — those were designed and implemented manually.
