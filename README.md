# Pelister Programming Language

A simple, stack-based interpreted programming language inspired by Forth.

## Usage

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

### Stack
| Word | Effect | Description |
| :--- | :--- | :----------- |
| `DUP`| `( a -- a a )` | Duplicates top item. |
| `DROP`| `( a -- )` | Discards top item. |
| `SWAP`| `( a b -- b a )`| Swaps top two items. |
| `OVER`| `( a b -- a b a )` | Copies 2nd item to top. |
| `ROT`| `( a b c -- b c a )` | Rotates top three items. |
| `>R` | `( a -- )` | Move item to return stack. |
| `R>` | `( -- a )` | Move item from return stack. |
| `R@` | `( -- a )` | Copy item from return stack. |

### Math & Logic
| Word | Effect | Description |
| :--- | :--- | :----------- |
| `+` `-` `*` `/` | `( a b -- res )` | Standard arithmetic. |
| `MOD`| `( a b -- rem )` | Remainder of `a / b`. |
| `AND` `OR` | `( a b -- res )` | Bitwise operations. |
| `NOT`| `( a -- flag )` | Logical not (0 -> 1, else -> 0). |
| `=` `<` `>` | `( a b -- flag )` | Comparisons (return 1 or 0). |

### Memory
| Word | Effect | Description |
| :--- | :--- | :----------- |
| `!` | `( val addr -- )` | Store `val` at `addr`. |
| `@` | `( addr -- val )` | Fetch value from `addr`. |

### Control Flow & Functions
| Word | Description |
| :--- | :----------- |
| `:` `;` | Define a new word. Usage: `: NAME ... ;` |
| `IF ELSE THEN` | Conditional execution. |
| `DO LOOP` | Counted loop. Usage: `limit start DO ... LOOP` |
| `I` `J` `K` | Loop counters for nested loops (inner to outer). |

### I/O
| Word | Description |
| :--- | :----------- |
| `.` | Print and pop the top of the stack. |
| `.S` | Print the entire stack without changing it. |
| `CR` | Print a newline. |

---

## References

- [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language))
- [Google Test](https://github.com/google/googletest/)
- [Graphviz](https://graphviz.org/)
