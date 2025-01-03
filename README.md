# Multi-Threaded Kernel Matrix

A program that integrates multi-threaded matrix multiplication with a Linux kernel module to handle thread information, demonstrating inter-process communication via the `/proc` filesystem.

## Features

### Kernel Module (`My_proc.c`)
- Creates a proc file at `/proc/thread_info`.
- Records thread ID, execution time, and context switch counts when written to.
- Outputs all recorded thread information when read.

### Multi-Threaded Matrix Multiplication (`MT_matrix.c`)
- Performs matrix multiplication using multiple threads.
- Each thread:
  - Computes part of the matrix product.
  - Writes its thread ID to `/proc/thread_info`.
- Main thread:
  - Reads `/proc/thread_info` to retrieve thread statistics.

### Input
- **Input Files**:
  - Matrix data is read from input files (e.g., `m1_1.txt`, `m2_1.txt`).

---

## Usage

### Build and Load the Kernel Module
1. Compile the kernel module:
   ```bash
   make
   sudo insmod My_proc.ko
2. Verify /proc/thread_info creation:
