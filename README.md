# Switch Scheduling & Packet-Level Simulation (C++)

## 📌 Overview
This project explores the behavior of high-performance network switches and packet-level processing through a set of C++ simulations and utilities.

It focuses on:
- Modeling switching algorithms  
- Analyzing throughput and latency  
- Implementing low-level IP packet handling  

---

## 🚀 Key Components

### 1. iSLIP Scheduling Algorithm
- Implementation of the iSLIP algorithm for input-queued switches  
- Models request–grant–accept cycles with rotating priorities  
- Simulates matching between inputs and outputs under contention  
- Demonstrates mitigation of Head-of-Line (HOL) blocking using VOQ  

---

### 2. Throughput & Switching Simulation
- Simulation of NxN input-queued switches under randomized traffic  
- Evaluates system throughput across different switch sizes  
- Analyzes performance limitations such as HOL blocking  
- Explores scalability and efficiency trade-offs  

---

### 3. IP Packet Builder & Parser
- Low-level implementation of IPv4 packet construction and parsing  
- Handles:
  - Header construction (version, IHL, TTL, protocol)  
  - Big-endian encoding/decoding  
  - Header checksum calculation  
- Supports file-to-packet conversion and reconstruction  

---

## 🧩 Key Concepts Demonstrated

- Switch scheduling (iSLIP, VOQ, HOL blocking)  
- Packet-level processing and protocol structure  
- Performance analysis (throughput, latency)  
- Systems programming in C++  
- Binary data handling and endianness  
- Algorithmic modeling of network behavior  

---

## 🛠 Technologies

- C++ (STL)  
- Linux environment  
- Standard libraries for simulation and file handling  

---

## 🎯 Purpose

This project was developed as part of academic work in **high-speed networks and systems programming**, with the goal of understanding how modern network switches operate and how packet processing is implemented at a low level.

---

## 📎 Structure

```plaintext
islip/          # iSLIP scheduling algorithm implementation
simulations/    # throughput and switching behavior simulations
ip_packet/      # IPv4 packet construction and parsing
```
