BITS 16

os_main:
	cli				; Clear interrupts
	mov ax, 0
	mov ss, ax			; Set stack segment and pointer
	mov sp, 0FFFFh
	sti				; Restore interrupts

	cld				; The default direction for string operations
					; will be 'up' - incrementing address in RAM

	mov ax, 0x2000			; Set all segments to match where kernel is loaded
	mov ds, ax			; After this, we don't need to bother with
	mov es, ax			; segments ever again, as MikeOS and its programs
	mov fs, ax			; live entirely in 64K
	mov gs, ax

	mov [bootdev], dl		; Save boot device number

	mov si,welcomestring
	call print_string
	
	;
	; drive goedkeuren
	;
	mov ah,0x00
	mov dl,byte [bootdev]
	int 0x13
	jnc nxt1
	mov si,msgx1
	call print_string
	cli
	hlt
	msgx1 db "RESET FOUT",0x00
	nxt1:
	;
	; Kijken of extensions er zijn
	;
	mov ah,0x41
	mov dl,byte [bootdev]
	mov bx,55AAh
	int 0x13
	jnc nxt2
	mov si,msgx2
	call print_string
	cli
	hlt
	msgx2 db "EXT FOUT",0x00
	nxt2:
	;
	; SECTOR LEZEN
	;
	mov si,datapacket
	mov ah,0x42
	mov dl,byte [bootdev]
	int 0x13
	jnc nxt3
	mov si,msgx3
	call print_string
	cli
	hlt
	msgx3 db "LEES FOUT",0x00
	nxt3:
	
	;
	; INTRODUCTIE
	;
	mov si,tell1
	call print_string
	mov si,filename
	call print_string
	mov si,tell2
	call print_string
	mov si,0x5000
	call print_string
	jmp $

fail:
mov si,failure
call print_string
jmp $

failure db "BOOT FAILURE",0x00
welcomestring db "Stage2                                              Sanderslando bootloader v1.0",0x00
tell1 db "Poging doen om ",0x00
filename db "KERNEL   BIN",0x00
tell2 db " te vinden en te laden!",0x00
bootdev db 0x00


print_string:				; Output string in SI to screen
	pusha

	mov ah, 0Eh			; int 10h teletype function

.repeat:
	lodsb				; Get char from string
	cmp al, 0
	je .done			; If char is zero, end of string
	int 10h				; Otherwise, print it
	jmp short .repeat

.done:
	popa
	ret
	
datapacket:
db 0x10
db 0
blkcnt dw 1
addrx dw 0x5000
dw 0
lba1 dd 19
lba2 dd 0
