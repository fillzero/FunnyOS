;nasm -dISFAT12 boot.asm -o boot.bin
;
; File:
;                            boot.asm
; Description:
;                           DOS-C boot
;
;                       Copyright (c) 1997;
;                           Svante Frey
;                       All Rights Reserved
;
; This file is part of DOS-C.
;
; DOS-C is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public License
; as published by the Free Software Foundation; either version
; 2, or (at your option) any later version.
;
; DOS-C is distributed in the hope that it will be useful, but
; WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
; the GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public
; License along with DOS-C; see the file COPYING.  If not,
; write to the Free Software Foundation, 675 Mass Ave,
; Cambridge, MA 02139, USA.
;
;
; $Log: boot.asm,v $
; Revision 1.3  2000/05/25 20:56:19  jimtabor
; Fixed project history
;
; Revision 1.2  2000/05/11 03:56:42  jimtabor
; Clean up and Release
;
; Revision 1.1.1.1  2000/05/06 19:34:53  jhall1
; The FreeDOS Kernel.  A DOS kernel that aims to be 100% compatible with
; MS-DOS.  Distributed under the GNU GPL.
;
; Revision 1.12  1999/09/25 06:42:18  jprice
; Optimize boot loader.  Documentation.
;
; Revision 1.11  1999/09/24 19:04:55  jprice
; Added changes recommended by Jens Horstmeier

; to make their bootable CD work.
;
; Revision 1.10  1999/09/23 04:39:02  jprice
; *** empty log message ***
;
; Revision 1.7  1999/04/23 03:43:46  jprice
; Ported to NASM by ror4
;
; Revision 1.6  1999/04/17 19:14:03  jprice
; Fixed multi-sector code
;
; Revision 1.5  1999/04/17 06:23:26  jprice
; Changed so multi-sector IO is optional.
;
; Revision 1.4  1999/04/13 15:52:22  jprice
; Moves boot sector to top of mem
;
; Revision 1.3  1999/04/06 22:53:36  jprice
; Put back code to read multiple sectors at a time.
;
; Revision 1.2  1999/04/01 07:23:20  jprice
; New boot loader
;
; Revision 1.1.1.1  1999/03/29 15:39:39  jprice
; New version without IPL.SYS
;
; Revision 1.3  1999/03/02 06:57:14  jprice
; Added entry address for more recent versions of TLINK
;
; Revision 1.2  1999/01/21 05:03:58  jprice
; Formating.
;
; Revision 1.1.1.1  1999/01/20 05:51:00  jprice
; Imported sources
;
;
;          Rev 1.5   10 Jan 1997  4:58:06   patv
;       Corrected copyright
;
;          Rev 1.4   10 Jan 1997  4:52:50   patv
;       Re-written to support C drive and eliminate restrictions on IPL.SYS
;
;          Rev 1.3   29 Aug 1996 13:06:50   patv
;       Bug fixes for v0.91b
;
;          Rev 1.2   01 Sep 1995 17:56:44   patv
;       First GPL release.
;
;          Rev 1.1   30 Jul 1995 20:37:38   patv
;       Initialized stack before use.
;
;          Rev 1.0   02 Jul 1995 10:57:52   patv
;       Initial revision.
;

;	+--------+
;	|        |
;	|        |
;	|--------| 4000:0000
;	|        |
;	|  FAT   |
;	|        |
;	|--------| 2000:0000
;	|BOOT SEC|
;	|RELOCATE|
;	|--------| 1FE0:0000
;	|        |
;	|        |
;	|        |
;	|        |
;	|--------|
;	|BOOT SEC|
;	|ORIGIN  | 07C0:0000
;	|--------|
;	|        |
;	|        |
;	|        |
;	|--------|
;	|KERNEL  |
;	|LOADED  |
;	|--------| 0060:0000
;	|        |
;	+--------+


;%define ISFAT12         1
;%define ISFAT16         1
;%define CALCPARAMS      1
;%define MULTI_SEC_READ  1


segment	.text

%define BASE            0x7c00

                org     BASE

Entry:          jmp     short real_start
		nop

;       bp is initialized to 7c00h
%define bsOemName       bp+0x03      ; OEM label
		db	"1234567",0	; name 8 bit
%define bsBytesPerSec   bp+0x0b      ; bytes/sector
		dw	0x0200		; 512 byte per sector
%define bsSecPerClust   bp+0x0d      ; sectors/allocation unit
		db	1		; 1 sector per allocation unit
%define bsResSectors    bp+0x0e      ; # reserved sectors
		dw	1		;
%define bsFATs          bp+0x10      ; # of fats
		db	2		; 2 fats
%define bsRootDirEnts   bp+0x11      ; # of root dir entries
		dw	0xe0		; 0xe0 root-directory entries (0x0e is not OK to reconize by DOS or Linux)
%define bsSectors       bp+0x13      ; # sectors total in image
		dw	0xb40		; 2880 = 18(sectors per track) * 80(track) * 2(side) 
%define bsMedia         bp+0x15      ; media descrip: fd=2side9sec, etc...
		db	0xf0		; 1.44M
%define sectPerFat      bp+0x16      ; # sectors in a fat
		dw	0x9		; 9 sectos in a fat
%define sectPerTrack    bp+0x18      ; # sectors/track
		dw	0x12		; 18 sectors/tarck
%define nHeads          bp+0x1a      ; # heads
		dw	0x2		; 2 headers
%define nHidden         bp+0x1c      ; # hidden sectors
		dd	0x0		; 0
%define nSectorHuge     bp+0x20      ; # sectors if > 65536
		dd	0x0		; 0
%define drive           bp+0x24      ; drive number
		db	0x0		; physical drive number 0
		db	0x0		; reserved byte
%define extBoot         bp+0x26      ; extended boot signature
		db	0x29
%define volid           bp+0x27		;0x27-0x2a 32 bit binary volume ID
		dd	0
%define vollabel        bp+0x2b		;0x2b-0x35 11 byte  Volume label
		db	"myosmyosos!"

%define filesys         bp+0x36		;0x36-0x3d 8 byte Reserved
		db	0x46		;F
		db	0x41		;A
		db	0x54		;T
		db	0x31		;1
		db	0x32		;2
		times	3	db	0x20 ; ---
		
%define LOADSEG         0x0060

%define FATBUF          0x2000          ; offset of temporary buffer for FAT chain

;-----------------------------------------------------------------------

		times	0x3E-$+$$ db 0

%define tempbuf         bp+0x3E
                dw      LOADSEG


%define RootDirSecs     bp+0x40         ; # of sectors root dir uses
                dw      1

%define fat_start       bp+0x42         ; first FAT sector
                dd      1

%define root_dir_start  bp+0x46         ; first root directory sector
                dd      19		

%define data_start      bp+0x4A         ; first data sector
                dd      33				; 19+14=33

showal:
		;;;;;;;;;;;;;;;;;;;;;;;sub print ax start
		push ax
		and al,0xf0
		shr al,4
		CMP AL,0xa
		jb add30	;below
		add al,0x7
	add30:
		add al,0x30
		mov ah,0xE
		int 0x10
		pop ax

		
		push ax
		and al,0x0f
		cmp al,0xa
		jb add30_	;below
		add al,0x7
	add30_:
		add al,0x30
		mov ah,0xE
		int 0x10
		pop ax

		ret
		;;;;;;;;;;;;;;;;;;;;;;;sub print ax end
;-----------------------------------------------------------------------
;   ENTRY
;-----------------------------------------------------------------------

real_start:     cli								;Addr: 0x7c4e
                cld								;Addr: 0x7c4f
		xor	ax, ax							;Addr: 0x7c50
                mov     ss, ax          ; initialize stack				;Addr: 0x7c52
		mov	ds, ax							; ###DS: 0x0
                mov     bp, 0x7c00						;         0x7c56
                lea     sp, [bp-0x20]						;         0x7c59
                sti								;             7c5c
                xor 	dl, dl
		int     0x13            ; reset drive                                                    ;             7c5d
;		int	0x12		; get memory available in AX
;		mov	ax, 0x01e0
;		mov	cl, 6		; move boot sector to higher memory
;		shl	ax, cl
;		sub	ax, 0x07e0

		mov	ax, 0x1FE0						;            7c5f
		mov	es, ax							; ###ES: 0x1FE0, DS: 0x0
		mov	si, bp							;	7c64
		mov	di, bp							;	7c66
		mov	cx, 0x0100						;	7c68
		rep	movsw		; Move word at address DS:(E)SI to address ES:(E)DI      ;7c6b
		
		
		push	es							;	7c6d  %%%%return to cs
		mov	bx, cont						;	7c6e
		push	bx							;	7c71  %%%%return to ip
		
		
		retf								;	7c72

cont:   									; ### now cs:1fe0, ip: cont
		;;;;Show cs
        ;push ax
        ;mov ax,cs
        ;xchg al,ah
		;call showal
		;pop ax
		;jmp $
		
		mov     ds, ax				;###DS:0x1FE0, ES:0x1FE0			;	7c73
		mov	ss, ax							;	7c75
        mov     [drive], dl     ; BIOS passes drive number in DL		;	7c77

		
		;;;;;;;;;;;;;;;;;;;;ADD TEMP
		;jmp getchain
		;;;;;;;;;;;;;;;;;;;ADD END
		
;---------------Add by me start ---------------
;		call	print
;		db	"Get Driver: ",0
;		push ax
;
;		mov al,[drive]
;		add al,0x0030
;		
;		mov ah,0xE
;		int 0x10
;
;		pop ax
;---------------Add by me end ----------------
	
                ;call    print					;[print]->0x7d7a		7c7a
                ;db      13,10,13,10,"Loading FreeDOS...",13,10,"ROOT",0
                ;db      13,10,"Loading DOS.",0



;       FINDFILE: Searches for the file in the root directory.
;
;       Returns:
;                               AX = first cluster of file
				; Get content from bios to ES:BX
                ; First, read the whole root directory
                ; into the temporary buffer.

                mov     ax, word [root_dir_start]	;19	sector, read filename from root sectors	;	7c99
                mov     dx, word [root_dir_start+2]				;	7c9d
                mov     di, word [RootDirSecs]		;14			;	7ca0
                xor     bx, bx					;	7ca3
                mov     es, [tempbuf]			;###es: 0x0060, ds: 0x1FE0
												; now 0x0060:0000 contains:
												; KERNEL  SYS .....
												; ......0200 5e20 0000 (start cluster is 0x2)
												; .............
		
                call    readDisk				;[readDisk]->0x7d8a	7ca8

                ;;;Add by me , now see  es:bx;;;
;		mov cx, 1
;		mov di, 0;0x7c00;//filename
;	test1:
;		mov ax, [es:di];//ds;//[ds:bx];//[bx]
;		inc di
;		inc di
;		call showal
;		xchg ah,al
;		call showal
;		loop test1
                ;;;end;;;;;;;;;;;;;;
                
                jc      jmp_boot_error				;[jmp_bot_eror]->0x7cfe	7cab

                xor     di, di							;	7cad
		
		; Search for KERNEL.SYS file name, and find start cluster.

next_entry:     mov     cx, 11							;	7caf
                mov     si, filename				;[filename]->0x7dda	7cb2

		;push ax
		;mov ax,di
		;call showal
		;pop ax
                repe    cmpsb							;	7cd0
                ;pop     di							;	7cd1
                ;mov     ax, [es:di+0x1A]; get cluster number from directory entry	;	7cd2
                je      ffDone					;[ffDone]->7ce3		7cd6

				sub	si, filename
				sub	di, si
                add     di, 0x20   ; go to next directory entry			;	7cd8

		;;;;;;;;;;;;;
		;push ax
		;call    print
		;db      13,10,"n 2",13,10,0
		;pop ax
                ;;;;;;;;;;;;;;
                
		;---------------Add by me start ---------------
;		push ax
;		call	print
;		db	13,10,"Go to next directory",0
;		pop ax
		;;;;;;;;;;;;;;;
		
                cmp     byte [es:di], 0	; if the first byte of the name is 0,		;	7cdb
                jnz     next_entry	; there is no more files in the directory		;	7ce1

                jc      boot_error	; fail if not found				;	7cdf
ffDone:
				sub		di, 11	;sub filename.length
				mov     ax, [es:di+0x1A]; get start cluster number from directory entry
                push    ax              ; store start cluster number
			
				;;;;Get Ax begin
                ;push ax
				;mov ax,di
				;xchg ah,al
				;call showal
				;pop ax
				;;;;Get Ax end
				
                call    print
                db      " FAT",0


;getchain:
;       GETFATCHAIN:
;
;       Reads the FAT chain and stores it in a temporary buffer in the first
;       64 kb.  The FAT chain is stored an array of 16-bit cluster numbers,
;       ending with 0.
;
;       The file must fit in conventional memory, so it can't be larger than
;       640 kb. The sector size must be at least 512 bytes, so the FAT chain
;       can't be larger than around 3 kb.
;
;       Call with:      AX = first cluster in chain
				; and the bios will return data to ES:BX
                ; Load the complete FAT into memory. The FAT can't be larger
                ; than 128 kb, so it should fit in the temporary buffer.

                mov     es, [tempbuf] 		; ###es->0060, ds=>0x1fe0
                xor     bx, bx
                mov     di, [sectPerFat]	;9
                mov     ax, word [fat_start]	;read 9 sectors start from sector 2
                mov     dx, word [fat_start+2]
                ;mov		di, 2
                ;mov     ax, word [root_dir_start]	;19			;	7c99
                ;mov     dx, word [root_dir_start+2]
                call    readDisk			;now es->0600
											;0600:0000 contains below: 
											;(contents from sector 2: 0x0200)
											; f0ff ff03 f0ff 0000 0000 0000
											

				;;;Add by me , now see  es:bx;;;
				mov cx, 3		; read cx character
				mov di, 0;0x7c00;//filename
		test1:
				mov ax, [es:di];//ds;//[ds:bx];//[bx]
				inc di
				inc di
				;call showal
				xchg ah,al
				call showal
				loop test1
				
				push ax			;print "\r\n"
				call    print
				db      13,10,0
				pop ax
                ;;;end;;;;;;;;;;;;;;

                
                pop     ax                      ; restore start cluster number
                
                ;;;;Get Ax begin
                ;push ax
				;mov ax,es
				;xchg ah,al
				;call showal
				;pop ax
				;;;;Get Ax end
				
jmp_boot_error: jc      boot_error
				
				
                ; Set ES:DI to the temporary storage for the FAT chain.
                push    ds	; ###ds:0x1fe0	1fe0:0000=> 0000 0000 0000 0000
                push    es	; ###es:0x0060, 0060:0000=> f0ff ff03 f0ff 0000
                pop     ds	; ###ds->0x0060
                pop     es	; ###es->0x1FE0
                mov     di, FATBUF	;0x2000	offset of temporary buffer for FAT chain
									;DI use DS as segment register

next_clust:     
				;;;;;;;;;;;;;
				;push ax
				;call    print
				;db      13,10,"n 2",13,10,0
				;pop ax
				;jmp $
				;---------------
				
				
				;mov ax,0x1fe0
				;push ds
				;mov ds,ax
				;call    print					;[print]->0x7d7a		7c7a
                ;db      13,10,13,10,"Loading FreeDOS...",13,10,"ROOT",0
                ;db      13,10,"Ldg O",0
                ;pop ds
				
				stosw					; store cluster number ;store AX at address ES:(E)DI;
										; 1fe0:2000 will store ax ( the start cluster of KERNEL.SYS)
				;;;Show ax
				;push ax
				;call showal	;eg: 0x02
				;pop ax
				;jmp $
				;;;end
                mov     si, ax			; SI = cluster number
										; SI use DS as segment register

%ifdef ISFAT12
                ; This is a FAT-12 disk.

fat_12:         add     si, si          ; multiply cluster number by 3...
                add     si, ax
                shr     si, 1           ; ...and divide by 2
										; save for cross cluster
										; now si has the exact start cluster of KERNEL.SYS
                ;;;;Get SI begin
                ;push ax
				;mov ax,si
				;xchg ah,al
				;call showal
				;pop ax			;##show 03, si->03
				;jmp $
				;;;;Get SI end
				
                lodsw					;Load word at address DS:(E)SI into AX
										; DS:SI => 0060:ax*3/2 (if ax=2, will be 0060:0003)
										;eg: 0060:0003
										; f0ff ff03 f0ff
										; now AX contains the next cluster: 0x03

				;push ax
				;push si
				;push bx
				;call	print
				;db	"abc",0
				;pop bx
				;pop si
				;pop ax
				;jmp $
				
				;;;;Get AX
				;push ax
				;call showal
				;pop ax
				;jmp $
				;;;;End

				;---------------Add by me start ---------------
				;push ax
				;call	print
				;db	13,10,"af",0
				;pop ax
				;jmp $
				;---------------Add by me end ------------------
				
                ; If the cluster number was even, the cluster value is now in
                ; bits 0-11 of AX. If the cluster number was odd, the cluster
                ; value is in bits 4-15, and must be shifted right 4 bits. If
                ; the number was odd, CF was set in the last shift instruction.

                jnc     fat_even
                ;;;;Get AX
				;push ax
				;call showal
				;pop ax
				;jmp $
				;;;;End
                ;---------------Add by me start ---------------
				;push ax
				;call	print
				;db	13,10,"not jnc",0
				;pop ax
				;---------------Add by me end ------------------
				
                mov     cl, 4
                shr     ax, cl          ; shift the cluster number

fat_even:       
				;;;;Get AX
				;push ax
				;call showal
				;pop ax
				;jmp $
				;;;;End
				
				;---------------Add by me start ---------------
				;push ax
				;call	print
				;db	13,10,"yes jnc",0
				;pop ax
				;---------------Add by me end ------------------
				
				and     ah, 0x0f        ; mask off the highest 4 bits
                cmp     ax, 0x0fff      ; check for EOF
                jb      next_clust      ; continue if not EOF
                
                ;;;;;;;;;;Add me
                ;jmp $
                ;;;;;;;;;;

%endif
%ifdef ISFAT16
                ; This is a FAT-16 disk. The maximal size of a 16-bit FAT
                ; is 128 kb, so it may not fit within a single 64 kb segment.

fat_16:         mov     dx, [tempbuf]
                add     si, si          ; multiply cluster number by two
                jnc     first_half      ; if overflow...
                add     dh, 0x10        ; ...add 64 kb to segment value

first_half:     mov     ds, dx          ; DS:SI = pointer to next cluster
                lodsw                   ; AX = next cluster

                cmp     ax, 0xfff8      ; >= FFF8 = 16-bit EOF
                jb      next_clust      ; continue if not EOF
%endif

finished:       ; Mark end of FAT chain with 0, so we have a single
                ; EOF marker for both FAT-12 and FAT-16 systems.

				;;;Show ax
				;push ax
				;call showal	;eg: 0x02
				;pop ax
				;jmp $
				;;;end
				
                xor     ax, ax
                
                stosw					; store AX at address es:(e)di;
										; 1fe0:2000 will store ax (0x0)
										; now 1fe0:2000 will contain:
										; low --------> high
										; 0200 0300 0000 (kernel.sys file starts at 1st cluster and 2nd cluster)

				;;;; show contents at 1fe0:2000
;				mov cx, 5		; read cx character
;				mov di, 0x2000;0x7c00;//filename
;		test2:
;				mov ax, [es:di];//ds;//[ds:bx];//[bx]
;				inc di
;				inc di
;				call showal
;				xchg ah,al
;				call showal
;				loop test2				
;				jmp $
				;;;;
				
                push    cs				; ####cs->1fe0
                pop     ds				; ###ds->0x1fe0, es->0x1fe0
                
                
                ;;;;Show ds
                ;push ax
                ;mov ax,cs
				;call showal
				;pop ax
				;jmp $
                ;;;

                call    print
                db      " KERNEL",0
                
                ;jmp $

;       loadFile: Loads the file into memory, one cluster at a time.
;                 load to 0060:0000

                mov     es, [tempbuf]   ; set ES:BX to load address
										; ### es->0060, ds->1fe0
										; es:bx -> 0060:0000
                xor     bx, bx

                mov     si, FATBUF      ; set DS:SI to the FAT chain
										; ds:si -> 1fe0:2000
										; low -------> high
										; 0200 0300 0000 (kernel.sys file starts at 1st cluster and 2nd cluster)

cluster_next:   
				lodsw                           ; AX = next cluster to read
												;Load word at address ds:(e)si into AX
                or      ax, ax                  ; if EOF...
                je      boot_success            ; ...boot was successful

                dec     ax                      ; cluster numbers start with 2
                dec     ax

                mov     di, word [bsSecPerClust] ; di=1
                and     di, 0xff                ; DI = sectors per cluster
                mul     di
                
				
                add     ax, [data_start]		; ax+33
                adc     dx, [data_start+2]      ; DX:AX = first sector to read
%if 0
                ;;;;Show ds
                push ax
                ;mov ax,cs
				call showal
				pop ax
				jmp $
				;;;;;
%endif

				cmp		bx, 0					; is BX += 0x200 , overflow
				jne		callReadDisk
				cmp		ax, 0xa0				; is AX gread enough
				jl		callReadDisk
				push	ax						; es + 0x1000 -> 0x1060
				mov		ax, es
				add		ax, 0x1000
				mov		es, ax
				pop		ax
				
		callReadDisk:
				;es:bx = destination buffer		es:bx -> 0060:0000
                call    readDisk
                
                ;;;;;;;;;
;				mov cx, 10		; read cx character
;				mov di, 0x0;0x7c00;//filename
;		test3:
;				mov ax, [es:di];//ds;//[ds:bx];//[bx]
;				inc di
;				inc di
;				call showal
;				xchg ah,al
;				call showal
;				loop test3
				;jmp $
                ;;;;;;;;
                
                jnc     cluster_next


boot_error:     call    print
                ;db      13,10,"BOOT error!",13,10,0
                db      13,10,"BOT eor!",13,10,0

		xor	ah,ah
		int	0x16			; wait for a key
		int	0x19			; reboot the machine

boot_success:   call    print
                db      " GO!",13,10,0
                ;jmp $
                mov     bl, [drive]
				mov ax,  0x1d9
				mov ds, ax
		jmp	word LOADSEG:0x127c


; prints text after call to this function.

print:          pop   si                       ; this is the first character
                xor   bx, bx                   ; video page 0
                mov   ah, 0x0E                 ; else print it //TTY character output, al=character,bh=page
print1:         lodsb                          ; get token  //Load byte at address DS:(E)SI into AL
                cmp   al, 0                    ; end of string?
                je    print2                   ; if so, exit
                int   0x10                     ; via TTY mode
                jmp   short print1             ; until done
print2:         push  si                       ; stack up return address
                ret                            ; and jump to it


;       readDisk:       Reads a number of sectors into memory.
;
;       Call with:      DX:AX = 32-bit DOS sector number
;                       DI = number of sectors to read
;                       ES:BX = destination buffer
;                       ES must be 64k aligned (1000h, 2000h etc).
;
;       Returns:        CF set on error
;                       ES:BX points one byte after the last byte read.

readDisk:       push    si
read_next:      push    dx
                push    ax
		;;;;;;;;;;;;;;
		;---------------Add by me start ---------------
		;push ax
		;call	print
		;db	13,10,"In r",0
		;pop ax
		;;;;;;;;;;;;;;;

		;---------------Add by me start ---------------
;		call	print
;		db	13,10,"readDisk al: ",0
;		pop ax
;		push ax
;		xor ah,ah
;		mov al,al
;		add al,0x0030
		
;		mov ah,0xE
;		int 0x10

;		pop ax
		;---------------Add by me end ----------------
                ;
                ; translate sector number to BIOS parameters
                ;

                ;
                ; abs = sector                          offset in track
                ;     + head * sectPerTrack             offset in cylinder
                ;     + track * sectPerTrack * nHeads   offset in platter //nHeads=2
                ;
                ; t1     = abs  /  sectPerTrack         (ax has t1)
                ; sector = abs mod sectPerTrack         (cx has sector)
                ;


                div     word [sectPerTrack]	;sectPerTrack=18 //quotient->AX, remainder->DX
                mov     cx, dx			; remainder->CX CX=sector in which track

                ;
                ; t1   = head + track * nHeads
                ;
                ; track = t1  /  nHeads                 (ax has track)
                ; head  = t1 mod nHeads                 (dl has head)
                ;
                xor     dx, dx			;DX:AX -> 0:AX
                div     word [nHeads]		;nHeads=2 //quotient->AX , remainder->DX
						;for now DX=1(header=1), AX=0(track 0), CX=1(sector)

		inc cl				;CL = start sector (1-80)
		mov ch,al			;CH = track number
		mov dh,dl			;DH = header number ( 0 or 1)
		mov dl,0			;driver number (0 or 1)
		mov ax,0x0201			;ah=02h, al=sectors want to read = 1
		int 0x13
                ; the following manipulations are necessary in order to
                ; properly place parameters into registers.
                ; ch = cylinder number low 8 bits
                ; cl = 7-6: cylinder high two bits
                ;      5-0: sector
;                mov     dh, dl                  ; save head into dh for bios
;                ror     ah, 1                   ; move track high bits into
;                ror     ah, 1                   ; bits 7-6 (assumes top = 0)
;                xchg    al, ah                  ; swap for later
;                mov     dl, byte [sectPerTrack]
;                sub     dl, cl
;                inc     cl                      ; sector offset from 1 
;                or      cx, ax                  ; merge cylinder into sector
;                mov     al, dl                  ; al has # of sectors left

;                mov     ax, 0x0201
;                mov     dl, [drive]
;                int     0x13

                jnc     read_ok                 ; jump if no error
                
                xor     ah, ah                  ; else, reset floppy
                int     0x13
                pop     ax
                pop     dx                      ; and...
                jmp     short read_next         ; read the same sector again

read_ok:

		;---------------Add by me start ---------------
		;push ax
		;call	print
		;db	13,10,"readO",0
		;db	"AB",0
		;pop ax
		;;;;;;;;;;;;;;;

		;push ax
		;mov ax,bx
		;call showal
		;pop ax
		
                ;add     bx, word [bsBytesPerSec]
		
                jnc     no_incr_es              ; if overflow...

                ;;;;;;;;Add
                ;push ax
		;mov ax,es
		;call showal
		;pop ax
		;;;;;;;;;
		
                mov     ax, es
                add     ah, 0x10                ; ...add 1000h to ES
                mov     es, ax

no_incr_es:     
		;---------------Add by me start ---------------
		;push ax
		;call	print
		;db	"no",0
		;pop ax
		;;;;;;;;;;;;;;;
		pop     ax
                pop     dx                      ; DX:AX = last sector number

                add     ax, 1
                adc     dx, byte 0              ; DX:AX = next sector to read

                add	bx, 512
                dec     di                      ; if there is anything left to read,
                jnz     read_next               ; continue

                clc
                pop     si
                ret

filename        db      "KERNEL  SYS"

		times	0x01fe-$+$$ db 0

sign            dw      0xAA55

