[GLOBAL isr0]
[GLOBAL isr1]
[GLOBAL isr2]
[GLOBAL isr3]
[GLOBAL isr4]
[GLOBAL isr5]
[GLOBAL isr6]
[GLOBAL isr7]
[GLOBAL isr8]
[GLOBAL isr9]
[GLOBAL isr10]
[GLOBAL isr11]
[GLOBAL isr12]
[GLOBAL isr13]
[GLOBAL isr14]
[GLOBAL isr15]
[GLOBAL isr16]
[GLOBAL isr17]
[GLOBAL isr18]
[GLOBAL isr19]
[GLOBAL isr20]
[GLOBAL isr21]
[GLOBAL isr22]
[GLOBAL isr23]
[GLOBAL isr24]
[GLOBAL isr25]
[GLOBAL isr26]
[GLOBAL isr27]
[GLOBAL isr28]
[GLOBAL isr29]
[GLOBAL isr30]
[GLOBAL isr31]


;Common ISR stub. Saves processor state, sets up kernel code segments, 
;calls C-level fault handler, and restores stack fram
;when processor recieves interrupt, it saves the contents of essential register:
;instruction pointer, stack pointer, code and data segment, flag register to the stack.
;then, finds interrupt handler location for IDT and jumps to it

extern isr_handler
isr_common_stub:
    pusha                       ;pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

    mov ax, ds                  ;lower 16 bits of data segemnt into eax
    push eax                    ;saves data segment descriptor

    mov ax, 0x10                ;load kernel data descriptor 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop eax                     ;reloads original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                        ;returns edi, esi, etc
    add esp, 8                  ;cleans up the pushed error code
    sti
    iret

extern irq_handler
;Save processor state, sets up kernel mode segments, call C-level fault handler, 
;and restores stack frames
irq_common_stub:
    pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds               ; Lower 16-bits of eax = ds.
    push eax                 ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call irq_handler

    pop ebx        ; reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                     ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    sti
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

;********************************************************************************;
;                                 Interrupts                                     ;
;********************************************************************************;
isr0:
    cli                  ;disable interrupt 
    push byte 0          ;Push a dummy error code (if isr0 doesnt push its own error code)
    push byte 0          ;push the interrupt number(0)
    jmp isr_common_stub  ;Go to our common handler

isr1: 
    cli 
    push byte 0
    push byte 1
    jmp isr_common_stub
isr2: 
    cli 
    push byte 0
    push byte 2
    jmp isr_common_stub
isr3: 
    cli 
    push byte 0
    push byte 3
    jmp isr_common_stub
isr4: 
    cli 
    push byte 0
    push byte 4
    jmp isr_common_stub
isr5: 
    cli 
    push byte 0
    push byte 5
    jmp isr_common_stub
isr6: 
    cli 
    push byte 0
    push byte 6
    jmp isr_common_stub
isr7: 
    cli 
    push byte 0
    push byte 7
    jmp isr_common_stub
isr8: 
    cli 
    push byte 0
    push byte 8
    jmp isr_common_stub
isr9: 
    cli 
    push byte 0
    push byte 9
    jmp isr_common_stub
isr10: 
    cli 
    push byte 0
    push byte 10
    jmp isr_common_stub
isr11: 
    cli 
    push byte 0
    push byte 11
    jmp isr_common_stub
isr12: 
    cli 
    push byte 0
    push byte 12
    jmp isr_common_stub
isr13: 
    cli 
    push byte 0
    push byte 13
    jmp isr_common_stub
isr14: 
    cli 
    push byte 0
    push byte 14
    jmp isr_common_stub
isr15: 
    cli 
    push byte 0
    push byte 15
    jmp isr_common_stub
isr16: 
    cli 
    push byte 0
    push byte 16
    jmp isr_common_stub
isr17: 
    cli 
    push byte 0
    push byte 17
    jmp isr_common_stub
isr18: 
    cli 
    push byte 0
    push byte 18
    jmp isr_common_stub
isr19: 
    cli 
    push byte 0
    push byte 19
    jmp isr_common_stub
isr20: 
    cli 
    push byte 0
    push byte 20
    jmp isr_common_stub
isr21: 
    cli 
    push byte 0
    push byte 21
    jmp isr_common_stub
isr22: 
    cli 
    push byte 0
    push byte 22
    jmp isr_common_stub
isr23: 
    cli 
    push byte 0
    push byte 24
    jmp isr_common_stub
isr24: 
    cli 
    push byte 0
    push byte 24
    jmp isr_common_stub
isr25: 
    cli 
    push byte 0
    push byte 25
    jmp isr_common_stub
isr26: 
    cli 
    push byte 0
    push byte 26
    jmp isr_common_stub
isr27: 
    cli 
    push byte 0
    push byte 27
    jmp isr_common_stub
isr28: 
    cli 
    push byte 0
    push byte 28
    jmp isr_common_stub
isr29: 
    cli 
    push byte 0
    push byte 29
    jmp isr_common_stub
isr30: 
    cli 
    push byte 0
    push byte 30
    jmp isr_common_stub
isr31: 
    cli 
    push byte 0
    push byte 31
    jmp isr_common_stub


; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47
