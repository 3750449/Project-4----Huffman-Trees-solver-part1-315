## Student Information

**Name:** Desmond Farley-Williams
**Student ID:** 008508702
**Repository:**

---

## Collaboration & Sources

- **cppreference.com** — consulted for `std::fstream`, `std::tolower`, `std::filesystem`
- **geeksforgeeks.org** — cross-checked C++ I/O examples and syntax

---

## Implementation Details — **Project 4, Part 1**

Reconstruct the original token stream using only a Huffman header (.hdr) and code (.code) file and output must exactly match the encoder’s <base>.tokens
- '<base>.hdr' — one line per leaf: word<space>code (code is a string of 0/1)
- '<base>.code' — ASCII 0/1 with possible newlines to ignore

**Determinism & Rules**
- Priority queue is implemented as a **vector kept sorted** by the same comparator (so **min is at back**)
- Huffman construction always attaches **left=a (0), right=b (1)** when merging the two minima
- **Single-word edge case:** that word’s code is `"0"`
- **Empty input:** all outputs exist and contain exactly one trailing newline

### Files Implemented

- `PriorityQueue.hpp` / `PriorityQueue.cpp` — vector + sort (simulated PQ)
- `utils.hpp` / `utils.cpp` — error codes, filesystem checks, error reporting
- `main.cpp` — driver wiring modules and writing outputs
- `CMakeLists.txt` — build configuration
- `HuffmanTree.hpp` / `HuffmanTree.cpp` — build Huffman tree, write header

### PriorityQueue (vector+sort stand-in)

- Builds from BST’s `(word,count)` vector
- Sorted invariant `.freq`:
- **Primary:** count **descending** (alphabetical)

**HuffmanTree**
- `buildFromCounts(countsLex)` — build using the PQ loop; **left=0**, **right=1**; parent’s `key_word = min(left.key_word, right.key_word)`
- `writeHeader(os)` — **preorder over leaves**; line is `word<space>code\n`; if tree empty, write a single `\n`
- `encode(tokens, os, 80)` — builds codebook once; streams codes; **wraps at 80 columns**, always ends with a newline

---

## Outputs

- **`<base>.tokens`**
- One token per line
- **Ends with exactly one trailing newline** (even if the input is empty)

**`<base>.hdr`**
- **Preorder over leaves**; `word<space>code`
- **Single-word** → that word gets code `"0"`
- Always ends with a trailing newline; empty → single `\n`

**`<base>.code`**
- ASCII `0/1` only; **hard wrap exactly 80 chars per line**
- Always ends with a trailing newline; empty tokens → single `\n`

### Required error handling

Usage:
two args: <header.hdr> <bits.code>, verify both inputs are readable and output file can be created
Code file must contain only 0/1 plus line breaks

Decode-time validation:
Hitting a non-existent child while walking bits then error
If the file ends while not at the root (mid-symbol) then error

Empty cases:
Empty header/tree or no decoded words then write a single newline
(All errors are reported via utils.hpp/utils.cpp)

---

## Testing & Status

### Sample run (The Bells)

input:
input_output/TheBells.tokens

**Excerpt of `input_output/The Bells.tokens_decoded:**

edgar
allan
poe
s
the
bells
hear
the
sledges
with
the
bells
silver
bells
what
a
world
of
merriment
their
melody
foretells
how
they
tinkle
tinkle
tinkle
in
the
icy
air
of
night
while
the
stars
that
oversprinkle
all
the
heavens
seem
to
twinkle
with
a
crystalline
delight
keeping
time
time
time
in
a
sort
of
runic
rhyme
to
the
tintinnabulation
that
so
musically
wells
from
the
bells
bells
bells
bells
bells
bells
bells
from
the
jingling
and
the
tinkling
of
the
bells

- Files:

- `empty.hdr`  → **exactly one newline**
- `empty.code` → **each is a single newline**

---

## How to Compile & Run

### Plain g++

g++ -std=c++20 -Wall HuffmanTree.cpp main.cpp PriorityQueue.cpp utils.cpp -o huff_decode.x

CMake

mkdir -p build && cd build
cmake ..
make

# Run the produced executable
./p4_part1 <file>.hdr <file>.code