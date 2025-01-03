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
2. Verify `/proc/thread_info` creation:
   ```bash
   ls /proc/thread_info

### Compile and Run the Multi-Threaded Program
1. Compile the program:
   ```bash
   gcc -pthread -o MT_matrix MT_matrix.c
2. Run the program:
   ```bash
   ./MT_matrix <number_of_threads> <matrix1_file> <matrix2_file>

### Example
1. Run the kernel module:
   ```bash
   sudo insmod My_proc.ko
2. Perform matrix multiplication:
   ```bash
   ./MT_matrix 4 m1_1.txt m2_1.txt
3. Check thread information:
   ```bash
   cat /proc/thread_info
4. Unload the kernel module:
   ```bash
   sudo rmmod My_proc

---

## Notes
- Ensure the kernel module is loaded before running the matrix multiplication program.
- Use valid input matrix files (e.g., m1_1.txt, m2_1.txt) with appropriate dimensions.
   
