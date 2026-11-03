; ==============================================================================
; Tier 4: x86-64 Assembly (NASM Syntax with C Runtime Linkage)
; Demonstrating Register Allocation, Stack Framing, CMP, and Conditional Branching
; ==============================================================================

global main
extern printf
extern scanf
extern exit

section .data
    prompt_banner   db "==================================================", 10
                    db "  Tier 4 (x86-64 Assembly Architecture)           ", 10
                    db "==================================================", 10, 0
    prompt_guess    db "Enter your guess (1-100): ", 0
    fmt_input       db "%d", 0
    
    msg_higher      db ">> HIGHER! Attempts left: %d", 10, 0
    msg_lower       db ">> LOWER!  Attempts left: %d", 10, 0
    msg_correct     db ">> BINGO! You found the secret number in %d attempts!", 10, 0
    msg_gameover    db ">> OUT OF ATTEMPTS! Target was: %d", 10, 0

    ; Simulation setup: secret number pre-loaded or set via register
    SECRET_TARGET   equ 42
    MAX_ATTEMPTS    equ 7

section .bss
    user_input      resd 1          ; 4-byte buffer for scanf

section .text
main:
    ; Standard x86-64 stack frame setup (16-byte alignment)
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32                 ; Shadow space and alignment

    ; Print intro banner
    lea     rdi, [rel prompt_banner]
    xor     eax, eax
    call    printf

    ; --------------------------------------------------------------------------
    ; Register Allocation Map:
    ;   EBX = Secret Number (42)
    ;   R12D = Attempts Counter (starts at 0)
    ;   R13D = Max Attempts (7)
    ; --------------------------------------------------------------------------
    mov     ebx, SECRET_TARGET      ; Target value loaded into hardware register EBX
    xor     r12d, r12d              ; R12D = 0 (attempts counter)
    mov     r13d, MAX_ATTEMPTS      ; R13D = 7 (maximum allowed attempts)

.game_loop:
    ; Check if attempts exhausted: CMP R12D, R13D
    cmp     r12d, r13d
    jge     .out_of_attempts        ; Branch if attempts >= max

    ; Display prompt
    lea     rdi, [rel prompt_guess]
    xor     eax, eax
    call    printf

    ; Read user integer input into [user_input] via scanf
    lea     rdi, [rel fmt_input]
    lea     rsi, [rel user_input]
    xor     eax, eax
    call    scanf

    ; If scanf returned <= 0 or EOF, exit gracefully
    cmp     eax, 1
    jne     .exit_program

    ; Increment attempt counter: R12D = R12D + 1
    inc     r12d

    ; Fetch user guess from memory into general-purpose register EAX
    mov     eax, [rel user_input]

    ; --------------------------------------------------------------------------
    ; Core Branching Logic:
    ; CMP performs internal subtraction: (EAX - EBX) and sets CPU EFLAGS:
    ;   - Zero Flag (ZF): 1 if EAX == EBX
    ;   - Sign Flag (SF) != Overflow Flag (OF): indicates signed less than
    ; --------------------------------------------------------------------------
    cmp     eax, ebx
    je      .match_found            ; ZF = 1 -> Guess == Target
    jl      .is_too_low             ; SF != OF -> Guess < Target
    jg      .is_too_high            ; ZF = 0 and SF == OF -> Guess > Target

.is_too_low:
    ; Compute remaining attempts: R13D - R12D
    mov     esi, r13d
    sub     esi, r12d
    lea     rdi, [rel msg_higher]
    xor     eax, eax
    call    printf
    jmp     .game_loop              ; Unconditional jump back to loop head

.is_too_high:
    mov     esi, r13d
    sub     esi, r12d
    lea     rdi, [rel msg_lower]
    xor     eax, eax
    call    printf
    jmp     .game_loop              ; Unconditional jump back to loop head

.match_found:
    lea     rdi, [rel msg_correct]
    mov     esi, r12d               ; Pass attempts used as second printf arg
    xor     eax, eax
    call    printf
    jmp     .exit_program

.out_of_attempts:
    lea     rdi, [rel msg_gameover]
    mov     esi, ebx                ; Pass secret number as second printf arg
    xor     eax, eax
    call    printf

.exit_program:
    ; Epilogue: restore stack frame and return 0
    mov     eax, 0
    mov     rsp, rbp
    pop     rbp
    ret
