#include <iostream>
#include <vector>
#include <random>
#include <cstdint>

/*
 * Throughput Simulation for an NxN Input-Queued Switch
 *
 * This simulation models an input-queued switch where:
 * - Each input always has a head-of-line (HOL) packet ready.
 * - Each HOL packet has a uniformly random destination output.
 * - In every time slot, each output can serve at most one input.
 * - If an input is served, its HOL packet departs and is immediately replaced
 *   by a new packet with a new random destination.
 * - If an input is NOT served, its HOL packet remains at the head of the queue
 *   into the next time slot.
 *
 * This persistence of blocked HOL packets is essential for modeling
 * Head-of-Line (HOL) blocking correctly.
 *
 * Throughput is calculated as:
 *
 *   total_served / total_arrived
 *
 * Since every input always has a packet ready, total_arrived = N * ts.
 */

/*
 * Simulate an NxN input-queued switch for ts time slots.
 *
 * Parameters:
 *   N  - number of input ports and output ports
 *   ts - number of simulated time slots
 *
 * Returns:
 *   estimated throughput
 */
double simulate_throughput(int N, std::int64_t ts) {
    std::mt19937 rng(123456); // fixed seed for repeatability
    std::uniform_int_distribution<int> output_dist(0, N - 1);

    // hol_destination[i] = current HOL packet destination for input i
    std::vector<int> hol_destination(N);
    for (int i = 0; i < N; ++i) {
        hol_destination[i] = output_dist(rng);
    }

    std::int64_t total_served = 0;

    // request_lists[o] = list of inputs requesting output o in this slot
    std::vector<std::vector<int>> request_lists(N);

    for (std::int64_t t = 0; t < ts; ++t) {
        // Clear request lists for the current slot
        for (int o = 0; o < N; ++o) {
            request_lists[o].clear();
        }

        // Build requests from current HOL packets
        for (int i = 0; i < N; ++i) {
            request_lists[hol_destination[i]].push_back(i);
        }

        // Track which inputs were served this slot
        std::vector<bool> input_served(N, false);

        // Each output serves at most one requesting input
        for (int o = 0; o < N; ++o) {
            if (!request_lists[o].empty()) {
                std::uniform_int_distribution<int> pick_input(
                    0, static_cast<int>(request_lists[o].size()) - 1
                );

                int chosen_input = request_lists[o][pick_input(rng)];
                input_served[chosen_input] = true;
                total_served++;
            }
        }

        // Only served inputs get a new HOL packet.
        // Unserved inputs keep the same HOL packet -> HOL blocking persists.
        for (int i = 0; i < N; ++i) {
            if (input_served[i]) {
                hol_destination[i] = output_dist(rng);
            }
        }
    }

    const double total_arrived = static_cast<double>(N) * static_cast<double>(ts);
    return static_cast<double>(total_served) / total_arrived;
}

int main() {
    const std::int64_t ts = 1000000;

    // Print N and throughput, tab-separated for easy copy/paste into Excel
    for (int N = 2; N <= 50; ++N) {
        std::cout << N << '\t' << simulate_throughput(N, ts) << '\n';
    }

    return 0;
}
