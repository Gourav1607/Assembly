.MODEL SMALL
.486
.STACK 30H
.DATA
MSG01 DB "HEX TO DECIMAL CONVERSION $"
.CODE
.STARTUP
MOV AH,01H
INT 21H
MOV BL,AL
MOV DL,' '
MOV AH,02
INT 21H
AND AL,0F0H
SHR AL,4
MOV DL,AL
CALL OUTPUT
MOV AL,BL
AND AL,0FH
MOV DL,AL
CALL OUTPUT
MOV DL,' '
MOV AH,02
INT 21H
XOR AX,AX
MOV AL,BL
MOV CL,64H
DIV CL
MOV BL,AH
MOV DL,AL
CALL OUTPUT
XOR AX,AX
MOV AL,BL
MOV CL,0AH
DIV CL
MOV BL,AH
MOV DL,AL
CALL OUTPUT
MOV DL,BL
CALL OUTPUT
.EXIT

OUTPUT PROC NEAR
;---------- CONVERT TO ASCII ---------
CMP DL,09                               
JBE JUMP10
ADD DL,07H
JUMP10: ADD DL,30H
MOV AH,02
INT 21H
RET
OUTPUT ENDP

END

