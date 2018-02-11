global irq_defaulte
irq_defaulte:
    pusha
    push ds
    push es
    push fs
    push gs
    push eax
    
    mov al,0x20
    out 0x20,al
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret
    
global getcs
getcs:
mov eax,cs
ret
