# Pelister Programming Language

Simple Stack Based interpreted Programming Language, heavly inspired by Forth.


# REPL MODE

- just add the flag --repl


```bash
user@macbook build % ./pelilang --repl
```

You will get this:

```bash
Pelister-Lang REPL v1.0. Type 'bye' to exit.
>
```

## Programs to try:

1. Arithmetic

```bash
> (COMMENT)
> 10 5       ( Stack: 10 5 )
> +          ( Pops 5, pops 10, calculates 10+5=15, pushes 15 )
<stack bottom> 15 <top>
```

- Example:

```bash
> 10 5 +
> 70 1 -
> 42 10 *
> 28980 420 /
```

---

| Word | Name | Stack Effect | Description |
| :--- | :--- | :--- | :----------- |
| `+` | Add | `( a b -- sum )` | Adds the top two numbers. |
| `-` | Subtract | `( a b -- diff )` | Subtracts `b` from `a`. |
| `*` | Multiply | `( a b -- prod )` | Multiplies the top two numbers. |
| `/` | Divide | `( a b -- quot )` | Divides `a` by `b`. |
| `DUP`| Duplicate | `( a -- a a )` | Duplicates the top number. |
| `DROP`| Drop | `( a -- )` | Discards the top number. |
| `SWAP`| Swap | `( a b -- b a )`| Swaps the top two numbers. |
| `OVER`| Over | `( a b -- a b a )` | Copies the second number to the top. |
| `ROT`| Rotate | `( a b c -- b c a )` | Rotates the top three numbers. |
| `.` | Dot (Print)| `( a -- )` | Prints and removes the top number. |
| `CR` | Carriage Return| `( -- )` | Prints a newline. |

---

## References to libs and other resources:

- [Forth](https://en.wikipedia.org/wiki/Forth_(programming_language))
- [Google Test](https://github.com/google/googletest/)
- [Graphviz](https://graphviz.org/)
