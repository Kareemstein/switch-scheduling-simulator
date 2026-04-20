#include "islip.h"

/*
 * iSLIP Scheduler Implementation
 *
 * This module implements a single scheduling cycle of the iSLIP algorithm
 * for input-queued (IQ) switches using Virtual Output Queues (VOQ).
 *
 * The algorithm performs iterative request-grant-accept matching with
 * round-robin arbitration to improve fairness and avoid starvation.
 *
 * This implementation models:
 * - Request phase (inputs request outputs)
 * - Grant phase (outputs select one requester)
 * - Accept phase (inputs accept one grant)
 * - Pointer updates only on accepted matches (core iSLIP behavior)
 */

/*
 * Helper for 1-based round-robin pointers.
 *
 * Example:
 *   N = 5
 *   1 -> 2
 *   2 -> 3
 *   5 -> 1
 */
static int wrap_next_1based(int x, int N) {
    return (x == N) ? 1 : (x + 1);
}

/*
 * Run one iSLIP scheduling cycle on an NxN input-queued switch.
 *
 * Parameters:
 *   N         - Number of input ports and output ports.
 *   B         - Request matrix:
 *               B[i][j] == 1 means input i has a packet waiting for output j.
 *   in_ptr    - Per-input round-robin accept pointers (1-based).
 *   out_ptr   - Per-output round-robin grant pointers (1-based).
 *   max_iters - Maximum number of iSLIP iterations inside the cycle.
 *
 * Returns:
 *   ISlipResult containing:
 *     - S:     matching matrix, where S[i][j] == 1 means input i matched output j
 *     - in_ptr updated input pointers
 *     - out_ptr updated output pointers
 *
 * Notes:
 *   - Inputs and outputs can be matched at most once per cycle.
 *   - Pointer updates happen only for accepted matches, which is the key iSLIP rule.
 */
ISlipResult islip_one_cycle(
    int N,
    const std::vector<std::vector<int>>& request_matrix,
    std::vector<int> in_ptr,
    std::vector<int> out_ptr,
    int max_iters)
{
    // S[i][j] = 1 iff input i is matched to output j in this cycle
    std::vector<std::vector<int>> match_matrix(N, std::vector<int>(N, 0));

    // Track which inputs/outputs are already matched
    std::vector<bool> in_matched(N, false);
    std::vector<bool> out_matched(N, false);

    for (int it = 0; it < max_iters; ++it) {
        // grants_to_input[i] stores the outputs that granted input i this iteration
        std::vector<std::vector<int>> grants_to_input(N);

        // --------------------
        // GRANT PHASE
        // --------------------
        // Each unmatched output grants one unmatched requesting input,
        // starting from its round-robin pointer.
        for (int j = 0; j < N; ++j) {
            if (out_matched[j]) {
                continue;
            }

            int start = out_ptr[j];   // 1-based pointer over inputs
            int chosen_i = -1;

            // Scan inputs in round-robin order
            for (int step = 0; step < N; ++step) {
                int i = (start - 1 + step) % N;  // convert to 0-based index

                if (in_matched[i]) {
                    continue;
                }

                if (request_matrix[i][j] == 1) {
                    chosen_i = i;
                    break;
                }
            }

            if (chosen_i != -1) {
                grants_to_input[chosen_i].push_back(j);
            }
        }

        // --------------------
        // ACCEPT PHASE
        // --------------------
        // Each unmatched input accepts one of the grants it received,
        // starting from its own round-robin pointer.
        bool any_accept = false;

        for (int i = 0; i < N; ++i) {
            if (in_matched[i] || grants_to_input[i].empty()) {
                continue;
            }

            int start = in_ptr[i];   // 1-based pointer over outputs
            int chosen_j = -1;

            for (int step = 0; step < N; ++step) {
                int j = (start - 1 + step) % N;  // convert to 0-based index

                if (out_matched[j]) {
                    continue;
                }

                bool granted = false;
                for (int g : grants_to_input[i]) {
                    if (g == j) {
                        granted = true;
                        break;
                    }
                }

                if (granted) {
                    chosen_j = j;
                    break;
                }
            }

            if (chosen_j != -1) {
                any_accept = true;

                match_matrix[i][chosen_j] = 1;
                in_matched[i] = true;
                out_matched[chosen_j] = true;

                // iSLIP pointer update:
                // advance only for accepted matches
                in_ptr[i] = wrap_next_1based(chosen_j + 1, N);
                out_ptr[chosen_j] = wrap_next_1based(i + 1, N);
            }
        }

        // If nothing was accepted, no more progress can be made this cycle
        if (!any_accept) {
            break;
        }
    }

    return ISlipResult{match_matrix, in_ptr, out_ptr};
}
