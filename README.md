# 3D Chunk Generator and Processor

This project is a Minecraft-inspired **3D chunk simulator** implemented in C, created as part of the second programming assignment for the _Computer Programming_ course (PCLP).

The assignment involves dynamically managing a 3D grid (chunk) of blocks using pointers, and implementing various generation, transformation, and compression operations on the chunk.

## Features

- Fully dynamic 3D matrix using `char***` pointers
- Block placement, cuboid/sphere filling
- 3D flood-fill algorithms (planar and full-space)
- Rotations and gravity-based chunk reshaping
- Lossless compression and decompression using RLE
- Memory-safe implementations (validated with Valgrind)

## Task Overview and Implementation

### 🧱 Task 1: Place Block
**Objective:** Insert a block at specific (x, y, z) coordinates in the chunk.  
**Implementation:**  
Checked whether the coordinates are within bounds, and inserted the given block directly in the 3D matrix.

### 🔳 Task 2: Fill Cuboid
**Objective:** Fill a cuboid defined by two opposite corners with a block type.  
**Implementation:**  
Iterated through the chunk in the given bounds and filled with the block, reusing the function from Task 1.

### 🟠 Task 3: Fill Sphere
**Objective:** Fill all positions strictly inside a sphere centered at (x, y, z) with radius `r`.  
**Implementation:**  
Traversed the chunk and checked if the distance to the center was less than the radius, then inserted the block accordingly.

### 🧊 Task 4: Shell Around Target Block
**Objective:** Surround every `target_block` with a `shell_block` on the 18-connected directions.  
**Implementation:**  
Marked all target blocks with a special `LIMIT` value to handle cases where `target_block == shell_block`. Then applied shell construction by checking 18-neighbors for each position.

### 🧬 Task 5: Flood Fill in xOz Plane
**Objective:** Replace all blocks connected in the xOz plane from a starting point.  
**Implementation:**  
Used a classic queue-based flood fill algorithm limited to 2D movement along the x and z axes.

### 🌐 Task 6: Full 3D Flood Fill
**Objective:** Generalize the flood fill algorithm to all six directions in 3D space.  
**Implementation:**  
Adapted the queue-based algorithm to explore in all directions: up, down, left, right, forward, back.

### 🔄 Task 7: Rotate Around Y Axis
**Objective:** Rotate the chunk 90° around the Y axis (in the xOz plane).  
**Implementation:**  
For each slice, rotated the planes in the xOz orientation, adjusting the chunk dimensions accordingly.

### 🪨 Task 8: Apply Gravity
**Objective:** Drop all connected solid bodies (non-air) down as far as possible, then trim empty space above.  
**Implementation:**  
Used Task 6’s flood fill to identify connected bodies, then repeatedly checked if each could fall one unit. Bodies were rebuilt using a secondary chunk and a `chunk_fill_connected()` helper to preserve shapes.

### 📦 Task 9: Encode Chunk (RLE)
**Objective:** Compress the chunk using run-length encoding (RLE) into a compact `unsigned char*` array.  
**Implementation:**  
Built an intermediate `runs` array using a custom struct and encoded it into bytes following the format specified in the statement.

### 🔓 Task 10: Decode Chunk
**Objective:** Reconstruct a chunk from a compressed `unsigned char*` RLE representation.  
**Implementation:**  
Parsed the RLE byte array into a `runs` array, then used it to rebuild the original 3D chunk with dynamically allocated memory.

### ✅ Task 11: Valgrind Validation
**Objective:** Ensure memory safety for tasks 7 through 10.  
**Implementation:**  
Validated the entire program using Valgrind (`--leak-check=full`) to ensure no memory leaks or invalid accesses were present.

## Structure

- `libchunk/`: Contains implementation files for generation, processing, transformation, and compression
- `Makefile`: Builds the project (`make`, `make run`, `make valgrind`)
- `README.md`: This file
- `README` : Original README file for the assignemt

## Build & Run

```bash
make           # Builds the project
make run       # Runs with default test
make valgrind  # Runs Valgrind for memory leak detection
```

## Notes

- All memory was **dynamically allocated and properly freed**.
- Coding style **adheres to the Cpp Core Guidelines**.
- All tasks **passed the Moodle checker** with a score of **100/100**.

## Course Info

📚 **Computer Programming (CB & CD)** — 2024  
🧠 **Task Score:** 100/100

## License

- This project is for educational purposes only.
- Minecraft is a trademark of Mojang Studios and Microsoft. This simulator is not affiliated with or endorsed by Mojang or Microsoft.
