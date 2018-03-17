.model small
.486
.stack 20h
.data
msg0 db "enter list size (01-FF): $"
msg1 db "enter list elements (byte size) : $"
choice db "a for ascending / d for descending : $"
msg2 db "                iteration no $"
msg3 db " : $"
arr db ?
.code
.startup
nocount: mov dl,0ah          ; line break
mov ah,02h
int 21h
mov dx, offset msg0 ; print msg0
mov ah,09h
int 21h
mov si,0320h        ; initialize arr offset/index
mov di,si           ; store offset/index
call ipt            ; fetch counter
cmp bx,00h
jz nocount
mov bp,bx           ; store counter
mov cx,bx           ; load counter
mov dl,0ah          ; line break
mov ah,02h
int 21h
mov dx, offset msg1 ; print msg1
mov ah,09h
int 21h
num: call ipt       ; input arr data
mov arr[si],bl      ; store data in arr
mov dl,20h          ; introduce space b/w input data
mov ah,02h
int 21h
inc si              ; increment index of arr
loop num
mov dl,0ah          ; line break
mov ah,02h
int 21h
mov dx, offset choice ; print choice
mov ah,09h
int 21h
mov ah,01h
int 21h
xor ah,ah
push ax
mov ch,01h
mov bx,bp		     ; load counter 
dec bl              ; set counter for search
sort1:  mov dl,0ah          ; line break
        mov ah,02h
        int 21h
        mov dx, offset msg2 ; print msg2
        mov ah,09h
        int 21h

        mov dl,ch   ; load iteration count
        rol dl,4    ; load upper nibble to lower nibble and viceversa
        and dl,0fh  ; mask lower nibble   
        cmp dl,09h  ; cmp for numerial digit
        jbe go10    ; yes then jmp
        add dl,07h  ; if not add 07 to create hex (A-F)
go10:   add dl,30h  ; add 30h to convert hex to ascii
        mov ah,02h  ; load control word for dos as o/p
        int 21h

        mov dl,ch     ; load iteration count
        and dl,0fh    ; mask lower nibble   
        cmp dl,09h    ; cmp for numerial digit
        jbe go12      ; yes then jmp
        add dl,07h    ; if not add 07 to create hex (A-F)
go12:   add dl,30h    ; add 30h to convert hex to ascii
        mov ah,02h    ; load control word for dos as o/p
        int 21h

        mov dx, offset msg3 ; print msg3
        mov ah,09h
        int 21h
        inc ch
        mov si,di   ; load initial offset/index
        
        mov bh,bl   ; set counter for inner loop
        pop dx
  .if (dl=='a' || dl=='A')
    sort2: mov al,arr[si]    ; load index value for comaperison
	        cmp al,arr[si+1]  ; compare with successive value
	        jbe nchng         ; jump if index value less or equal 
	        xchg al,arr[si+1] ; swap index and successive index values
	        xchg al,arr[si]
    nchng: inc si            ; increment index 
           dec bh            ; decrement inner counter
    jnz sort2                ; check for zero value of inner counter
  .elseif
    sort3: mov al,arr[si]    ; load index value for comaperison
	        cmp al,arr[si+1]  ; compare with successive value
	        jae nchng1        ; jump if index value greater or equal 
	        xchg al,arr[si+1] ; swap index and successive index values
	        xchg al,arr[si]
    nchng1:inc si            ; increment index 
           dec bh            ; decrement inner counter
    jnz sort3                ; check for zero value of inner counter
  .endif
  push dx  
mov si,di           ; load initial offset/index 
mov dx,bp           
mov cl,dl           ; load counter    
call opt            ; print list
dec bl              ; decrement search counter
jnz sort1  
mov dl,0ah
mov ah,02h
int 21h
.exit

ipt proc near
xor bx,bx           ; clear bx register
mov ah,01h          ; input data from screen
int 21h
sub al,30h          ; convert ascii to hex
cmp al,09h          ; check for hex numbers      
jbe nxt             ; if data is b/w (0-9) then jmp
sub al,07h          ; else convert (A-F) to hex
cmp al,0fh          ; check for small letters
jbe nxt
sub al,20h          ; convert (a-f) to hex
nxt: add bl,al      ; store data in bl
shl bl,4            ; shift nibble to upper one
mov ah,01h          ; repeat the above steps for lower nibble
int 21h
sub al,30h
cmp al,09h
jbe nxt1
sub al,07h
cmp al,0fh          ; check for small letters
jbe nxt1
sub al,20h          ; convert (a-f) to hex
nxt1: add bl,al
ret                 ; return to main program
ipt endp
 
opt proc near
num1: mov dl,arr[si] ; fetch data from arr
      rol dl,4       ; load upper nibble to lower nibble and viceversa
      and dl,0fh     ; mask lower nibble   
      cmp dl,09h     ; cmp for numerial digit
      jbe go1        ; yes then jmp
      add dl,07h     ; if not add 07 to create hex (A-F)
go1:  add dl,30h     ; add 30h to convert hex to ascii
      mov ah,02h     ; load control word for dos as o/p
      int 21h

      mov dl,arr[si] ; fetch data from arr
      and dl,0fh     ; mask lower nibble   
      cmp dl,09h     ; cmp for numerial digit
      jbe go2        ; yes then jmp
      add dl,07h     ; if not add 07 to create hex (A-F)
go2:  add dl,30h     ; add 30h to convert hex to ascii
      mov ah,02h     ; load control word for dos as o/p
      int 21h
      
      mov dl,' '     ; intro space in b/w data
      mov ah,02h
      int 21h
      inc si         ; increment arr index
      dec cl
jnz num1
ret
opt endp
 
end