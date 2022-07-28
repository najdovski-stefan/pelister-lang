<h1>
  Pelister Programming Language
  <img src="peli-logo.png" alt="Peli Logo" style="height:80px; vertical-align:middle; margin-left:10px;">
</h1>


A simple, stack-based interpreted programming language inspired by Forth.

## Usage

### Building:

```bash
mkdir build
cd build
cmake ..
make
```

### Run a file:
```bash
./bin/pelilang programs/bubble_sort.peli
```

### Run interactive REPL:
```bash
./bin/pelilang --repl
```
*Type `bye` to exit. Comments are enclosed in `( ... )`.*

## Example: Defining a Function

```forth
: SQUARE DUP * ;  ( Defines a function named SQUARE )
5 SQUARE          ( Puts 25 on the stack )
.                 ( Prints and pops 25 )
```

## Supported Operations

### Core Stack Manipulation
| Word | Name | Stack Effect | Description |
| :--- | :--- | :--- | :----------- |
| `DUP`| Duplicate | `( a -- a a )` | Duplicates the top number. |
| `DROP`| Drop | `( a -- )` | Discards the top number. |
| `SWAP`| Swap | `( a b -- b a )`| Swaps the top two numbers. |
| `OVER`| Over | `( a b -- a b a )` | Copies the second number to the top. |
| `ROT`| Rotate | `( a b c -- b c a )` | Rotates the top three numbers. |
| `>R` | To Return | `( a -- )` | Moves the top of the data stack to the return stack. |
| `R>` | From Return | `( -- a )` | Moves the top of the return stack to the data stack. |
| `R@` | Fetch Return| `( -- a )` | Copies the top of the return stack to the data stack. |

### Arithmetic & Logical
| Word | Name | Stack Effect | Description |
| :--- | :--- | :--- | :----------- |
| `+` | Add | `( a b -- sum )` | Adds `a` and `b`. |
| `-` | Subtract | `( a b -- diff )` | Subtracts `b` from `a`. |
| `*` | Multiply | `( a b -- prod )` | Multiplies `a` and `b`. |
| `/` | Divide | `( a b -- quot )` | Divides `a` by `b`. |
| `MOD`| Modulo | `( a b -- rem )` | Computes the remainder of `a / b`. |
| `AND`| Bitwise AND | `( a b -- res )` | Performs a bitwise AND. |
| `OR` | Bitwise OR | `( a b -- res )` | Performs a bitwise OR. |
| `NOT`| Logical NOT | `( a -- flag )`| Returns `1` if `a` is `0`, otherwise `0`. |

### Comparison
| Word | Name | Stack Effect | Description |
| :--- | :--- | :--- | :----------- |
| `=` | Equals | `( a b -- flag )` | Returns `1` if `a == b`, else `0`. |
| `<` | Less Than | `( a b -- flag )` | Returns `1` if `a < b`, else `0`. |
| `>` | Greater Than| `( a b -- flag )` | Returns `1` if `a > b`, else `0`. |

### Memory
| Word | Name | Stack Effect | Description |
| :--- | :--- | :--- | :----------- |
| `!` | Store | `( val addr -- )` | Stores value `val` at memory `addr`. |
| `@` | Fetch | `( addr -- val )` | Fetches the value from memory `addr`. |

### Defining Words & Control Flow
| Word | Name | Stack Effect | Description |
| :--- | :--- | :--- | :----------- |
| `:` | Colon | `( -- )` | Starts a new word definition. Usage: `: NAME ... ;` |
| `;` | Semicolon | `( -- )` | Ends a word definition. |
| `IF` | If | `( flag -- )` | Executes code until `ELSE` or `THEN` if `flag` is non-zero. |
| `ELSE`| Else | `( -- )` | Executes code until `THEN` if the `IF` condition was false. |
| `THEN`| Then | `( -- )` | Marks the end of an `IF...THEN` or `IF...ELSE...THEN` block. |
| `DO` | Do | `( limit start -- )` | Starts a loop that runs from `start` up to (but not including) `limit`. |
| `LOOP`| Loop | `( -- )` | Marks the end of a `DO...LOOP`. Increments the index by 1. |
| `I` | Loop Index I | `( -- index )`| Pushes the index of the innermost loop. |
| `J` | Loop Index J | `( -- index )`| Pushes the index of the next-outer loop. |
| `K` | Loop Index K | `( -- index )`| Pushes the index of the third-level loop. |

### Input/Output
| Word | Name | Stack Effect | Description |
| :--- | :--- | :--- | :----------- |
| `.` | Dot (Print)| `( a -- )` | Prints and removes the top number, followed by a space. |
| `.S` | Dot-S (Stack) | `( -- )` | Prints the entire contents of the stack without changing it. |
| `."`| Dot-Quote | `( -- )` | Prints the string that follows it, up to the next `"`. |
| `ACCEPT`| Accept | `( addr len -- len' )` | Reads a line of user input into a buffer at `addr`. |
| `>NUMBER`| To-Number | `( addr len -- num )`| Converts a string in memory to a number on the stack. |
| `CR` | Carriage Return| `( -- )` | Prints a newline character. |
---

## References

- [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language))
- [Google Test](https://github.com/google/googletest/)
- [Graphviz](https://graphviz.org/)
- [Linenoise](https://github.com/antirez/linenoise/)
