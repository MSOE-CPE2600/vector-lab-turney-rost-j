# Vector Calculator with Dynamic Memory

Vector lab that uses dynamic memory for resizing with the `realloc` function.  
It also includes new **save** and **load** functionality that can store and retrieve vector data from CSV files.

---

## Overview
This program is a **C-based Vector Calculator** that demonstrates the use of **dynamic memory allocation** and **file I/O**.  
Users can create, modify, and save vectors, then reload them later for continued use.  
The project emphasizes clean memory management and modular design.

---

## Features
- **Dynamic Memory Resizing** using `malloc` and `realloc`
 - The program checks if the size is full 
 - Doubles the size and moves the data over
- **Interactive Menu System** for managing vectors
- **CSV File Support** for saving and loading vectors
- **Error Handling** for invalid input and file operations
- **Memory Safety** with proper use of `free`

---
## Commands
name = x y z         Create or replace a vector (e.g., a = 1 2 3)
list                 List all stored vectors
clear                Remove all stored vectors
save <file>          Ability to save to existing or new file
load <file>          Need to load from an existing file
name                 Display a single vector (e.g., a)
a + b, a - b         Vector addition and subtraction
a * b                Dot product (scalar result)
a x b                Cross product
2 * a or a * 2       Scalar multiplication
quit                 Exit the program

## File Descriptions
| File | Description |
|------|--------------|
| `main.c` | Handles user interaction and menu system |
| `vector.c` | Implements vector creation, resizing, and manipulation |
| `vector.h` | Declares vector structure and function prototypes |
| `fileio.c` | Contains save and load functions for CSV I/O |
| `fileio.h` | Header file for CSV functions |
| `Makefile` | Automates build and clean operations |

---

## Build Instructions
To compile the program with valgrind:
```bash
make clean
make
valgrind ./vectorcalc 
