
.model small                            ;.model directive is to specify len of our program
.486                                    ;80386 microprocessor directive library is installed           

.data                                   ;.data means we are using data segment register
MESS0 DB "           BCD ADDITION & SUBTRACTION PROGRAM$"
MESS1 DB "Enter 1st 8 digit BCD number: $"   ;message to be displayed on the screen to input 1st no.
MESS2 DB "Enter 2nd 8 digit BCD number: $"   ;message to be displayed on the screen to input 2nd no.
MESS30 DB "ADDTION OR SUBTRACTION (A/S) : $"
MESS31 DB "               Addition is: $"
MESS32 DB "               Difference is: $"   ;message to be displayed on the screen to display sum.
ERR DB "INVALID ENTRY!! Re-Enter the no. again $"
MESS4 DB "WANT MORE ARITHMETIC (Y/N) : $"
N1 DB 4 DUP(?)
N2 DB 4 DUP(?)
ARR DB 4 DUP(?)
.code                                   ; .code means our program code follows here 
.startup                                ; now the program starts

STRT: CALL LINE
MOV DX, OFFSET MESS0                    ;offset of mess1 passed in DX
CALL MSG  
CALL LINE
MOV DX, OFFSET MESS1                    ;offset of mess1 passed in DX
CALL MSG                                ;procedure to display message
MOV CL,04
MOV DI,0
RD1:CALL NUMB                               ;procedure to input 32bit no.
MOV N1[DI],BL
INC DI
DEC CL
JNZ RD1
                                
CALL LINE                               ;line break

;---------- SECOND NUMBER ------------

MOV DX, OFFSET MESS2                    ;offset of mess2 passed in DX
CALL MSG                                ;procedure to display message
MOV CL,04
MOV DI,0
MOV SI,DI
RD2:CALL NUMB                               ;procedure to input 32bit no.
MOV N2[DI],BL
INC DI
DEC CL
JNZ RD2                              
CALL LINE                               ;line break

MOV DX, OFFSET MESS30                   ;offset of mess30 passed in DX
CALL MSG                                ;procedure to display message
MOV AH,01
INT 21H

.IF AL=='A' || AL=='a'
CALL LINE
MOV DX, OFFSET MESS31                    ;offset of mess3 passed in DX
CALL MSG                                ;procedure to display message
DEC DI
MOV CX,04
CLC
L22: MOV AL,N1[DI]
    ADC AL,N2[DI]                               ;add two 32bit numbers
    DAA                             ;convert Hex to BCD
    MOV ARR[DI],AL
    DEC DI
LOOP L22
JNC JUMP8
MOV DL,01H
CALL OUTPUT
JMP JUMP8

.ELSE

;---------- SUBTRACTION CODE ------------
CALL LINE
MOV DX, OFFSET MESS32                    ;offset of mess3 passed in DX
CALL MSG                                ;procedure to display message
DEC DI
MOV CX,04
CHECK: MOV AL,N1[SI]
CMP AL,N2[SI]
JB SB
JA SB1
INC SI
LOOP CHECK
SB1: MOV CX,04
CLC
L2: MOV AL,N1[DI]
    SBB AL,N2[DI]                               ;add two 32bit numbers
    DAS                             ;convert hex to BCD
    MOV ARR[DI],AL
    DEC DI
LOOP L2
JMP JUMP8                               ;jump if sum is 32bit no.

SB: MOV CX,04
MOV DL,0C4H                             ;loading code for minus sign 
MOV AH,02                               ;display the minus sign
INT 21H
CLC
L9: MOV AL,N2[DI]
    SBB AL,N1[DI]                               ;add two 32bit numbers
    DAS                             ;store the result in EBX
    MOV ARR[DI],AL
    DEC DI
LOOP L9
;---------- OUTPUT RESULT -----------------
.ENDIF 
JUMP8: 
MOV CX,04
MOV DI,00
L7: MOV DL,ARR[DI]
    ROL DL,4
    AND DL,0FH
    CALL OUTPUT                   ;procedure to display sum
    MOV DL,ARR[DI]
    AND DL,0FH
    CALL OUTPUT
    INC DI 
LOOP L7

CALL LINE
MOV DX,OFFSET MESS4
MOV AH,09
INT 21H
MOV AH,01
INT 21H
.IF AL=='Y' || AL=='y'
JMP STRT
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
;---------- convert to ascii ---------
ADD DL,30H
MOV AH,02
INT 21H
RET
OUTPUT ENDP


NUMB PROC NEAR
;-------------- INPUT NUMBER -----------
XOR BL,BL
MOV CH,02
L1:     SHL BX,4
L11:    MOV AH,01
        INT 21H
        SUB AL,30H
        CMP AL,09
        JBE JUMP1
        CALL LINE
        MOV DX, OFFSET ERR
        CALL MSG
        JMP L11
JUMP1:  ADD BL,AL 
DEC CH
JNZ L1
RET
NUMB ENDP

end                                     ;end of program