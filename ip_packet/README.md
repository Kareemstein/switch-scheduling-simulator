# IP Packet Builder & Parser (C++)

## 📌 Overview
This module implements a simplified IPv4 packet builder and parser.

It supports:
- Converting a file into a sequence of IPv4 packets
- Reconstructing the original file from those packets
- Validating packets using header fields and checksum

---

## ⚙️ Features

- Fixed IPv4 header (20 bytes, no options)
- Payload size up to 256 bytes
- Big-endian encoding/decoding
- Header checksum calculation
- Packet filtering by:
  - Source IP
  - Destination IP
  - Protocol field
  - Checksum validity

---

## 🧠 Key Concepts Demonstrated

- IPv4 header structure
- Binary data manipulation
- Endianness (big-endian encoding)
- Checksum computation
- Packet parsing and validation

---

## 🚀 Usage

### Convert file to packets
Splits a file into multiple IPv4 packets.

### Reconstruct file
Reads packets, validates them, and rebuilds the original file.

---

## 🛠 Technologies

- C++
- Binary file I/O
- Low-level data handling

---

## 📎 Notes

- Uses a custom protocol number (143)
- Only valid packets are reconstructed
- Designed for educational and systems-level understanding
