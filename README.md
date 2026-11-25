# Arbitrary Precision Calculator (APC)

A command-line Arbitrary Precision Calculator written in C using doubly linked lists. It performs arithmetic on integers of arbitrary length beyond standard `long long` limits by storing numbers in base-10000 chunks.

## Features
- **Supported Operations:** Addition (`+`), Subtraction (`-`), Multiplication (`*`), Division (`/`), Modulo (`%`), and Exponentiation (`^`)
- **Signed Arithmetic:** Handles positive and negative operands across all basic operations
- **Input Validation:** Verifies operand formatting, signs, and operator validity before execution
- **Memory Management:** Frees all dynamically allocated doubly linked list nodes on both normal completion and error paths

## Project Structure
- `calculate.h` — Data structures (`Dlist`), `BASE` definition (`10000`), status enums, and function prototypes
- `main.c` — CLI argument handling, sign resolution, operation dispatch, and memory cleanup
- `func.c` — Input validation, string-to-list conversion, list insertion/deletion, comparison, and printing
- `add.c` / `sub.c` — Addition with carry and magnitude subtraction with borrow
- `mul.c` — Chunk-by-chunk multiplication with partial product shifting
- `div.c` — Base-10000 long division using binary search to find each quotient chunk
- `pow.c` — Integer exponentiation for non-negative exponents

## Build & Run
```bash
gcc main.c func.c add.c sub.c mul.c div.c pow.c -o apc
./apc <operand1> <operator> <operand2>