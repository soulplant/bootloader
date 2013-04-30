[ORG 0x7c00]
start:
    mov si, msg
    call print
    call read_sector
    jmp hang

print:
	pusha
print_loop:
    lodsb
    cmp al, 0
    je print_done

    mov ah, 0Eh
    mov bx, 7
    int 10h

    jmp print_loop
print_done:
	popa
    ret

read_sector:
    mov ah, 2h      ; read sectors from drive
    mov al, 1       ; sectors to read count
    mov ch, 0       ; track
    mov cl, 2		; sector (1-based)
    mov dh, 0		; head
    mov dl, 0       ; drive (0 = floppy)
    mov bx, 0x9000  ; ES:BX dest
    int 13h
    jc error
	mov eax, [0x9000]
    mov si, success_string
    call print
    ret

hang:
    jmp hang

error:
    mov si, error_string
    call print
    jmp error

msg db 'Hello, World!',0xd,0xa,'Who needs a kernel?',0xd,0xa,0
error_string db 'error',0xd,0xa,0
success_string db 'success',0xd,0xa,0

times 510-($-$$) db 0
dw 0xAA55
times 1024-($-$$) dw 0x1234
