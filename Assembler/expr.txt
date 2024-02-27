
        IN
        CALL FIB
        OUT
        HLT

FIB:
        POP R8

        PUSH R8
        PUSH 0
        JE BASE0

        PUSH R8
        PUSH 1
        JE BASE1


        PUSH R8 ; R8 - 1ST ARG
        PUSH 1
        SUB
        POP R8
        PUSH R8 ; R8 - 1

        PUSH R8
        PUSH 1
        SUB
        POP R8
        PUSH R8 ; R8 - 2

        CALL FIB
        POP R9
        POP R10
        PUSH R9
        PUSH R10
        CALL FIB
        ADD
        RET

BASE0:
        PUSH 0
        RET

BASE1:
        PUSH 1
        RET
