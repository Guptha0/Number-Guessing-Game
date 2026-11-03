# Machine-Level & Bytecode Execution Breakdown

This document illustrates how high-level logic translates to assembly instructions, then into raw machine code (hexadecimal opcodes), and how the CPU microarchitecture (ALU, Register File, Flags register, Pipeline) processes them at runtime.

---

## 1. Assembly to Machine Opcode Mapping

Consider the core comparison and branching engine from Tier 4:

```assembly
; Register assignments: EAX = User Guess, EBX = Secret Number (e.g., 42)
cmp eax, ebx        ; Compare EAX with EBX
je  .match_found    ; Jump if equal (relative offset)
jl  .is_too_low     ; Jump if less than (relative offset)
jg  .is_too_high    ; Jump if greater than (relative offset)
```

### Binary Encoding Disassembly

| Virtual Address | Assembly Instruction | Hex Opcode Bytes | Instruction Format Breakdown |
| :--- | :--- | :--- | :--- |
| `0x004015A0` | `cmp eax, ebx` | `39 D8` | `39` = CMP r/m32, r32 opcode<br>`D8` = ModR/M byte: `11` (reg-direct) `011` (ebx) `000` (eax) |
| `0x004015A2` | `je .match_found` | `74 24` | `74` = Short conditional jump (JE/JZ)<br>`24` = 8-bit signed relative displacement (+36 bytes) |
| `0x004015A4` | `jl .is_too_low` | `7C 0E` | `7C` = Short jump if less (JL/JNGE)<br>`0E` = 8-bit signed relative displacement (+14 bytes) |
| `0x004015A6` | `jg .is_too_high` | `7F 16` | `7F` = Short jump if greater (JG/JNLE)<br>`16` = 8-bit signed relative displacement (+22 bytes) |

---

## 2. Microarchitectural State During Execution

### A. Hardware Register File
At the silicon level within an x86-64 core (e.g., Intel Raptor Lake or AMD Zen 4), general-purpose registers (`RAX`, `RBX`, `RCX`, `RDX`, `RSI`, `RDI`, `R8-R15`) are physically mapped to a multi-ported SRAM Register File via Register Renaming (RAT - Register Alias Table):

* **`EBX` (Secret Number):**
  * Hex: `0x0000002A` (decimal 42)
  * Binary: `0000 0000 0000 0000 0000 0000 0010 1010`
* **`EAX` (User Guess - e.g. 50):**
  * Hex: `0x00000032` (decimal 50)
  * Binary: `0000 0000 0000 0000 0000 0011 0010`

### B. ALU Operation and the `RFLAGS` / `EFLAGS` Register
When `cmp eax, ebx` executes, the Arithmetic Logic Unit (ALU) computes:
$$\text{Result} = \text{EAX} - \text{EBX} = 50 - 42 = +8$$

The result `$8$` is **discarded** (not written back to `EAX`), but the status flags in the CPU's `EFLAGS` register are updated:

* **ZF (Zero Flag, bit 6):** Set to `0` because $50 - 42 \neq 0$.
* **SF (Sign Flag, bit 7):** Set to `0` because the MSB of the result is $0$ (positive).
* **OF (Overflow Flag, bit 11):** Set to `0` because no signed arithmetic wrap occurred.
* **CF (Carry Flag, bit 0):** Set to `0` because no unsigned borrow was needed.

### C. Branch Evaluation & Hardware Jump Logic
The conditional jump instructions read the condition codes in `EFLAGS`:

* `je`: Evaluates `ZF == 1`. Since `ZF = 0`, the jump is **not taken**.
* `jl`: Evaluates `SF != OF`. Since `SF = 0` and `OF = 0` ($0 \neq 0$ is False), the jump is **not taken**.
* `jg`: Evaluates `ZF == 0 && SF == OF`. Since `ZF = 0` and `0 == 0` (True), the condition passes!
  * The hardware Branch Predictor / Instruction Pointer (`RIP`) adds `+0x16` to `RIP`:
    $$\text{Target RIP} = \text{Next RIP} + \text{Displacement} = 0x004015A8 + 0x16 = 0x004015BE$$
  * Instruction Fetch (IF) jumps directly to `.is_too_high`.

---

## 3. High-Level Comparison: CLR / JVM Bytecode vs. Machine Opcode

In Tier 1 (C#), the C# compiler (`csc`) emits Common Intermediate Language (CIL/MSIL) bytecode:

```cil
ldloc.0          // Push guess onto virtual evaluation stack
ldloc.1          // Push secret onto virtual evaluation stack
bgt.s IL_0028    // Branch to target if greater (opcode: 2D 0F)
blt.s IL_0036    // Branch to target if less than (opcode: 2C 12)
beq.s IL_0044    // Branch if equal (opcode: 2E 18)
```

At runtime, the .NET JIT (Just-In-Time) compiler compiles those virtual stack operations into native x86-64 machine instructions (`39 D8`, `74 24`, etc.), replacing stack simulation with direct CPU register manipulation.

