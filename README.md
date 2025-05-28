
# Recursive Descent Parser in C

## 📌 Objective
This project implements a **Recursive Descent Parser** in C to validate simple code structures such as:
- `if-else` statements
- `while` loops
- Arithmetic expressions with nested brackets

A unique identifier `LTD` (e.g., last three digits of your student ID) is embedded into the parsing logic for student-specific validation.

---

## ⚙️ Compilation Instructions

To compile the program, use the GCC compiler:

```bash
gcc parser.c -o parser
```

Replace `parser.c` with the actual file name if it's different.

---

## ▶️ Runtime Instructions

After compiling, run the program using:

```bash
./parser
```

The sample input is hardcoded in the `main()` function. You can modify the `input` string to test different code snippets.

Example in code:
```c
input = "{ if (a == LTD) { while (b < 100) { (a + b) * (b - LTD); } } else { (x + y) * (a - b); } }";
```

You can edit this string to test your own cases.

---

## ✅ Valid Test Cases

These test inputs should parse without errors:

### 1. Nested `if-else` and `while`:
```c
{ if (a == LTD) { while (b < 100) { (a + b) * (b - LTD); } } else { (x + y) * (a - b); } }
```

### 2. Simple expression with LTD:
```c
{ (a + LTD) * (LTD - b); }
```

---

## ❌ Invalid Test Cases

These inputs will trigger specific syntax errors:

### 1. Missing semicolon:
```c
{ a + b }
```

### 2. Invalid identifier:
```c
{ 3a = 5; }
```

### 3. Unbalanced brackets:
```c
{ if (a == b { a + b); } }
```

---

## 🛠 Error Handling

The parser provides detailed syntax error messages:
- Unexpected tokens
- Invalid identifiers
- Missing semicolons
- Mismatched brackets

Example output:
```
Syntax Error: Unexpected token at '}'
```

---

## 🧠 Educational Justification

This project demonstrates understanding in:
- Recursive parsing
- Lexical analysis
- Compiler frontend structure
- Modular programming

It reflects real-world parsing tasks akin to a simplified compiler or interpreter front-end.

---

## 👤 LTD Personalization

Make sure to replace the placeholder:
```c
#define LTD "285"  // Replace with your student ID last three digits
```

---

## 🏁 Output Example

```
✅ Program parsed successfully.
```

This confirms the input code passed all parsing rules.

---
