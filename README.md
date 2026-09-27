# Jobless Joke Language (JJL)

### An esoteric programming language with slightly fewer inconveniences.

Welcome to **Jobless Joke Language (JJL)**, an esoteric programming language inspired by the classic Brainf*** programming language.

Originally introduced in 1993, Brainf*** established a remarkably minimalist approach to programming through eight fundamental instructions and a tape-based memory model.

JJL builds upon that foundation with a few additional conveniences:

- **Absolute memory navigation** through `@` sequences.
- **Multiplier arithmetic** using `*` chains.
- **Non-destructive memory inspection** with `$`.
- **Inline conditional expressions** supporting equality and inequality.
- **Dynamic memory expansion** for growing programs and questionable ambitions.

JJL preserves the simplicity of its predecessor while introducing additional control over memory access and arithmetic.

It is intentionally compact, unconventional, and designed for programmers who enjoy understanding exactly what their computer is doing.

Or at least pretending they do.

---

## 1. Interpreter

JJL is executed through **JJLI (Jobless Joke Language Interpreter)**.

JJLI interprets `.jjl` source files directly, executing instructions against a dynamically expandable memory tape.

### Running a Program

```bash
./jjl program.jjl
```

JJL source files use the `.jjl` extension.

The interpreter processes the source and executes its instructions sequentially, including its extended navigation, arithmetic, and conditional operations.

---

## 2. Memory Model

JJL uses a dynamically expandable tape consisting of unsigned 8-bit memory cells.

| Property | Behavior |
|---|---|
| Initial tape size | 32,768 cells |
| Cell width | 8 bits |
| Minimum value | 0 |
| Maximum value | 255 |
| Arithmetic | Wrapping |
| Expansion | 1,024 cells |
| Initial pointer | Cell 0 |

Each cell stores a value between 0 and 255.

Incrementing a cell containing 255 wraps its value to 0. Decrementing a cell containing 0 wraps it to 255.

The tape uses zero-based indexing.

The memory pointer cannot move to a negative position.

---

## 3. Instruction Set

JJL retains the eight fundamental instructions of Brainf*** while introducing additional operators for memory navigation, inspection, arithmetic, and conditional execution.

### 3.1 Standard Instructions

| Instruction | Operation | Description |
|---|---|---|
| `>` | Move right | Advance the memory pointer by one cell. |
| `<` | Move left | Move the memory pointer back by one cell. |
| `+` | Increment | Increase the active cell by one. |
| `-` | Decrement | Decrease the active cell by one. |
| `.` | Output | Print the active cell as an ASCII character. |
| `,` | Input | Read a byte from standard input. |
| `[` | Loop start | Skip the loop if the active cell is zero. |
| `]` | Loop end | Return to the matching `[` if the active cell is nonzero. |

These instructions follow the conventional Brainf*** execution model.

Existing Brainf*** programs using the standard instruction set remain compatible with JJL.

### 3.2 Extended Instructions

| Instruction | Name | Description |
|---|---|---|
| `@` | Absolute navigation | Jump directly to an absolute memory address. |
| `$` | Memory inspection | Evaluate the active cell's numerical value without modifying it. |
| `*` | Multiplier | Expand an arithmetic operation into multiples of ten. |
| `==` | Equality | Compare two operands for equality. |
| `!=` | Inequality | Compare two operands for inequality. |
| `:` | Conditional separator | Separate the condition and its execution branches. |

The extended instructions are designed to reduce repetitive operations while retaining JJL's compact syntax.

---

## 4. Absolute Memory Navigation

### The `@` Operator

Repeated `@` symbols move the pointer directly to an absolute memory address.

The number of symbols determines the destination.

| Syntax | Destination |
|---|---|
| `@` | Cell 0 |
| `@@` | Cell 1 |
| `@@@` | Cell 2 |
| `@@@@` | Cell 3 |
| `@@@@@` | Cell 4 |
| `@` × N | Cell N − 1 |

For example:

```jjl
@ +++++
@@@ ++++++++++
@@@@@ +++
```

This initializes:

- Cell 0 to 5.
- Cell 2 to 10.
- Cell 4 to 3.

Absolute navigation does not require intermediate pointer movements.

The pointer simply arrives at its destination. No sightseeing required.

---

## 5. Memory Inspection

### The `$` Operator

The `$` operator evaluates the numerical value stored in the currently active cell.

It does not modify the cell or move the pointer.

This allows programs to inspect memory without consuming or altering its contents.

Example:

```jjl
@ +++++
$ == +++++ : @@@ : @@
```

The condition compares the active cell against the integer literal `5`.

If the values are equal, execution jumps to Cell 2.

Otherwise, execution jumps to Cell 1.

Memory inspection is particularly useful in conditional expressions.

---

## 6. Multiplier Arithmetic

### The `*` Operator

JJL introduces multiplier chains to simplify repetitive arithmetic operations.

Each `*` appended to a `+` or `-` operation contributes an additional multiple of ten.

Trailing arithmetic operators contribute individual increments or decrements.

### Examples

| Expression | Result |
|---|---:|
| `+*` | +10 |
| `+**` | +20 |
| `+***` | +30 |
| `+*++` | +12 |
| `-**` | -20 |
| `-**--` | -22 |

The multiplier mechanism reduces the number of instructions required for common arithmetic operations.

It also makes numerical assignments considerably easier to read.

For canonical arithmetic formatting conventions, see [Canonical Coding Style](#9-canonical-coding-style).

---

## 7. Conditional Expressions

JJL supports inline conditional expressions using equality and inequality comparisons.

This allows programs to evaluate values and select an execution branch without constructing additional loops.

### 7.1 Syntax

The general syntax is:

```jjl
LEFT_OPERAND OPERATOR RIGHT_OPERAND : IF_ACTION : ELSE_ACTION
```

The interpreter evaluates both operands and performs the comparison.

If the condition is true, the IF action executes.

Otherwise, the ELSE action executes.

Each branch executes a single instruction.

### 7.2 Supported Comparison Operators

| Operator | Meaning |
|---|---|
| `==` | Equal to |
| `!=` | Not equal to |

### 7.3 Supported Operands

| Operand | Meaning |
|---|---|
| `$` | Value of the active cell |
| `@` | Value stored in Cell 0 |
| `@@` | Value stored in Cell 1 |
| `@@@` | Value stored in Cell 2 |
| `<` | Value of the left neighboring cell |
| `>` | Value of the right neighboring cell |
| `+` | Positive integer literal |
| `-` | Negative integer literal |

Repeated `@` symbols reference successive absolute memory addresses.

Repeated `+` and `-` symbols represent signed integer literals.

Relative neighbor operands inspect adjacent cells without moving the pointer.

### 7.4 One-Line Conditional

For simple conditions and short branches, use the compact form:

```jjl
$ == @@ : @@@ : @@@@
```

This compares the active cell against Cell 1.

- If equal, jump to Cell 2.
- Otherwise, jump to Cell 3.

The one-line form is intended for straightforward conditional operations.

### 7.5 Expanded Conditional Formatting

When a conditional expression becomes difficult to read, use the expanded layout.

```jjl
$ == @@
: @@@
: @@@@
```

The three-line convention separates the comparison from its two branches.

This formatting is especially useful when documenting longer expressions or when the individual branches require additional explanation.

The colon delimiters retain their normal syntactic meaning.

**Note:** Expanded formatting is a readability convention. Each conditional branch still executes one instruction under the current JJLI implementation.

---

## 8. Comments and Whitespace

JJL supports comments beginning with `#`.

Comments extend to the end of the line and are ignored during execution.

Example:

```jjl
# Initialize comparison values

@ +++++
@@ +++++

# Compare the values

@ $ == @@ : @@@ : @@@@
```

Whitespace may be used to separate instructions and improve source readability.

Canonical JJL programs use whitespace and comments to make memory assignments and conditional expressions easier to understand.

---

## 9. Canonical Coding Style

JJL uses a small collection of formatting conventions intended to keep programs compact, consistent, and readable.

### Rule 1: Prefer Nearby Multiples of Ten

When assigning ASCII values, construct the target value from a nearby multiple of ten.

**Non-canonical:**

```jjl
+*******++++++++
```

**Canonical:**

```jjl
+********--
```

Both produce 78, corresponding to the ASCII character `N`.

The canonical form uses a nearby multiple of ten and a small correction.

### Rule 2: Consolidate Multiplier Chains

Avoid repeating individual multiplier blocks.

**Non-canonical:**

```jjl
+*+*+*
```

**Canonical:**

```jjl
+***
```

Both represent an increment of 30.

### Rule 3: Align Sequential Assignments

Use whitespace to align memory addresses and their corresponding operations.

```jjl
@ +********--
@@ +*******++++
@@@ +***+++
```

Consistent alignment makes sequential memory initialization easier to scan.

### Rule 4: Keep Small Loops Compact

Short loops should remain on a single line.

```jjl
[-]
```

### Rule 5: Format Complex Loops Structurally

Longer loops should use dedicated opening and closing brackets with two-space indentation.

```jjl
[
  @ $ == @@ : @@@ : @@@@
  @ -
]
```

### Rule 6: Separate Complex Conditional Branches

Use the expanded conditional layout when a comparison or its branches would become difficult to read in a single line.

```jjl
$ == @@
: @@@
: @@@@
```

Keep the condition and branches visually distinct.

---

## 10. Complete Example: YES / NO

The following program initializes two comparison values, checks whether they are equal, and prints the corresponding result.

```jjl
@@@ +********----
@@@@ +********--
@@@@@ +********+

@ +++++
@@ +++++

@ $ == @@ : @@@@@ : @@@@

.
```

### Expected Output

When both comparison values are equal:

```text
Y
```

When the values differ:

```text
N
```

### Memory Layout

| Cell | Value | Purpose |
|---|---:|---|
| 0 | 5 | First comparison value |
| 1 | 5 | Second comparison value |
| 2 | 0 | Unused |
| 3 | 78 | ASCII `N` |
| 4 | 89 | ASCII `Y` |

The program demonstrates the core JJL workflow:

1. Initialize memory cells.
2. Navigate directly to relevant addresses.
3. Evaluate a conditional expression.
4. Select an output character.
5. Print the result.

A compact demonstration of memory navigation, arithmetic, comparison, and output.

---

## 11. Design Philosophy

JJL is designed to remain **backwards compatible with Brainf***** while extending its original instruction model with additional capabilities.

The language preserves the familiar tape-based execution model and introduces optional operators for absolute navigation, multiplier arithmetic, memory inspection, and conditional branching.

Existing Brainf*** programs can therefore be used without requiring the extended JJL instructions.

JJL remains intentionally minimal, with its additional features focused on reducing repetitive operations and improving readability.
