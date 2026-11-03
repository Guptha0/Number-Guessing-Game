; ==============================================================================
; Tier 6A: Bare-Metal x86 BIOS Boot Sector (Ring 0 / Standalone Operating System)
; Direct CPU Execution at RAM 0x7C00 - Zero OS, Zero C Runtime, Pure Silicon Access
; Size: Exactly 512 bytes ending with MBR boot signature 0xAA55
; ==============================================================================

[BITS 16]
[ORG 0x7C00]

start:
    ; 1. Initialize CPU Segment Registers & Stack Pointer
    cli                         ; Disable CPU hardware interrupts during setup
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00              ; Stack grows downward from bootloader origin
    sti                         ; Re-enable hardware interrupts

    ; 2. Clear Screen via BIOS Video Service (INT 0x10, AH=0x06)
    mov ah, 0x06
    xor al, al                  ; Clear entire screen
    xor cx, cx                  ; Upper left corner (0,0)
    mov dx, 0x184F              ; Lower right corner (24, 79)
    mov bh, 0x0F                ; White on black attribute
    int 0x10

    ; 3. Acquire Hardware Entropy via CMOS RTC Clock (INT 0x1A)
    mov ah, 0x00                ; Read system clock tick counter
    int 0x1A                    ; DX contains low 16 bits of hardware timer ticks
    mov ax, dx
    xor dx, dx
    mov cx, 100
    div cx                      ; DX = remainder (0 - 99)
    inc dl                      ; Target = 1 - 100
    mov [secret_target], dl

    ; Print Banner
    mov si, msg_banner
    call print_string

    mov byte [attempts_left], 7

game_loop:
    cmp byte [attempts_left], 0
    je game_over_defeat

    ; Display Prompt
    mov si, msg_prompt
    call print_string

    ; Read 2-digit decimal input from physical keyboard (INT 0x16)
    call read_two_digit_input   ; Returns parsed integer in AL
    mov bl, al                  ; BL = User guess

    dec byte [attempts_left]

    ; --------------------------------------------------------------------------
    ; Direct Hardware ALU Comparison
    ; --------------------------------------------------------------------------
    mov al, [secret_target]
    cmp bl, al
    je game_over_win
    jl hint_higher
    jg hint_lower

hint_higher:
    mov si, msg_higher
    call print_string
    jmp game_loop

hint_lower:
    mov si, msg_lower
    call print_string
    jmp game_loop

game_over_win:
    mov si, msg_win
    call print_string
    jmp halt_cpu

game_over_defeat:
    mov si, msg_defeat
    call print_string
    mov al, [secret_target]
    call print_number
    mov si, msg_crlf
    call print_string

halt_cpu:
    mov si, msg_reboot
    call print_string
hang:
    hlt                         ; Suspend CPU until next hardware interrupt
    jmp hang

; ==============================================================================
; Hardware I/O Routines via BIOS Interrupts
; ==============================================================================

; Print null-terminated string pointed to by SI
print_string:
    pusha
.next_char:
    lodsb                       ; Load byte from DS:[SI] into AL
    cmp al, 0
    je .done
    mov ah, 0x0E                ; Teletype output function
    mov bh, 0x00                ; Page 0
    mov bl, 0x0B                ; Cyan foreground color
    int 0x10                    ; BIOS Video Interrupt
    jmp .next_char
.done:
    popa
    ret

; Read two decimal keystrokes and convert ASCII to 8-bit integer in AL
read_two_digit_input:
    ; Read Tens Digit
    mov ah, 0x00
    int 0x16                    ; Read keystroke into AL
    mov ah, 0x0E
    int 0x10                    ; Echo char to display
    sub al, '0'
    mov cl, 10
    mul cl                      ; AL = tens * 10
    mov ch, al

    ; Read Units Digit
    mov ah, 0x00
    int 0x16
    mov ah, 0x0E
    int 0x10
    sub al, '0'
    add al, ch                  ; AL = (tens * 10) + units

    push ax
    mov si, msg_crlf
    call print_string
    pop ax
    ret

; Print 8-bit integer in AL as decimal string
print_number:
    pusha
    xor ah, ah
    mov cl, 10
    div cl                      ; AL = quotient (tens), AH = remainder (units)
    add al, '0'
    mov dl, ah
    mov ah, 0x0E
    int 0x10
    mov al, dl
    add al, '0'
    mov ah, 0x0E
    int 0x10
    popa
    ret

; ==============================================================================
; Data Segment
; ==============================================================================
secret_target   db 0
attempts_left   db 0

msg_banner      db "=== [TIER 6: BARE-METAL x86 BIOS BOOT OS] ===", 13, 10, 0
msg_prompt      db "Enter 2-digit guess (e.g. 42): ", 0
msg_higher      db ">> HIGHER! [ALU CMP: Guess < Target]", 13, 10, 0
msg_lower       db ">> LOWER!  [ALU CMP: Guess > Target]", 13, 10, 0
msg_win         db 13, 10, ">> TARGET MATCHED! CPU HALTED.", 13, 10, 0
msg_defeat      db 13, 10, ">> ATTEMPTS EXHAUSTED! Secret was: ", 0
msg_reboot      db "System halted. Press Reset to reboot.", 13, 10, 0
msg_crlf        db 13, 10, 0

; ==============================================================================
; Boot Sector Padding & Signature
; ==============================================================================
times 510 - ($ - $$) db 0       ; Pad remaining bytes up to 510 with zeros
dw 0xAA55                       ; Magic BIOS MBR boot signature (Bytes 511 & 512)
