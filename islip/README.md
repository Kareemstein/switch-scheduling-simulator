# iSLIP Scheduling Simulation (C++)

##  Overview
This module implements a simulation of the **iSLIP scheduling algorithm**, a widely used arbitration mechanism for **input-queued (IQ) network switches** with **Virtual Output Queues (VOQ)**.

The implementation models a **single scheduling cycle**, including the core **grant–accept mechanism** and **round-robin pointer updates**, which are essential for achieving fairness and high throughput in high-speed switches.

---

##  Background

In input-queued switches, multiple inputs may request the same output simultaneously, leading to contention and performance degradation (e.g., Head-of-Line blocking).

The **iSLIP algorithm** addresses this by:
- Using **iterative request–grant–accept phases**
- Applying **round-robin arbitration**
- Updating pointers **only on successful matches**

This allows the switch to:
- Improve fairness between flows  
- Reduce starvation  
- Increase throughput under high load  

---

## ⚙️ Implementation Details

###  Grant Phase
Each **unmatched output** selects one requesting input  
based on its **round-robin pointer**

###  Accept Phase
Each **unmatched input** accepts one of the received grants  
based on its **round-robin pointer**

###  Pointer Updates
Pointers are updated **only for accepted matches**  
(this is the defining characteristic of iSLIP)

###  Iterative Matching
The algorithm runs for a bounded number of iterations (`max_iters`)  
and stops early if no further matches can be made

---

##  Input Representation

- `request_matrix[i][j] = 1`  
  → Input `i` has a packet destined for Output `j`

- Round-robin pointers (1-based):
  - `in_ptr[i]` → input `i` accept pointer  
  - `out_ptr[j]` → output `j` grant pointer  

---

##  Output

The function returns:

- `match_matrix`  
  → Binary NxN matrix indicating successful matches  

- Updated:
  - `in_ptr`  
  - `out_ptr`  

---

##  Build & Run

### Compile
```bash
g++ -std=c++17 -O2 islip.cpp demo_islip.cpp -o islip_demo
