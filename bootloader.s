[map all labels.map]

Stage2Area equ 0x9000

[ORG 0x7c00]
start:
    mov si, msg
    call print
    call read_sector
    cli
    lgdt [GDT_toc]
    sti

    jmp Stage2Area
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
    mov cl, 2       ; sector (1-based)
    mov dh, 0       ; head
    mov dl, 0       ; drive (0 = floppy)
    mov bx, Stage2Area  ; ES:BX dest
    int 13h
    jc error
    mov eax, [Stage2Area]
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

GDT_start:
  ; null selector (reserved by processor)
  dd 0
  dd 0

  ; code selector
  dw 0xffff     ; Limit 0-15
  dw 0x0        ; Base 0-15
  db 0x0        ; Base 16-23
  db 10011010b  ; See http://www.brokenthorn.com/Resources/OSDev8.html
  db 11001111b  ; As above.
  dw 0x0        ; Base 24-31

  ; data selector - as above but with the code/data bit set to 0
  dw 0xffff
  dw 0x0
  db 0x0
  db 10010010b  ; bit 3 here is 0, not 1 as above
  db 11001111b
  dw 0x0
GDT_end:

GDT_toc:
  dw GDT_end - GDT_start - 1    ; size of GDT - 1
  dd GDT_start


times 510-($-$$) db 0
dw 0xAA55

stage2:
  mov eax, 0x1234

  cli
  mov eax, cr0
  or eax, 1
  mov cr0, eax
protected:
  jmp 0x8:0x9018    ; this is stage3 atm, we have to code it explicitly because this file is ORG 0x7c00.

bits 32
stage3:
  sti
  mov ax, 0x10
  mov ds, ax  ; causes reboot
  mov ss, ax
  mov es, ax
  ; sti

stage3_hang:
  jmp 0x8:stage3_hang

times 1024-($-$$) dw 0x1234
