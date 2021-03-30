[EXTERN isr_handler]
;Common ISR stub. Saves processor state, sets up kernel code segments, 
;calls C-level fault handler, and restores stack fram

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


