#pragma once
#include <vector>

/*
 * Interface for a single-cycle iSLIP scheduler simulation.
 *
 * Models request-grant-accept matching for an NxN input-queued switch
 * and returns the resulting match matrix together with updated
 * round-robin pointers.
 */
struct ISlipResult {
    std::vector<std::vector<int>> match_matrix; // NxN
    std::vector<int> in_ptr;                    // size N
    std::vector<int> out_ptr;                   // size N
};

/*
 * Run one iSLIP scheduling cycle for an NxN input-queued switch.
 *
 * Parameters:
 *   N              - Number of input ports and output ports.
 *   request_matrix - NxN binary matrix:
 *                    request_matrix[i][j] == 1 means input i requests output j.
 *   in_ptr         - Per-input round-robin accept pointers (1-based).
 *   out_ptr        - Per-output round-robin grant pointers (1-based).
 *   max_iters      - Maximum number of grant/accept iterations in the cycle.
 *
 * Returns:
 *   ISlipResult containing:
 *     - match_matrix : NxN binary matrix of accepted matches
 *     - in_ptr       : updated input pointers
 *     - out_ptr      : updated output pointers
 */
ISlipResult islip_one_cycle(
    int N,
    const std::vector<std::vector<int>>& request_matrix,
    std::vector<int> in_ptr,
    std::vector<int> out_ptr,
    int max_iters
);
