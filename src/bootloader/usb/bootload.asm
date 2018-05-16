; ==================================================================
; The Mike Operating System bootloader
; Copyright (C) 2006 - 2014 MikeOS Developers -- see doc/LICENSE.TXT
;
; Based on a free boot loader by E Dehling. It scans the FAT12
; floppy for KERNEL.BIN (the MikeOS kernel), loads it and executes it.
; This must grow no larger than 512 bytes (one sector), with the final
; two bytes being the boot signature (AA55h). Note that in FAT12,
; a cluster is the same as a sector: 512 bytes.
; ==================================================================


	BITS 16

	jmp short bootloader_start	; Jump past disk description section
	nop				; Pad out before disk description


; ------------------------------------------------------------------
; Disk description table, to make it a valid floppy
; Note: some of these values are hard-coded in the source!
; Values are those used by IBM for 1.44 MB, 3.5" diskette

OEMLabel		db "MIKEBOOT"	; Disk label
BytesPerSector		dw 512		; Bytes per sector
SectorsPerCluster	db 1		; Sectors per cluster
ReservedForBoot		dw 1		; Reserved sectors for boot record
NumberOfFats		db 2		; Number of copies of the FAT
RootDirEntries		dw 224		; Number of entries in root dir
					; (224 * 32 = 7168 = 14 sectors to read)
LogicalSectors		dw 2880		; Number of logical sectors
MediumByte		db 0F0h		; Medium descriptor byte
SectorsPerFat		dw 9		; Sectors per FAT
SectorsPerTrack		dw 18		; Sectors per track (36/cylinder)
Sides			dw 2		; Number of sides/heads
HiddenSectors		dd 0		; Number of hidden sectors
LargeSectors		dd 0		; Number of LBA sectors
DriveNo			dw 0		; Drive No: 0
Signature		db 41		; Drive signature: 41 for floppy
VolumeID		dd 00000000h	; Volume ID: any number
VolumeLabel		db "MIKEOS     "; Volume Label: any 11 chars
FileSystem		db "FAT12   "	; File system type: don't change!


; ------------------------------------------------------------------
; Main bootloader code

bootloader_start:
	mov ax, 07C0h			; Set up 4K of stack space above buffer
	add ax, 544			; 8k buffer = 512 paragraphs + 32 paragraphs (loader)
	cli				; Disable interrupts while changing stack
	mov ss, ax
	mov sp, 4096
	sti				; Restore interrupts

	mov ax, 07C0h			; Set data segment to where we're loaded
	mov ds, ax

	; NOTE: A few early BIOSes are reported to improperly set DL

	mov [bootdev], dl		; Save boot device number
	mov si,hello
	call print_string
	cli
	hlt
hello db "Hello world!",0x00
	
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
times 510-($-$$) db 0	; Pad remainder of boot sector with zeros
dw 0AA55h		; Boot signature (DO NOT CHANGE!)

