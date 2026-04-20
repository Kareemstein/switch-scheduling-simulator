#include "islip.h"

#include <iostream>
#include <vector>

/*
 * Print a binary matrix.
 */
static void print_matrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int value : row) {
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }
}

/*
 * Print a vector of round-robin pointers with a label.
 */
static void print_ptrs(const std::vector<int>& ptrs, const char* name) {
    std::cout << name << ": ";
    for (int value : ptrs) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

int main() {
    const int N = 5;
    const int max_iters = 10;

    /*
     * request_matrix[i][j] = 1 iff VOQ(i,j) > 0
     *
     * Rows = inputs  (I1..I5)
     * Cols = outputs (O1..O5)
     */
    std::vector<std::vector<int>> request_matrix = {
        {1, 0, 1, 0, 0},  // I1 -> O1, O3
        {0, 1, 1, 0, 1},  // I2 -> O2, O3, O5
        {0, 0, 0, 0, 1},  // I3 -> O5
        {1, 0, 1, 1, 0},  // I4 -> O1, O3, O4
        {0, 1, 0, 1, 0}   // I5 -> O2, O4
    };

    /*
     * Initial 1-based round-robin pointers.
     */
    std::vector<int> in_ptr  = {1, 3, 3, 2, 5};
    std::vector<int> out_ptr = {2, 3, 3, 1, 1};

    ISlipResult result = islip_one_cycle(N, request_matrix, in_ptr, out_ptr, max_iters);

    std::cout << "Matching matrix:\n";
    print_matrix(result.match_matrix);

    std::cout << '\n';
    print_ptrs(result.in_ptr, "Updated input pointers");
    print_ptrs(result.out_ptr, "Updated output pointers");

    std::cout << "\nMatched pairs (input -> output):\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (result.match_matrix[i][j] == 1) {
                std::cout << "I" << (i + 1) << " -> O" << (j + 1) << '\n';
            }
        }
    }

    return 0;
}
