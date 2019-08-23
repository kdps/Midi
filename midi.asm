.MODEL TINY
.186
.CODE

PUBLIC _settable
PUBLIC _pcmstart
PUBLIC _pcmstop
PUBLIC _playseq
PUBLIC _waitempty
PUBLIC _displayoff
PUBLIC _displayon
PUBLIC _mykbhit
PUBLIC _mygetch
PUBLIC _pc98patch
PUBLIC _pcmstart98
PUBLIC _pcmstop98

even

_timer1	proc
	mov	ax,40h
	mov	ds,ax
	cmp	byte ptr ds:[0095h],0
	jnz	t1j1
	cli
	mov	al,20H
	out	20H,AL
	sti
	mov	ax,0f71ah
	out	22H,AX
	retf

t1j1:	jmp 	dword ptr cs:[timer1_org_off]
_timer1	endp

even

_int0ah  proc
	push	ax
	push	ds
	mov	ax,0040h
	mov	ds,ax
	cmp	byte ptr ds:[0095h],0
	pop	ds
	jnz	i0al1
	cli
	mov	al,20h
	out	20h,al
	sti
i0al1:	pop	ax
	jmp	dword ptr cs:[int0aofs]
_int0ah	endp

even

_int08h	proc
	push	ax
	push	bx
	mov	bx,0100h
PCMRP	equ	$-2
	mov	al,byte ptr cs:[BX+offset pcmdata]
	out	42h,al
	inc	bx
	and	bh,07h
	mov	word ptr cs:[PCMRP],bx
	mov	al,20h
	out	20h,al
	pop	bx
	pop	ax
	iret
_int08h	endp

even

_int08h98	proc
	push	ax
	push	bx
	push	dx
	mov	bx,0100h
PCMRP2	equ	$-2
	mov	al,byte ptr cs:[BX+offset pcmdata]
	mov	dx,3fdbh
	out	dx,al
	inc	bx
	and	bh,07h
	mov	word ptr cs:[PCMRP2],bx
	mov	al,20h
	out	00h,al
	pop	dx
	pop	bx
	pop	ax
	iret
_int08h98	endp

even

_synthe	proc
	mov	ah,0
PCMWP	equ	$-1	;Byte ptr
synl2:	cmp	ah,byte ptr ds:[PCMRP+1]
pat98a	equ	$-2
	jz	synl2
	sub	al,al
	mov	si,ax
	add	si,offset pcmdata

	sub	ch,ch				; loop counter
	mov	cl,byte ptr ds:[revlevel]
	mov	bx,offset phase
	nop

synl3:	mov	ax,word ptr ds:[bx]
NOISE	equ	$	; replace to 'add ah,al'
	nop
	nop
	add	ax,0100h
tonea	equ	$-2
	mov	word ptr ds:[bx],ax
	mov	al,1
	jns	synj1a
	add	al,00h
vola	equ	$-1

synj1a:	add	word ptr ds:[bx+8],0100h
toneb	equ	$-2
	jns	synj1b
	add	al,00h
volb	equ	$-1

synj1b:	add	word ptr ds:[bx+16],0100h
tonec	equ	$-2
	jns	synj1c
	add	al,00h
volc	equ	$-1

synj1c:	add	word ptr ds:[bx+24],0100h
toned	equ	$-2
	jns	synj1d
	add	al,00h
vold	equ	$-1

poly4	equ	$		; replace to 'jmp polyjmp'
synj1d:	add	word ptr ds:[bx+32],0100h
tonee	equ	$-2
	jns	synj1e
	add	al,00h
vole	equ	$-1

synj1e:	add	word ptr ds:[bx+40],0100h
tonef	equ	$-2
	jns	synj1f
	add	al,00h
volf	equ	$-1

synj1f:	add	word ptr ds:[bx+48],0100h
toneg	equ	$-2
	jns	synj1g
	add	al,00h
volg	equ	$-1

synj1g:	add	word ptr ds:[bx+56],0100h
toneh	equ	$-2
	jns	synj1h
	add	al,00h
volh	equ	$-1

poly8	equ	$		; replace to 'jmp polyjmp'
synj1h:	add	word ptr ds:[bx+64],0100h
tonei	equ	$-2
	jns	synj1i
	add	al,00h
voli	equ	$-1

synj1i:	add	word ptr ds:[bx+72],0100h
tonej	equ	$-2
	jns	synj1j
	add	al,00h
volj	equ	$-1

synj1j:	add	word ptr ds:[bx+80],0100h
tonek	equ	$-2
	jns	synj1k
	add	al,00h
volk	equ	$-1

synj1k:	add	word ptr ds:[bx+88],0100h
tonel	equ	$-2
	jns	synj1l
	add	al,00h
voll	equ	$-1

poly12	equ	$		; replace to 'jmp polyjmp'
synj1l:	add	word ptr ds:[bx+96],0100h
tonem	equ	$-2
	jns	synj1m
	add	al,00h
volm	equ	$-1

synj1m:	add	word ptr ds:[bx+104],0100h
tonen	equ	$-2
	jns	synj1n
	add	al,00h
voln	equ	$-1

synj1n:	add	word ptr ds:[bx+112],0100h
toneo	equ	$-2
	jns	synj1o
	add	al,00h
volo	equ	$-1

synj1o:	add	word ptr ds:[bx+120],0100h
tonep	equ	$-2
	jns	synj1p
	add	al,00h
volp	equ	$-1

polyjmp	equ	$
synj1p:	mov	ah,byte ptr ds:[si]
	shr	ah,cl			; reverb
	add	ah,al
	mov	byte ptr ds:[si],ah
	inc	si
	dec	ch
	jz	synend
	jmp	synl3

synend:	mov	al,byte ptr ds:[PCMWP]
	inc	al
	and	al,07h
	mov	byte ptr ds:[PCMWP],al
	ret
_synthe	endp

_waitempty proc
	mov	al,byte ptr cs:[PCMWP]
	dec	al
	and	al,07h
wel1:	cmp	al,byte ptr cs:[PCMRP+1]
pat98b	equ	$-2
	jnz	wel1
	sub	ax,ax
	ret
_waitempty endp

_pcmstart	proc
	push	bp
	mov	bp,sp
	mov	ax,[bp+8]	; limit max polyphonous
	cmp	ax,4
	jz	stj0
	cmp	ax,8
	jz	stj1
	cmp	ax,12
	jz	stj2
	jmp	stj3

stj0:	mov	ah,offset polyjmp-offset poly4-2
	mov	al,0ebh
	mov	word ptr cs:[poly4],ax
	jmp	stj3

stj1:	mov	ah,offset polyjmp-offset poly8-2
	mov	al,0ebh
	mov	word ptr cs:[poly8],ax
	jmp	stj3

stj2:	mov	ah,offset polyjmp-offset poly12-2
	mov	al,0ebh
	mov	word ptr cs:[poly12],ax

stj3:
	cli
	in	al,21h
	or	al,00000001b
	out	21h,al
	push	ds
	mov	ax,0000h
	mov	ds,ax
	mov	ax,offset _int08h
	xchg	ax,word ptr ds:[4*8]
	mov	word ptr cs:[int08ofs],ax
	mov	ax,cs
	xchg	ax,word ptr ds:[4*8+2]
	mov	word ptr cs:[int08seg],ax

	test	word ptr [bp+6],01h

	jnz	stskp

	mov	ax,offset _int0ah
	xchg	ax,word ptr ds:[4*10]
	mov	word ptr cs:[int0aofs],ax
	mov	ax,cs
	xchg	ax,word ptr ds:[4*10+2]
	mov	word ptr cs:[int0aseg],ax

	mov	al,87h
	out	22h,al
	in	al,23h
	push	ax
	mov	al,0a0h
	out	23h,al
	mov	ax, 9000h
	mov	ds, ax
	mov	ax, offset cs:_timer1
	push	cs
	pop	bx
	xchg	ax, ds:[0f7b4h]
	xchg	bx, ds:[0f7b6h]
	mov	cs:[timer1_org_off], ax
	mov	cs:[timer1_org_seg], bx
	pop	ax
	out	23h,al

stskp:	pop	ds

	mov	al,00110100b
	out	43h,al
	mov	ax,[bp+4]
	out	40h,al
	mov	al,ah
	out	40h,al
	mov	al,10010100b
	out	43h,al
	mov	al,1
	out	42h,al

	in	al,21h
	and	al,11111110b
	out	21h,al

	in	al,61h
	or	al,3
	out	61h,al
	mov	al,1
stlp2:	out	42h,al
	mov	cx,4000h
stlp:	dec	cx
	jnz	stlp
	inc	al
	cmp	al,04h
	jnz	stlp2
	mov	al,10010000b
	out	43h,al
	mov	al,1
	out	42h,al
	sti
	sub	ax,ax
	pop	bp
	ret
_pcmstart endp

_pcmstop proc
	push	bp
	mov	bp,sp
	cli
	in	al,21h
	or	al,00000001b
	out	21h,al
	push	ds
	mov	ax,0000h
	mov	ds,ax
	mov	ax,word ptr cs:[int08ofs]
	mov	word ptr ds:[4*8],ax
	mov	ax,word ptr cs:[int08seg]
	mov	word ptr ds:[4*8+2],ax

	test	word ptr [bp+4],01h
	jnz	spskp

	mov	ax,word ptr cs:[int0aofs]
	mov	word ptr ds:[4*10],ax
	mov	ax,word ptr cs:[int0aseg]
	mov	word ptr ds:[4*10+2],ax

	mov	al,87h
	out	22h,al
	in	al,23h
	push	ax
	mov	al,0a0h
	out	23h,al
	mov	ax, 9000h
	mov	ds, ax
	mov	ax,cs:[timer1_org_off]
	mov	bx,cs:[timer1_org_seg]
	mov	ds:[0f7b4h],ax
	mov	ds:[0f7b6h],bx
	pop	ax
	out	23h,al

spskp:	pop	ds

	mov	al,10010100b
	out	43h,al

	mov	al,4
enlp2:	out	42h,al
	mov	cx,4000h
enlp:	dec	cx
	jnz	enlp
	dec	al
	jnz	enlp2

	mov	al,00110100b
	out	43h,al
	mov	al,0
	out	40h,al
	mov	al,0
	out	40h,al

stlp1:	in	al,40h
	in	al,40h
	cmp	al,80h
	jb	stlp1

	mov	al,60h
	out	20h,al

	in	al,21h
	and	al,11111110b
	out	21h,al
	in	al,61h
	and	al,11111100b
	out	61h,al
	mov	al,10110110b
	out	43h,al
	mov	al,33h
	out	42h,al
	mov	al,05h
	out	42h,al
	sti
	sub	ax,ax
	pop	bp
	ret
_pcmstop endp

; * play sequencer
; * [es:di] music data ptr
; * data: [interval(channel bit 3)] [data] .....
; * interval: 00h - 7Fh   1 byte   lsb:channel (3bit)
; *           80h - 7FFFh 1 word  bit 7 = 1 [15:8][6:0] -> [14:7][6:0]
; * data      vvvv vccc  tttt tttv
; *           v:volume t:tone c:channel(0-2bit)
; *           v=0:key off
; *           t=0:pitch bend +0  - +63
; *           t=1:pitch bend +64 - +127
; *           t=2:pitch bend -0  - -63
; *           t=3:pitch bend -64 - -127
; *           ffh,ffh: seq end
; *           00h,ffh: nop
	
;* int	playseq(unsigned int dataofs,unsigned int data seg);
_playseq proc
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ds
	push	es
	mov	bx,word ptr [bp+4]	; data pointer (&ofs)
	mov	di,ds:[bx]
	mov	ax,cs
	mov	ds,ax
	mov	ax,word ptr [bp+6]	; data pointer (seg)
	mov	es,ax
	mov	ax,word ptr [bp+8]	; rev level
	mov	byte ptr [revlevel],al


pll1:	mov	dx,di
	mov	bx,word ptr es:[di]	; read interval
	inc	di
	test	bl,0feh	; interval=0
	jz	plj1
	jns	plj2	; bit 7 = 0

	shl	bl,1
	shr	bx,1
	inc	di
	jmp	short plj3

plj2:	sub	bh,bh	; bl:interval
plj3:	mov	cx,ds
	sub	ax,ax
	mov	ds,ax
	mov	al,byte ptr ds:[417h]
pat98c	equ	$-2
	mov	ds,cx
	test	al,0Ch
pat98d	equ	$-1
	jz	pljy

	sub	bx,bx
	mov	ds,bx
	and	byte ptr ds:[417h],0f0h
pat98e	equ	$-1
	jmp	plj4	; user break

pljy:	push	bx
	shr	bx,1

	test	al,03h
pat98d2	equ	$-1
	jz	pll2
	shr	bx,1
	or	bx,bx
	jz	pljxx

pll2:	push	bx
	call	_synthe
	pop	bx
	dec	bx
	jnz	pll2

pljxx:	pop	bx

plj1:	mov	dx,word ptr es:[di]	; read data
	inc	di
	inc	di
	cmp	dh,0ffh
	jnz	plj9
	or	dl,dl
	jz	pll1		; FF00H:nop
	cmp	dl,0ffh
	mov	al,0
	jz	plj4		; FFFFH:seq end

plj9:	and	bl,01h
	shl	bl,3
	mov	bh,dl
	and	bh,07h
	or	bl,bh
	shl	bl,3
	sub	bh,bh	; bx:Channel*8

	sub	ch,ch
	mov	cl,dh
	and	cl,0feh	; cx:tone x 2

	jz	pbend1	; tone=0:pitch bend plus
	cmp	cl,04h
	jc	pbend2	; tone=1:pitch bend plus 64
	jz	pbend3	; tone=2:pitch bend minus
	cmp	cl,06h
	jz	pbend4	; tone=3:pitch bend minus 64

	shr	dx,1
	shr	dl,2	; dl:Volume
	jz	plkoff
	jmp	plj5

; Key Off
plkoff:
	sub	ax,ax
	mov	si,word ptr ds:[bx+offset vol]
	mov	byte ptr ds:[si],al
	cmp	cl,0feh
	jnz	plj31
	mov	si,word ptr ds:[bx+offset tone]
	mov	word ptr ds:[si],ax
	mov	word ptr ds:[bx+offset phase],ax
	mov	byte ptr ds:[bx+offset pitbend],al
	or	bl,bl
	jnz	plj31
	mov	word ptr ds:[NOISE],9090h	; nop x2
plj31:	jmp	pll1

; seq end
plj4:	pop	es
	pop	ds
	mov	bx,word ptr [bp+4]	; data pointer (&ofs)
	mov	ds:[bx],dx
	mov	ah,0
	pop	di
	pop	si
	pop	bp
	ret

; Pitch Bend (+64)
pbend2:	
	add	dx,40h*8

; Pitch Bend (+)
pbend1:
	shr	dx,1
	shr	dl,2	; dl:Pitch Bend (25cent)

	mov	byte ptr ds:[BX+offset pitbend],dl
	mov	cl,byte ptr ds:[BX+offset key]	; get key
	sub	ch,ch
	jmp	short pljx

; Pitch Bend (-64)
pbend4:	
	add	dx,40h*8

; Pitch Bend (-)
pbend3:
	shr	dx,1
	shr	dl,2
	neg	dl	; dl:Pitch Bend (25cent)

	mov	byte ptr ds:[BX+offset pitbend],dl
	mov	cl,byte ptr ds:[BX+offset key]	; get key
	sub	ch,ch

pljx:	
	shl	cx,1
	mov	al,dl	; get pitbend
	cbw
	add	cx,ax
	shl	cx,1
	mov	si,cx
	mov	ax,word ptr ds:[si+offset tonetbl]
	mov	si,word ptr ds:[bx+offset tone]
	mov	word ptr ds:[si],ax

	jmp	pll1


; Key On
plj5:	mov	byte ptr ds:[BX+offset key],cl		; store key

	or	bl,bl	; ch 0
	jnz	plj51
	cmp	cl,2*4	; key 4
	jnz	plj51

; noise 
	mov	word ptr ds:[NOISE],0c400h	; add ah,al
	mov	word ptr ds:[tonea],35		; n=(n*257+35)%65536
	mov	byte ptr ds:[vola],dl
	jmp	pll1

plj51:	shl	cx,1
	mov	al,byte ptr ds:[BX+offset pitbend]	; get pitbend
	cbw
	add	cx,ax
	shl	cx,1
	mov	si,cx
	mov	ax,word ptr ds:[si+offset tonetbl]
	mov	si,word ptr ds:[bx+offset tone]
	mov	word ptr ds:[si],ax
	mov	si,word ptr ds:[bx+offset vol]
	mov	byte ptr ds:[si],dl

	jmp	pll1

_playseq endp

_settable	proc
	push	bp
	mov	bp,sp
	mov	bx,[bp+4]
	shl	bx,1
	mov	ax,[bp+6]
	mov	word ptr cs:[bx+tonetbl],ax
	pop	bp
	ret
_settable endp

_displayoff	proc
	cli
	mov	dx,03d8h
	in	al,dx
	and	al,0f7h
	out	dx,al

	mov	bx,2
doflp0:	mov	ax,0
doflp1:	dec	ax
	jnz	doflp1
	dec	bx
	jnz	doflp0
	
	mov	al,23h
	out	22h,al
	in	al,23h
	mov	byte ptr cs:[contrast],al
	mov	al,0
	out	23h,al

	mov	al,51h
	out	22h,al
	in	al,23h
	and	al,0fdh
	out	23h,al

	mov	al,1eh
	out	22h,al
	in	al,23h
	and	al,0fbh
	out	23h,al
	sti
	ret
_displayoff endp

_displayon	proc
	cli
	mov	al,1eh
	out	22h,al
	in	al,23h
	or	al,04h
	out	23h,al

	mov	al,51h
	out	22h,al
	in	al,23h
	or	al,02h
	out	23h,al

	mov	al,23h
	out	22h,al
	mov	al,byte ptr cs:[contrast]
	out	23h,al

	mov	bx,2
doflp3:	mov	ax,0
doflp2:	dec	ax
	jnz	doflp2
	dec	bx
	jnz	doflp3

	mov	dx,03d8h
	in	al,dx
	or	al,08h
	out	dx,al
	
	sti
	ret
_displayon endp

_mykbhit proc
	mov	ah,0bh
	int	21H		; Check Keybord Status
	cbw
	ret
_mykbhit endp

_mygetch proc
	mov	ax,0700h
	int	21H		; Direct Character Input Without Echo
	mov	ah,0
	ret
_mygetch endp

_pc98patch proc
	push	ds
	mov	ax,cs
	mov	ds,ax
	mov	word ptr [pat98a],offset PCMRP2+1
	mov	word ptr [pat98b],offset PCMRP2+1
	mov	word ptr [pat98c],053ah
	mov	byte ptr [pat98d],18h
	mov	byte ptr [pat98d2],01h
	mov	byte ptr [pat98e],0ffh
	pop	ds
	ret
_pc98patch endp

_pcmstart98	proc
	push	bp
	mov	bp,sp
	mov	ax,[bp+6]	; limit max polyphonous
	cmp	ax,4
	jz	st98j0
	cmp	ax,8
	jz	st98j1
	cmp	ax,12
	jz	st98j2
	jmp	st98j3

st98j0:	mov	ah,offset polyjmp-offset poly4-2
	mov	al,0ebh
	mov	word ptr cs:[poly4],ax
	jmp	st98j3

st98j1:	mov	ah,offset polyjmp-offset poly8-2
	mov	al,0ebh
	mov	word ptr cs:[poly8],ax
	jmp	st98j3

st98j2:	mov	ah,offset polyjmp-offset poly12-2
	mov	al,0ebh
	mov	word ptr cs:[poly12],ax

st98j3:
	cli
	in	al,02h
	mov	byte ptr cs:[intmask],al
	or	al,00000001b
	out	02h,al
	push	ds
	mov	ax,0000h
	mov	ds,ax
	mov	ax,offset _int08h98
	xchg	ax,word ptr ds:[4*8]
	mov	word ptr cs:[int08ofs],ax
	mov	ax,cs
	xchg	ax,word ptr ds:[4*8+2]
	mov	word ptr cs:[int08seg],ax
	pop	ds

	mov	al,00110100b
	out	77h,al
	mov	ax,[bp+4]
	out	71h,al
	mov	al,ah
	out	71h,al
	mov	al,01010000b
	out	77h,al
	mov	al,1
	mov	dx,3fdbh
	out	dx,al

	in	al,02h
	and	al,11111110b
	out	02h,al

	mov	al,06h
	out	37h,al
	sti
	sub	ax,ax
	pop	bp
	ret
_pcmstart98 endp

_pcmstop98 proc
	cli
	in	al,02h
	or	al,00000001b
	out	02h,al
	push	ds
	mov	ax,0000h
	mov	ds,ax
	mov	ax,word ptr cs:[int08ofs]
	mov	word ptr ds:[4*8],ax
	mov	ax,word ptr cs:[int08seg]
	mov	word ptr ds:[4*8+2],ax
	pop	ds

	mov	al,00110100b
	out	77h,al
	mov	al,0
	out	71h,al
	mov	al,0
	out	71h,al
	mov	al,01110110b
	out	77h,al
	push	ds
	sub	ax,ax
	mov	ds,ax
	test	byte ptr ds:[0501h],80h
	pop	ds
	mov	ax,998
	jnz	psp98j1
	mov	ax,1229
psp98j1:mov	dx,3fdbh
	out	dx,al
	mov	al,ah
	out	dx,al

	in	al,02h
	and	al,11111110b
	mov	ah,byte ptr cs:[intmask]
	and	ah,00000001b
	or	al,ah
	out	02h,al
	mov	al,07h
	out	37h,al
	sti
	sub	ax,ax
	ret
_pcmstop98 endp




even

int08ofs	dw	0
int08seg	dw	0
int0aofs	dw	0
int0aseg	dw	0
timer1_org_off	dw	0
timer1_org_seg	dw	0

tone:	dw	offset tonea
phase:	dw	0
vol:	dw	offset vola
key:	db	0
pitbend:db	0

	dw	offset toneb
	dw	0
	dw	offset volb
	db	0
	db	0

	dw	offset tonec
	dw	0
	dw	offset volc
	db	0
	db	0

	dw	offset toned
	dw	0
	dw	offset vold
	db	0
	db	0

	dw	offset tonee
	dw	0
	dw	offset vole
	db	0
	db	0

	dw	offset tonef
	dw	0
	dw	offset volf
	db	0
	db	0

	dw	offset toneg
	dw	0
	dw	offset volg
	db	0
	db	0

	dw	offset toneh
	dw	0
	dw	offset volh
	db	0
	db	0

	dw	offset tonei
	dw	0
	dw	offset voli
	db	0
	db	0

	dw	offset tonej
	dw	0
	dw	offset volj
	db	0
	db	0

	dw	offset tonek
	dw	0
	dw	offset volk
	db	0
	db	0

	dw	offset tonel
	dw	0
	dw	offset voll
	db	0
	db	0

	dw	offset tonem
	dw	0
	dw	offset volm
	db	0
	db	0

	dw	offset tonen
	dw	0
	dw	offset voln
	db	0
	db	0

	dw	offset toneo
	dw	0
	dw	offset volo
	db	0
	db	0

	dw	offset tonep
	dw	0
	dw	offset volp
	db	0
	db	0


tonetbl:dw	200h dup (?)
pcmdata:db	800h dup (01h)

revlevel:db	2
contrast:db	20
intmask:db	0
	end

