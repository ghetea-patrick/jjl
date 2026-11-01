# Jobless Joke Language (JJL)

**Eight familiar instructions. A few extra tricks. Still very much a tape.**

Jobless Joke Language, or **JJL**, is an esoteric programming language inspired by Brainf\*\*\*. Its interpreter is **JJLI**, the Jobless Joke Language Interpreter.

Brainf\*\*\* arrived in 1993 with eight fundamental instructions and a tape-based memory model. JJL keeps that compact foundation and adds absolute memory navigation, multiplier arithmetic, non-destructive inspection, and inline conditions. You still get close control over what the computer does, along with the occasional opportunity to wonder why you chose this hobby.

[What it does](#what-it-does) · [Get started](#get-started) · [A quick example](#a-quick-example) · [How it fits together](#how-it-fits-together) · [Reference](#reference) · [Examples](#examples) · [Good to know](#good-to-know) · [Design notes](#design-notes)

## What it does

- Preserves the eight standard Brainf\*\*\* instructions and their tape-based execution model.
- Jumps to absolute memory addresses with repeated `@` symbols.
- Builds arithmetic in multiples of ten with `*` chains.
- Inspects the active cell's numeric value with `$`, without changing it.
- Selects between single-instruction branches using `==` or `!=` conditions.
- Expands memory dynamically as programs grow and ambitions become questionable.
- Uses whitespace for readable source; comments are not supported.

## Get started

JJLI executes `.jjl` source files directly:

```bash
./jjl program.jjl
```

Use the `.jjl` extension for your source files. The interpreter reads the source and executes instructions sequentially against its expandable memory tape, including JJL's extended navigation, arithmetic, and conditional operations.

> [!NOTE]
> The command above is the documented interpreter invocation. The supplied project documentation does not include a build or installation procedure, so this README starts from an available `jjl` executable.

## A quick example

Start at Cell 0, add 80, subtract 2, and print the resulting ASCII character:

```jjl
@ +********-- .
```

Output:

```text
N
```

There are eight `*` symbols in the arithmetic chain: `+********` adds 80, and `--` adjusts it to 78, the ASCII value for `N`. The `.` instruction prints the character; it does not print the numeric value `78`.

## How it fits together

### The memory tape

JJL uses a dynamically expandable tape of unsigned 8-bit cells:

| Property | Behavior |
|---|---|
| Initial tape size | 32,768 cells |
| Cell width | 8 bits |
| Minimum value | 0 |
| Maximum value | 255 |
| Arithmetic | Wrapping |
| Expansion | 1,024 cells |
| Initial pointer | Cell 0 |

Each cell holds a value from 0 through 255. Arithmetic wraps: incrementing 255 produces 0, and decrementing 0 produces 255. The tape begins with 32,768 cells and expands in blocks of 1,024 cells.

Addresses use zero-based indexing, and the pointer starts at Cell 0. It cannot move to a negative position.

### Jump directly to an address

Repeated `@` symbols select an absolute memory cell. Count the symbols, then subtract one:

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

This initializes Cell 0 to 5, Cell 2 to 10, and Cell 4 to 3. Absolute navigation does not visit the cells in between; the pointer goes straight to its destination. No sightseeing required.

### Inspect a value without consuming it

`$` evaluates the active cell's numeric value without modifying the cell or moving the pointer. It is particularly useful inside a condition:

```jjl
@ +++++
$ == +++++ : @@@ : @@
```

The condition compares the active cell's value with the integer literal 5. If they are equal, the pointer jumps to Cell 2 (`@@@`); otherwise, it jumps to Cell 1 (`@@`).

### Count arithmetic in tens

Append `*` symbols to a `+` or `-` operation to build multiples of ten. Each star contributes another ten in that direction; trailing `+` or `-` operations add the individual adjustments.

| Expression | Result |
|---|---:|
| `+*` | +10 |
| `+**` | +20 |
| `+***` | +30 |
| `+*++` | +12 |
| `-**` | -20 |
| `-**--` | -22 |

`+**` means **+20**, not +100. The stars accumulate tens; they are not exponentiation. Multiplier chains reduce repetitive instructions and make numeric assignments easier to scan.

See [Canonical coding style](#canonical-coding-style) for conventions that keep these chains readable.

### Choose a branch with an inline condition

The general form is:

```jjl
LEFT_OPERAND OPERATOR RIGHT_OPERAND : IF_ACTION : ELSE_ACTION
```

The interpreter evaluates both operands and applies the comparison. A true result executes the IF action; a false result executes the ELSE action.

For a short condition, keep the expression on one line:

```jjl
$ == @@ : @@@ : @@@@
```

This compares the active cell with Cell 1. Equality selects Cell 2; inequality selects Cell 3.

When that becomes difficult to read, spread the same expression over three lines:

```jjl
$ == @@
: @@@
: @@@@
```

The first line is the comparison, the second is the true branch, and the third is the false branch. The colons keep their normal syntactic meaning across the line breaks.

> [!IMPORTANT]
> **Each conditional branch executes one instruction in the current JJLI implementation.** Expanded formatting improves readability; it does not turn a branch into a multi-instruction block.

### Use whitespace carefully

Whitespace separates instructions and makes source easier to read. It does not change the program's execution.

> [!CAUTION]
> **Comments are not supported by JJLI.** The interpreter ignores unrecognized characters, but it still executes any recognized JJL instruction characters that appear in supposed comment text. For example, in `# add + to the cell`, the `+` is still executed. Any other JJL instruction symbol in supposed comment text can also change the program. Do not add comments to `.jjl` source files.

## Reference

### Standard instructions

JJL retains the eight fundamental Brainf\*\*\* instructions:

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

The instructions follow the conventional execution model. Programs using the standard instruction set remain compatible without needing the extended JJL operators.

### Extended instructions

| Instruction | Name | Description |
|---|---|---|
| `@` | Absolute navigation | Jump directly to an absolute memory address. |
| `$` | Memory inspection | Evaluate the active cell's numerical value without modifying it. |
| `*` | Multiplier | Expand an arithmetic operation into multiples of ten. |
| `==` | Equality | Compare two operands for equality. |
| `!=` | Inequality | Compare two operands for inequality. |
| `:` | Conditional separator | Separate the condition and its execution branches. |

These additions reduce repetition while keeping the syntax compact. Remember that `.` outputs an ASCII character, while `$` evaluates a numeric value for inspection and comparison.

### Comparison operators

| Operator | Meaning |
|---|---|
| `==` | Equal to |
| `!=` | Not equal to |

### Conditional operands

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

Repeated `@` operands reference successive absolute addresses: one `@` reads Cell 0, two read Cell 1, and so on. In a condition, these operands provide stored values; as navigation instructions, `@` sequences move the pointer.

Repeated `+` and `-` symbols form signed integer literals. Relative `<` and `>` operands inspect neighboring cells **without moving the pointer**; the same symbols used as ordinary instructions move it.

### Canonical coding style

The conventions below keep JJL compact, consistent, and readable. They describe formatting style rather than adding new instructions.

#### Prefer nearby multiples of ten

When building ASCII values, start from a nearby multiple of ten and apply a small correction.

Non-canonical:

```jjl
+*******++++++++
```

Canonical:

```jjl
+********--
```

Both produce 78, the ASCII value for `N`. The canonical form makes the intended value easier to count.

#### Consolidate multiplier chains

Avoid repeating individual multiplier blocks.

Non-canonical:

```jjl
+*+*+*
```

Canonical:

```jjl
+***
```

Both increment by 30. One consolidated chain is easier to read than three separate ten-unit blocks.

#### Align sequential assignments

Use whitespace to align addresses with their operations:

```jjl
@   +********--
@@  +*******++++
@@@ +***+++
```

Consistent alignment makes sequential memory initialization easier to scan.

#### Keep small loops compact

A short loop can stay on one line:

```jjl
[-]
```

#### Give complex loops a structure

Use dedicated opening and closing brackets with two-space indentation for longer loops:

```jjl
[
  @ $ == @@ : @@@ : @@@@
  @ -
]
```

The indentation makes the loop body visible without changing execution.

## Examples

### Select an ASCII Y or N

This program initializes comparison values, checks equality, and selects the corresponding output character:

```jjl
@@@ [-]
@@@@ +********--
@@@@@ +*********-

@ +++++
@@ +++++

@ $ == @@ : @@@@@ : @@@@

.
```

When the comparison values are equal, the output is:

```text
Y
```

If you change one comparison value so the two differ, the output is:

```text
N
```

The example selects one character, `Y` or `N`, rather than printing the full words “YES” and “NO.” Its memory layout is:

| Cell | Value | Purpose |
|---|---:|---|
| 0 | 5 | First comparison value |
| 1 | 5 | Second comparison value |
| 2 | 0 | Unused |
| 3 | 78 | ASCII `N` |
| 4 | 89 | ASCII `Y` |

> [!TIP]
> ASCII `Y` is **89**, built here as `+*********-` (90 − 1). ASCII `N` is **78**, built as `+********--` (80 − 2). Cell 2 is explicitly cleared and left unused so the initialization matches the table.

The program demonstrates the full workflow:

1. Initialize the cells used by the example.
2. Navigate to the comparison values.
3. Evaluate a condition without changing those values.
4. Move to the selected output cell.
5. Print its ASCII character with `.`.

## Good to know

> [!IMPORTANT]
> The same compact symbols can have different roles in context. `@` navigates as an instruction but reads an addressed cell as a conditional operand. `<` and `>` move the pointer as instructions but inspect neighbors as operands. `$` inspects a numeric value; `.` prints a character.

- Cell arithmetic wraps within 0–255; it does not grow into an unrestricted integer stored in the tape.
- Memory addresses are zero-based, and negative pointer positions are not allowed.
- Dynamic expansion grows the tape in blocks of 1,024 cells from its initial 32,768-cell size.
- Conditional branches execute one instruction, whether written on one line or three.
- Whitespace is available for readability, but comments are not supported. Unrecognized characters are ignored individually; recognized JJL instructions inside supposed comments are still executed.
- Brainf\*\*\* programs using the standard eight instructions do not need the extensions.

## Design notes

JJL preserves the minimal tape-based execution model of Brainf\*\*\* and adds optional conveniences around it. Absolute navigation, multiplier arithmetic, inspection, and conditional branching give you more control over memory access and common operations without requiring the extended syntax in existing programs.

The language stays compact and unconventional, with its additions focused on reducing repetition and improving readability. You still know exactly which cells and instructions are involved. Whether that makes the programming choices more defensible is outside the interpreter's jurisdiction.
