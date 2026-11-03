<div align="center">

  # 🎮 QUANTUM NUMBER GUESSER
  ### *A Multi-Tier Systems Architecture & Abstraction Odyssey*

  [![Auspify Internship](https://img.shields.io/badge/Auspify_Technologies-Task_2_Showcase-FF6B00?style=for-the-badge&logo=codewars&logoColor=white)](https://github.com/Guptha0)
  [![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen?style=for-the-badge&logo=githubactions&logoColor=white)](#)
  [![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)](#license)
  
  <p align="center">
    <img src="https://img.shields.io/badge/C%23_.NET_6-512BD4?style=flat-square&logo=dotnet&logoColor=white" alt="C#">
    <img src="https://img.shields.io/badge/Modern_C%2B%2B17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white" alt="C++">
    <img src="https://img.shields.io/badge/Standard_C99-A8B9CC?style=flat-square&logo=c&logoColor=black" alt="C">
    <img src="https://img.shields.io/badge/x86--64_Assembly-EA4335?style=flat-square&logo=assemblyscript&logoColor=white" alt="x86_64">
    <img src="https://img.shields.io/badge/Verilog_RTL-009688?style=flat-square&logo=microchip&logoColor=white" alt="Verilog">
  </p>

  <br />

  ```
   ██████╗ ██╗   ██╗ █████╗ ███╗   ██╗████████╗██╗   ██╗███╗   ███╗
  ██╔═══██╗██║   ██║██╔══██╗████╗  ██║╚══██╔══╝██║   ██║████╗ ████║
  ██║   ██║██║   ██║███████║██╔██╗ ██║   ██║   ██║   ██║██╔████╔██║
  ██║▄▄ ██║██║   ██║██╔══██║██║╚██╗██║   ██║   ██║   ██║██║╚██╔╝██║
  ╚██████╔╝╚██████╔╝██║  ██║██║ ╚████║   ██║   ╚██████╔╝██║ ╚═╝ ██║
   ╚══▀▀═╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝   ╚═╝    ╚═════╝ ╚═╝     ╚═╝
  ```

  > **Task 2 Technical Portfolio Submission** for the **Auspify Technologies C/C++ Software Engineering Internship**.  
  > Demonstrating how high-level software semantics translate down the computing stack into raw register manipulation, machine opcodes, and silicon hardware execution.

  <br />

  <!-- Demo Banner / GIF Placeholder -->
  <p align="center">
    <img src="https://via.placeholder.com/900x420/0D1117/58A6FF?text=QUANTUM+GUESSER+GUI+%2F%2F+HARDWARE-ACCELERATED+CYBERPUNK+DASHBOARD" alt="Quantum Guesser Demo Banner" width="850">
  </p>

</div>

---

## 📌 Executive Summary

Most introductory programming projects treat the Number Guessing Game as a trivial `while`-loop script. **Quantum Number Guesser** reimagines this fundamental computer science problem by constructing it across **6 distinct layers of computational abstraction**.

From **C# WPF hardware-composited desktop applications** down to **16-bit bare-metal BIOS bootloaders** and **Verilog digital logic circuits**, this repository serves as an educational benchmark for studying:
1. Virtual Machine Evaluation Stacks & Just-In-Time (JIT) compilation (.NET CoreCLR).
2. Resource Acquisition Is Initialization (RAII) and Stream State Machines (C++17).
3. Explicit memory buffer sanitation & Linear Congruential PRNGs (ANSI C99).
4. Direct CPU general-purpose register allocation and `EFLAGS` status branching (x86-64 NASM/GAS).
5. Hexadecimal Instruction Opcode decoding & Execution Pipelines (Disassembly).
6. Direct-to-Silicon execution with zero host operating system (Ring 0 & FPGA RTL).

---

## 🏛️ The Multi-Tier Architecture Matrix

| Layer | Environment / Abstraction | Memory & State Model | I/O Subsystem | Branching & Control Flow |
| :--- | :--- | :--- | :--- | :--- |
| **Tier 1: C#** | .NET 6 Managed Runtime / WPF | Managed Heap, Generational GC | Direct3D / Retained Visual Tree | MSIL Bytecode (`bgt.s`, `blt.s`) |
| **Tier 2: C++** | Native AOT Systems Binary | Stack / RAII Scope Lifetimes | `std::iostream` (`failbit` recovery) | Static Branch Prediction (`if`/`else`) |
| **Tier 3: C** | Procedural C-Runtime (CRT) | Contiguous activation frames | `printf` / `scanf` manual stream drain | Direct assembly jump emission |
| **Tier 4: ASM** | x86-64 Instruction Set (ISA) | General Purpose Registers (`EAX`, `EBX`) | System V AMD64 ABI Syscall Wrappers | `cmp` ALU subtraction + `je`/`jl`/`jg` |
| **Tier 5: Machine** | Raw Microarchitectural Opcodes | CPU Register File (SRAM Flip-Flops) | Hardware interrupt vectors | Binary Opcode Bytes (`39 D8`, `74 24`) |
| **Tier 6A: Metal**| 16-bit Real Mode MBR (Ring 0) | BIOS physical RAM origin (`0x7C00`) | Hardware BIOS interrupts (`INT 0x10/0x16`) | Raw CPU Instruction Pointer (`IP`) |
| **Tier 6B: Silicon**| Verilog-2001 RTL Hardware Model | D-Flip-Flop Hardware Registers | Physical DIP switches & LEDs | Finite State Machine (FSM) state register |

---

## 🚀 Advanced Features & Mechanics

### 🌡️ Dynamic Proximity Heat Radar
The game computes a real-time mathematical proximity metric $P \in [0.0, 1.0]$:
$$P = 1.0 - \min\left(1.0, \frac{|G - T|}{\max(T - \text{min}, \text{max} - T)}\right)$$
- `[0.00 - 0.25]` 🧊 **Freezing Cyan** (`#00D2FF`) — Distant guess.
- `[0.26 - 0.50]` 🌱 **Neon Mint** (`#00FFA3`) — Closing in.
- `[0.51 - 0.75]` ⚡ **Solar Amber** (`#FFB800`) — Warm!
- `[0.76 - 0.99]` 🔥 **Supernova Red** (`#FF2A54`) — Scorching hot!
- `[    1.00   ]` ✨ **Radiant Gold** (`#FFFFFF`) — Direct target acquisition!

### ⏱️ Triple Game Modes
- 🎯 **Classic:** 7 calibrated attempts to deduce a secret between $1$ and $100$.
- ⚡ **Time Attack:** Sub-millisecond countdown timer running via `std::chrono` / `DispatcherTimer`. Guess before the chronometer reaches zero!
- 🧬 **Endurance:** Dynamic difficulty scaling. Each consecutive victory expands the target range exponentially:
  $$\text{Range}_{n+1} = \left[1, \; 100 \times 1.5^n\right]$$

### 💥 Physical Trauma Screen-Shake
Uses the industry-standard **Trauma-Based Quadratic Decay Model** (Squirrel Eiserloh):
$$\text{Offset}(x, y) = \left( A \cdot \text{Trauma}^2 \cdot \sin(\omega_1 t), \; A \cdot \text{Trauma}^2 \cdot \cos(\omega_2 t) \right)$$
Incorrect guesses and defeats apply directional trauma to the viewport, which smoothly decays over time.

### 🎨 Modern Cyberpunk Visuals & Audio FX
- **Glassmorphism UI:** Built with dark-mode acrylic aesthetics, custom drop-shadow cards, and real-time linear gradient progress visualizers.
- **Particle Storm:** Procedurally animates 60+ physics-based confetti particles across the viewport on victory.

---

## 📂 Repository Directory Layout

```
📦 Number-Guessing-Game
├── 📂 1_csharp            # Tier 1: C# OOP Console Application (.NET 6)
│   ├── 📄 GameManager.cs  # Encapsulated game logic & attempt tracking
│   └── 📄 Program.cs      # Console driver & input validation
├── 📂 2_cpp               # Tier 2: Modern C++ Systems Application
│   └── 📄 main.cpp        # std::mt19937 PRNG & std::cin.fail() stream safety
├── 📂 3_c                 # Tier 3: Procedural ANSI C Implementation
│   └── 📄 main.c          # Manual stdin buffer flushing & CRT rand()
├── 📂 4_asm               # Tier 4: x86-64 Assembly Architecture
│   ├── 📄 game.asm        # NASM syntax with libc linkage
│   └── 📄 game_gas.s      # GNU as / GCC Intel syntax
├── 📂 5_machine           # Tier 5: Machine-Level Opcode Disassembly
│   └── 📄 breakdown.md    # Detailed hex byte & ALU flags technical writeup
├── 📂 6_baremetal         # Tier 6A: 512-byte BIOS Bootloader OS (Ring 0)
│   └── 📄 boot.asm        # Pure 16-bit real-mode OS (No Windows / Linux needed)
├── 📂 6_verilog           # Tier 6B: Hardware Synthesizable Circuit
│   └── 📄 guessing_game.v # 8-bit LFSR PRNG, digital comparator & 7-seg decoder
├── 📂 gui_csharp          # Tier 1 GUI: Modern Cyberpunk WPF Desktop Application
│   ├── 📄 MainWindow.xaml # Glassmorphism XAML layout & animations
│   ├── 📄 MainWindow.xaml.cs # Proximity math, confetti & trauma engine
│   └── 📄 QuantumGuesser.csproj # .NET 6 WPF project definition
├── 📂 gui_cpp             # Tier 2 GUI: High-Performance Dear ImGui Architecture
│   └── 📄 imgui_game.cpp  # 144 FPS Immediate-Mode loop & hardware timers
└── 📂 gui_c_raylib        # Tier 3 GUI: Retro Arcade Pixel Engine
    └── 📄 arcade_game.c   # 8 KB pre-allocated particle memory arena
```

---

## 🛠️ Compilation & Execution Guide

### 1. C# Cyberpunk GUI (Native Windows WPF)
```powershell
cd "gui_csharp"
dotnet run
```
*Alternatively, run the compiled binary: `gui_csharp\bin\Debug\net6.0-windows\QuantumGuesser.exe`.*

---

### 2. Modern C++ Systems Layer (WSL / Linux / MinGW)
```bash
cd "2_cpp"
g++ -std=c++17 -O2 main.cpp -o guessing_cpp
./guessing_cpp
```

---

### 3. Procedural C with Manual Buffer Hygiene
```bash
cd "3_c"
gcc -Wall -Wextra -O2 main.c -o guessing_c
./guessing_c
```

---

### 4. x86-64 Assembly (Direct Hardware Registers)
```bash
cd "4_asm"
# Using GCC's integrated assembler:
gcc -no-pie game_gas.s -o guessing_asm
./guessing_asm

# Or via NASM:
nasm -f elf64 game.asm -o game.o && gcc -no-pie game.o -o guessing_asm
```

---

### 5. Inspecting the Raw Machine Code (Hex Opcodes)
Inspect how your CPU translates assembly mnemonics into raw binary opcode bytes:
```bash
cd "4_asm"
objdump -d -M intel guessing_asm | grep -A 15 "<main>:"
```
**Output Breakdown:**
```text
  40114c:   bb 2a 00 00 00          mov    ebx, 0x2a      <-- Load target (42) into EBX
  401151:   45 31 e4                xor    r12d, r12d     <-- Attempts = 0
  401154:   41 bd 07 00 00 00       mov    r13d, 0x7      <-- Max attempts = 7
  40116e:   39 d8                   cmp    eax, ebx       <-- ALU subtraction (39 D8)
  401170:   74 38                   je     4011aa         <-- Jump if ZF=1 (74 38)
```

---

### 6. Booting the Bare-Metal OS (Tier 6A)
Experience the game running with **no operating system** underneath inside a virtualized x86 CPU:
```bash
sudo apt update && sudo apt install -y nasm qemu-system-x86
cd "6_baremetal"
nasm -f bin boot.asm -o boot.bin
qemu-system-x86_64 -drive format=raw,file=boot.bin
```

---

## 💡 Engineering Insights & Comparative Analysis

> [!NOTE]
> **Stream State Corruption vs. Memory Safety:**  
> In C++, if a user submits alphabetical characters to `std::cin >> val`, the stream's `failbit` triggers, causing infinite loops unless resolved via `cin.clear()` and `cin.ignore()`. In C, `scanf("%d")` leaves unread bytes in the buffer, requiring a manual `while((c=getchar()) != '\n' && c != EOF)` drain loop. In C#, `int.TryParse` encapsulates this via managed string parsing.

> [!TIP]
> **Register Allocations in x86-64:**  
> In Tier 4, the secret number is stored in non-volatile register `EBX`, the user's guess is loaded into `EAX`, attempt counts are maintained in `R12D`, and the attempt ceiling is pinned in `R13D`. The comparison `cmp eax, ebx` executes a non-destructive subtraction that latches flags into `EFLAGS` (`ZF`, `SF`, `OF`), which dictate the behavior of conditional branches `je`, `jl`, and `jg`.

---

## 🏆 Acknowledgements & Internship Credits

This multi-tier engineering project was designed and developed as part of **Task 2 of the Auspify Technologies C/C++ Software Engineering Internship**. 

Special thanks to the technical mentors at **Auspify Technologies** for providing an environment that champions deep low-level mastery, systems architecture, and holistic software design.

---

## 👨‍💻 Author Profile

**Guptha**  
*Systems Architect, Multi-Language Engineer & Intern @ Auspify Technologies*

- 🌐 **GitHub:** [@Guptha0](https://github.com/Guptha0)
- 💼 **Focus:** Systems Programming (C/C++), Assembly (x86-64), Architecture & Performance Engineering

<div align="center">
  <sub>Engineered with precision. Star this repository if you found the architectural breakdown helpful! ⭐</sub>
</div>
