.model small
.486
.stack 20h
.data
msg0 db "enter list size (00-FF): $"
msg1 db "enter list elements (byte size) : $"
msg2 db "                   your list is : $"
arr db ?
.code
.startup
mov dl,0ah          ; line break
mov ah,02h
int 21h
mov dx, offset msg0 ; print msg0
mov ah,09h
int 21h
mov si,0000h        ; initialize arr index
mov di,si           ; save arr index
call ipt            ; fetch counter
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
mov dl,' '          ; introduce space b/w input data
mov ah,02h
int 21h
inc si              ; increment index of arr
loop num
mov dl,0ah          ; line break
mov ah,02h
int 21h
mov dx, offset msg2 ; print msg2
mov ah,09h
int 21h
call ipt             ; input the number to be searched
mov cx,bp            ; load counter
mov si,di            ; load the strarting index of arr
call opt

.exit

ipt proc near
xor bx,bx           ; clear bx register
mov ah,01h          ; input data from screen
int 21h
sub al,30h          ; convert ascii to hex
cmp al,09h
jbe nxt             ; if data is b/w (0-9) then jmp
sub al,07h          ; else convert (A-F) to hex
nxt: mov bl,al      ; store data in bl
shl bl,4            ; shift nibble to upper one
mov ah,01h          ; repeat the above steps for lower nibble
int 21h
sub al,30h
cmp al,09h
jbe nxt1
sub al,07h
nxt1: add bl,al
ret                 ; return to main program
ipt endp

proc opt near
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
loop num1
opt endp
 
end