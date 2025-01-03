# Multi-Threaded Kernel Matrix

A program that performs multi-threaded matrix multiplication and integrates with a Linux kernel module to handle thread information.

## Features
- **Kernel Module (`My_proc.c`)**:
  - Creates `/proc/thread_info` to record thread information (ID, execution time, context switches).
  - Outputs all recorded thread information when read.
- **Userspace Program (`MT_matrix.c`)**:
  - Executes multi-threaded matrix multiplication.
  - Writes thread information to `/proc/thread_info`.

## Input and Output
- **Input**: Matrix files (e.g., `m1_1.txt`, `m2_1.txt`).
- **Output**: Thread information can be accessed via `/proc/thread_info`.

---

## Usage

### Kernel Module
1. Compile the kernel module:
   ```bash
   make
2. Load the module:
   ```bash
   make load

### Userspace Program
1. Compile the program:
   ```bash
   gcc -o MT_matrix MT_matrix.c -lpthread
2. Run the program:
   ```bash
   sudo ./MT_matrix 2 m1_1.txt m2_1.txt

### Testing
1. Verify thread information:
   ```bash
   cat /proc/thread_info

---

## Notes
- Ensure the kernel module is loaded before running the userspace program.
- Repeat testing with different thread counts and matrix files (e.g., m2_1.txt, m2_2.txt).
   
