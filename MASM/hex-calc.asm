.model TINY                            ;.model directive is to specify len of our program
.486                                    ;80486 microprocessor directive library is installed           

.data                                   ;.data means we are using data segment register
MESS1 DB "Enter 1st 32 bit number: $"   ;message to be displayed on the screen to input 1st no.
MESS2 DB "Enter 2nd 32 bit number: $"   ;message to be displayed on the screen to input 2nd no.
PRD DB "Product is: $"   ;message to be displayed on the screen to display product.
QUT DB "Quotient is: $"   ;message to be displayed on the screen to display Quotient.
RMN DB "Remainder is: $"   ;message to be displayed on the screen to display Remainder.
SUM DB "Sum is: $"   ;message to be displayed on the screen to display sum.
DIFF DB "Difference is: $"   ;message to be displayed on the screen to display sum.
MN0 DB "        HEXADECIMAL CALCULATOR$"
MN1 DB "            1: ADDITION$"
MN2 DB "            2: SUBTRACTION$"
MN3 DB "            3: MULTIPLICATION$"
MN4 DB "            4: DIVISION$"
MN5 DB "            0: EXIT$"
MN6 DB "ENTER CHOICE:$"
MN7 DB "MORE CALCULATIONS (Y/N): $"

.code                                   ; .code means our program code follows here 
.startup                                ; now the program starts

;---------- FIRST NUMBER ------------
GO: MOV DX, OFFSET MESS1                ;offset of mess1 passed in DX
CALL MSG                                ;procedure to display message
CALL NUMB                               ;procedure to input 32bit no.
PUSH EBX                                ;storing the number in stack
CALL LINE                               ;line break

;---------- SECOND NUMBER ------------

MOV DX, OFFSET MESS2                    ;offset of mess2 passed in DX
CALL MSG                                ;procedure to display message
CALL NUMB                               ;procedure to input 32bit no.
PUSH EBX                                ;storing the number in stack
CALL LINE                               ;line break

;---------- MENU ------------

MOV DX, OFFSET MN0
CALL MSG
CALL LINE
MOV DX, OFFSET MN1
CALL MSG
CALL LINE
MOV DX, OFFSET MN2
CALL MSG
CALL LINE
MOV DX, OFFSET MN3
CALL MSG
CALL LINE
MOV DX, OFFSET MN4
CALL MSG
CALL LINE
MOV DX, OFFSET MN5
CALL MSG
CALL LINE
MOV DX, OFFSET MN6
CALL MSG
MOV AH,01
INT 21H
    .IF AL=='1'
      CALL LINE
      MOV DX, OFFSET SUM                    ;offset of mess3 passed in DX
      CALL MSG                                ;procedure to display messagePOP EDX
      POP EBX                                 ;retrive 2nd number
      POP EDX                                 ;retrive 1st number
      ADD EBX,EDX                             ;add two 32bit numbers
      ;MOV EBX,EDX                             ;store the result in EBX
      JNC JUMP8                               ;jump if sum is 32bit no.
      XOR EDX,EDX                             ;clear EDX
      MOV DL,31H                              ;copy carry in DL if sum is > 32bit no.
      MOV AH,02H                            ;to display carry
      INT 21H                                   
      JUMP8: CALL OUTPUT

    .ELSEIF AL=='2'
      CALL LINE
      MOV DX, OFFSET DIFF                    ;offset of mess3 passed in DX
      CALL MSG                                ;procedure to display messagePOP EDX
      POP ECX                                 ;retrive 2nd number
      POP EBX                                 ;retrive 1st number
      CMP EBX,ECX                             ;compare 1st & 2nd number
      JA JUMP9                               ;if EDX > EBX then jump
      XCHG ECX,EBX                             ;swaping of EDX & EBX
      MOV DL,0C4H                             ;loading code for minus sign sign
      MOV AH,02H                               ;display the sign
      INT 21H
      JUMP9:SUB EBX,ECX                       ;subtract two numbers
      CALL OUTPUT

    .ELSEIF AL=='3'
      CALL LINE
      MOV DX, OFFSET PRD                    ;offset of mess3 passed in DX
      CALL MSG                                ;procedure to display messagePOP EDX
      POP EAX                                 ;retrive 2nd number
      POP EBX                                 ;retrive 1st number
      MUL EBX
      PUSH EAX
      MOV EBX,EDX
      CALL OUTPUT
      POP EBX
      CALL OUTPUT

    .ELSEIF AL=='4'
      CALL LINE
      MOV DX, OFFSET QUT                    ;offset of mess3 passed in DX
      CALL MSG                                ;procedure to display message
      POP EBX                                 ;retrive 2nd number
      POP EAX                                 ;retrive 1st number
      MOV EDX,00H
      DIV EBX
      PUSH EDX
      MOV EBX,EAX
      CALL OUTPUT
      CALL LINE
      MOV DX, OFFSET RMN                    ;offset of mess3 passed in DX
      CALL MSG                                ;procedure to display message
      POP EBX
      CALL OUTPUT
    .ENDIF

CALL LINE
MOV DX, OFFSET MN7
CALL MSG
MOV AH,01
INT 21H
    .IF AL=='Y' || AL=='y'
      CALL LINE
      CALL LINE
      JMP GO
    .ENDIF
;------------ THE END --------------
.exit                                   ;exit to DOS

LINE PROC NEAR
;---------- NEW LINE -------------
MOV DL,0AH
MOV AH,02
INT 21H
RET
LINE ENDP

MSG PROC NEAR
;---------- TYPE MESSAGE -------------
MOV AH,09H
INT 21h
RET
MSG ENDP

OUTPUT PROC NEAR
MOV CX,8                               ;counter for nibble shift
JUMP14: ROL EBX,4
MOV EDX,EBX                     ;retrive product
AND EDX,0FH                             ;select nibble
ADD DL,30H
CMP DL,39H
JBE NEW
    ADD DL,07H
NEW: MOV AH,02H
    INT 21H
LOOP JUMP14
RET
OUTPUT ENDP

NUMB PROC NEAR
;-------------- INPUT NUMBER -----------
XOR EBX,EBX
XOR EDX,EDX
MOV CX,8
JUMP0: SHL EBX,4
MOV AH,01
INT 21H
SUB AL,30H
CMP AL,09
JBE JUMP1
SUB AL,07H
CMP AL,0FH
JBE JUMP1
SUB AL,20H
JUMP1: ADD BL,AL
LOOP JUMP0
RET
NUMB ENDP

end                                     ;end of program