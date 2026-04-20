#pragma once
#include <string>

/*
 * IP File Handler
 *
 * Provides utilities to:
 * 1. Convert a raw file into a sequence of IPv4-like packets
 * 2. Reconstruct a file from such packets with validation
 *
 * Each packet contains:
 * - 20-byte IPv4 header (no options)
 * - up to 256 bytes of payload
 *
 * Packets are written sequentially to a binary file.
 */
class ip_file_handler {
public:

    /*
     * Split a file into IPv4 packets and write them to an output file.
     *
     * Parameters:
     *   input_file_name  - original file
     *   output_file_name - file containing serialized packets
     *   source_ip        - source IP address (host-order)
     *   destination_ip   - destination IP address (host-order)
     *
     * Returns:
     *   true on success, false on failure
     */
    static bool file_to_ip_packets(
        std::string input_file_name,
        std::string output_file_name,
        unsigned int source_ip,
        unsigned int destination_ip
    );

    /*
     * Reconstruct a file from a sequence of IPv4 packets.
     *
     * Only packets that pass validation are used:
     * - correct source/destination IP
     * - correct protocol
     * - valid checksum
     *
     * Parameters:
     *   input_file_name  - file containing packets
     *   output_file_name - reconstructed output file
     *   source_ip        - expected source IP
     *   destination_ip   - expected destination IP
     *
     * Returns:
     *   true on success, false on failure
     */
    static bool ip_packets_to_file(
        std::string input_file_name,
        std::string output_file_name,
        unsigned int source_ip,
        unsigned int destination_ip
    );
};
