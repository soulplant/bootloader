[ORG 0]
    jmp 07C0h:start

msg db 'Hello, World!',0xd,0xa,'Who needs a kernel?',0

start:
    mov ax, cs
    mov ds, ax
    mov es, ax

    mov si, msg

print:
    lodsb
    cmp al, 0
    je hang

    mov ah, 0Eh
    mov bx, 7
    int 10h

    jmp print

hang:
    jmp hang
times 510-($-$$) db 0
dw 0AA55h
