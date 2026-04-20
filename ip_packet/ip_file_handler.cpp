#include "ip_file_handler.h"

#include <fstream>
#include <vector>

/*
 * IPv4 Packet Builder & Parser
 *
 * This module builds and parses simplified IPv4 packets:
 * - Fixed 20-byte header (no options)
 * - Custom protocol field (DN = 143)
 * - Max payload: 256 bytes
 *
 * Includes:
 * - Big-endian encoding/decoding
 * - Header checksum computation
 * - Packet validation on read
 */

namespace {

    const unsigned char IP_VERSION        = 4;
    const unsigned char IP_IHL            = 5;    // 5 * 4 = 20 bytes
    const int           IP_HEADER_LEN     = 20;
    const int           MAX_DATA_PER_PKT  = 256;
    const unsigned char IP_TTL            = 200;
    const unsigned char IP_PROTOCOL_DN    = 143;

    // Compute IPv4 header checksum
    unsigned short ip_header_checksum(const unsigned char* header, int length) {
        unsigned long sum = 0;

        for (int i = 0; i < length; i += 2) {
            unsigned short word = static_cast<unsigned short>(header[i]) << 8;
            if (i + 1 < length) {
                word |= static_cast<unsigned short>(header[i + 1]);
            }
            sum += word;

            while (sum > 0xFFFF) {
                sum = (sum & 0xFFFF) + (sum >> 16);
            }
        }

        return static_cast<unsigned short>(~sum & 0xFFFF);
    }

    inline void write_uint16_be(unsigned char* buf, int offset, unsigned short value) {
        buf[offset]     = (value >> 8) & 0xFF;
        buf[offset + 1] = value & 0xFF;
    }

    inline void write_uint32_be(unsigned char* buf, int offset, unsigned int value) {
        buf[offset]     = (value >> 24) & 0xFF;
        buf[offset + 1] = (value >> 16) & 0xFF;
        buf[offset + 2] = (value >> 8)  & 0xFF;
        buf[offset + 3] = value & 0xFF;
    }

    inline unsigned short read_uint16_be(const unsigned char* buf, int offset) {
        return (buf[offset] << 8) | buf[offset + 1];
    }

    inline unsigned int read_uint32_be(const unsigned char* buf, int offset) {
        return (buf[offset] << 24) |
               (buf[offset + 1] << 16) |
               (buf[offset + 2] << 8) |
               buf[offset + 3];
    }
}


/* -------------------- FILE → PACKETS -------------------- */

bool ip_file_handler::file_to_ip_packets(
    std::string input_file_name,
    std::string output_file_name,
    unsigned int source_ip,
    unsigned int destination_ip)
{
    std::ifstream in(input_file_name, std::ios::binary);
    if (!in.is_open()) return false;

    std::ofstream out(output_file_name, std::ios::binary);
    if (!out.is_open()) return false;

    char data_buf[MAX_DATA_PER_PKT];

    while (true) {
        in.read(data_buf, MAX_DATA_PER_PKT);
        std::streamsize bytes_read = in.gcount();

        if (bytes_read == 0) break;

        unsigned char header[IP_HEADER_LEN] = {0};

        header[0] = (IP_VERSION << 4) | IP_IHL;
        header[8] = IP_TTL;
        header[9] = IP_PROTOCOL_DN;

        unsigned short total_length = IP_HEADER_LEN + bytes_read;
        write_uint16_be(header, 2, total_length);

        write_uint32_be(header, 12, source_ip);
        write_uint32_be(header, 16, destination_ip);

        unsigned short checksum = ip_header_checksum(header, IP_HEADER_LEN);
        write_uint16_be(header, 10, checksum);

        out.write(reinterpret_cast<const char*>(header), IP_HEADER_LEN);
        out.write(data_buf, bytes_read);

        if (!out.good()) return false;

        if (in.eof()) break;
    }

    return true;
}


/* -------------------- PACKETS → FILE -------------------- */

bool ip_file_handler::ip_packets_to_file(
    std::string input_file_name,
    std::string output_file_name,
    unsigned int source_ip,
    unsigned int destination_ip)
{
    std::ifstream in(input_file_name, std::ios::binary);
    if (!in.is_open()) return false;

    std::ofstream out(output_file_name, std::ios::binary);
    if (!out.is_open()) return false;

    while (true) {
        unsigned char header[IP_HEADER_LEN];

        in.read(reinterpret_cast<char*>(header), IP_HEADER_LEN);
        if (in.gcount() == 0) break;
        if (in.gcount() < IP_HEADER_LEN) return false;

        unsigned char version = header[0] >> 4;
        unsigned char ihl     = header[0] & 0x0F;

        if (version != IP_VERSION || ihl != IP_IHL) return false;

        int header_len = ihl * 4;
        unsigned short total_length = read_uint16_be(header, 2);

        if (total_length < header_len) return false;

        int data_len = total_length - header_len;

        unsigned char protocol = header[9];
        unsigned int src = read_uint32_be(header, 12);
        unsigned int dst = read_uint32_be(header, 16);

        bool checksum_ok = (ip_header_checksum(header, IP_HEADER_LEN) == 0);

        std::vector<char> data(data_len);
        if (data_len > 0) {
            in.read(data.data(), data_len);
            if (in.gcount() < data_len) return false;
        }

        if (src == source_ip &&
            dst == destination_ip &&
            protocol == IP_PROTOCOL_DN &&
            checksum_ok)
        {
            out.write(data.data(), data_len);
            if (!out.good()) return false;
        }

        if (in.eof()) continue;
    }

    return true;
}
