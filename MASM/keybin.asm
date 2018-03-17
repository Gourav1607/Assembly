.model small
.486
.stack 20h
.data
msg db " press a key $"
msg1 db "the binary code of key pressed is $"
.code
.startup
start: mov dl,0ah       ; loading dl with data for line break
mov ah,02h              ; load control word to set dos as oputput port
int 21h                 ; call dos interrupt program
mov dx,offset msg       ; load offest of msg
mov ah,09h              ; load control word to print string
int 21h                 ; dos interrupt
mov ah,01h              ; load control word to set dos as input port
int 21h                 ; dos interrupt
mov bh,al               ; store input data in bl reg.
mov dl,0ah              ; line break
mov ah,02h
int 21h
mov dx,offset msg1      ; print custom output msg
mov ah,09h
int 21h
mov cx,08h              ; load counter with 8 for mapping byte
label0: mov bl,bh       ; load the input data in bl
test bl,80h             ; mask the MSB ANDing with 80H
jnz label1              ; jump if zero flag is reset
mov dl,30h              ; load dl with zero code
mov ah,02h              ; publish zero on dos screen
int 21h
jmp label2              ; unconditional jump to label2
label1: mov dl,31h      ; load dl with one code
mov ah,02h              ; publish zero on dos screen
int 21h
label2: rol bh,1        ; rotate the contents of bh left side
loop label0             ; loop as per counter cx
cmp bh,'$'              ; $ is assigned to terminate the program
jnz start
.exit
end