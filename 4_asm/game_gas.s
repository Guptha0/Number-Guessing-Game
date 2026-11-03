# ==============================================================================
# Tier 4: x86-64 Assembly (GNU as / GCC Intel Syntax)
# Compiles directly with: gcc -no-pie game_gas.s -o guessing_asm
# ==============================================================================

    .intel_syntax noprefix
    .global main
    .extern printf
    .extern scanf

    .set SECRET_TARGET, 42
    .set MAX_ATTEMPTS, 7

    .section .rodata
prompt_banner:
    .string "==================================================\n  Tier 4 (x86-64 Assembly Architecture)           \n==================================================\n"
prompt_guess:
    .string "Enter your guess (1-100): "
fmt_input:
    .string "%d"
msg_higher:
    .string ">> HIGHER! Attempts left: %d\n"
msg_lower:
    .string ">> LOWER!  Attempts left: %d\n"
msg_correct:
    .string ">> BINGO! You found the secret number in %d attempts!\n"
msg_gameover:
    .string ">> OUT OF ATTEMPTS! Target was: %d\n"

    .section .bss
    .lcomm user_input, 4

    .section .text
main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32

    # Print banner
    lea     rdi, [rip + prompt_banner]
    xor     eax, eax
    call    printf@PLT

    # EBX = Secret (42), R12D = Attempts (0), R13D = Limit (7)
    mov     ebx, SECRET_TARGET
    xor     r12d, r12d
    mov     r13d, MAX_ATTEMPTS

.Lgame_loop:
    cmp     r12d, r13d
    jge     .Lout_of_attempts

    # Prompt
    lea     rdi, [rip + prompt_guess]
    xor     eax, eax
    call    printf@PLT

    # Scan integer
    lea     rdi, [rip + fmt_input]
    lea     rsi, [rip + user_input]
    xor     eax, eax
    call    scanf@PLT

    # If scanf didn't match 1 integer (e.g. EOF or invalid input), exit gracefully
    cmp     eax, 1
    jne     .Lexit_program

    inc     r12d
    mov     eax, DWORD PTR [rip + user_input]

    # ALU Comparison & EFLAGS branching
    cmp     eax, ebx
    je      .Lmatch_found
    jl      .Lis_too_low
    jg      .Lis_too_high

.Lis_too_low:
    mov     esi, r13d
    sub     esi, r12d
    lea     rdi, [rip + msg_higher]
    xor     eax, eax
    call    printf@PLT
    jmp     .Lgame_loop

.Lis_too_high:
    mov     esi, r13d
    sub     esi, r12d
    lea     rdi, [rip + msg_lower]
    xor     eax, eax
    call    printf@PLT
    jmp     .Lgame_loop

.Lmatch_found:
    lea     rdi, [rip + msg_correct]
    mov     esi, r12d
    xor     eax, eax
    call    printf@PLT
    jmp     .Lexit_program

.Lout_of_attempts:
    lea     rdi, [rip + msg_gameover]
    mov     esi, ebx
    xor     eax, eax
    call    printf@PLT

.Lexit_program:
    xor     eax, eax
    mov     rsp, rbp
    pop     rbp
    ret
