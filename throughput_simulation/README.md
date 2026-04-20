# Throughput Simulation for an Input-Queued Switch (C++)

## Overview
This module simulates the throughput of an `N x N` input-queued switch over a large number of time slots.

The implementation models **Head-of-Line (HOL) blocking** by keeping each input's HOL packet at the front of the queue until it is actually served.

---

## Simulation Model

- The switch has `N` inputs and `N` outputs.
- Each input always has a HOL packet ready.
- Each HOL packet is destined uniformly at random to one of the `N` outputs.
- In each time slot:
  - every input requests its HOL destination
  - each output can serve at most one requesting input
- If an input is served:
  - its HOL packet departs
  - a new packet immediately replaces it
- If an input is not served:
  - its HOL packet remains at the head of the queue

This persistence is what creates HOL blocking and distinguishes the model from a memoryless random-request model.

---

## Throughput Calculation

Throughput is computed as:

```text
total_served / total_arrived
```

Since every input always has a packet ready in every time slot:

```text
total_arrived = N * ts
```

---

## Build & Run

### Compile
```bash
g++ -std=c++17 -O2 throughput_simulation.cpp -o throughput_simulation
```

### Run
```bash
./throughput_simulation
```

---

## Output

The program prints two tab-separated columns:

- `N`
- estimated throughput

This format is convenient for copying into Excel and plotting the throughput as a function of `N`.

---

## Key Concepts Demonstrated

- Input-queued switch behavior
- Head-of-Line (HOL) blocking
- Randomized arbitration
- Throughput estimation by simulation
- Performance analysis of switching systems

## Expected Behavior

For large values of `N`, the simulated throughput approaches the classic HOL-blocking limit of approximately **0.586** for input-queued switches under uniform random traffic.
