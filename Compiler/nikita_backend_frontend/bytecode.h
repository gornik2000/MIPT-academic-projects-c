//---------------------------------------------------------------------------*/
//                My function codes                                          //
//---------------------------------------------------------------------------*/
DEF_BC (EXIT,    5, 0xB8, 0x00, 0x4C, 0xCD, 0x21)

DEF_BC (ENTER,   3, 0x55, 0x8B, 0xEC)

DEF_BC (LEAVE,   3, 0x8B, 0xE5, 0x5D)

DEF_BC (POWER,  11, 0x5B, 0x59, 0x4B, 0x8B, 0xC1, 0xF6, 0xE1, 0x4B, 0x75, 0xFB, \
                    0x50)

DEF_BC (INPUT,  27, 0x33, 0xC9, 0xB3, 0x0A, 0xB4, 0x01, 0xCD, 0x21, 0x3C, 0x0D, \
                    0x74, 0x0E, 0x2C, 0x30, 0x8A, 0xD0, 0x8B, 0xC1, 0xF6, 0xE3, \
                    0x8B, 0xC8, 0x02, 0xCA, 0xEB, 0xEA, 0x51)

DEF_BC (REVOUT, 30, 0x58, 0xB5, 0x0A, 0xF6, 0xF5, 0x80, 0xC4, 0x30, 0x8A, 0xD4, \
                    0x8A, 0xC8, 0xB4, 0x02, 0xCD, 0x21, 0x8A, 0xC1, 0x32, 0xE4, \
                    0x3C, 0x00, 0x75, 0xEB, 0xB2, 0x0A, 0xB4, 0x02, 0xCD, 0x21)

DEF_BC (OUTPUT, 34, 0x58, 0xB5, 0x0A, 0x33, 0xDB, 0x33, 0xD2, 0xF6, 0xF5, 0x80,
                    0xC4, 0x30, 0x8A, 0xD4, 0x32, 0xE4, 0x52, 0x43, 0x3C, 0x00,
                    0x75, 0xF1, 0xB4, 0x02, 0x5A, 0xCD, 0x21, 0x4B, 0x75, 0xFA,
                    0xB2, 0x0A, 0xCD, 0x21)

DEF_BC (PUTC,    5, 0xB4, 0x02, 0x5A, 0xCD, 0x21)

DEF_BC (HOUR,    9, 0xB4, 0x2C, 0xCD, 0x21, 0x32, 0xC9, 0x86, 0xCD, 0x51)

DEF_BC (MIN,     7, 0xB4, 0x2C, 0xCD, 0x21, 0x32, 0xED, 0x51)

DEF_BC (SEC,     9, 0xB4, 0x2C, 0xCD, 0x21, 0x32, 0xD2, 0x86, 0xD6, 0x52)

DEF_BC (MSEC,    7, 0xB4, 0x2C, 0xCD, 0x21, 0x32, 0xF6, 0x52)
//---------------------------------------------------------------------------*/
//                Operation codes                                            //
//---------------------------------------------------------------------------*/
DEF_BC (ADD_R8,  1, 0x00)
DEF_BC (ADD_R16, 1, 0x01)
DEF_BC (R8_ADD,  1, 0x02)
DEF_BC (R16_ADD, 1, 0x03)
//---------------------------------------------------------------------------*/
DEF_BC (OR_R8,   1, 0x08)
DEF_BC (OR_R16,  1, 0x09)
DEF_BC (R8_OR,   1, 0x0A)
DEF_BC (R16_OR,  1, 0x0B)
//---------------------------------------------------------------------------*/
DEF_BC (AND_R8,  1, 0x20)
DEF_BC (AND_R16, 1, 0x21)
DEF_BC (R8_AND,  1, 0x22)
DEF_BC (R16_AND, 1, 0x23)
//---------------------------------------------------------------------------*/
DEF_BC (SUB_R8,  1, 0x28)
DEF_BC (SUB_R16, 1, 0x29)
DEF_BC (R8_SUB,  1, 0x2A)
DEF_BC (R16_SUB, 1, 0x2B)
//---------------------------------------------------------------------------*/
DEF_BC (XOR_R8,  1, 0x30)
DEF_BC (XOR_R16, 1, 0x31)
DEF_BC (R8_XOR,  1, 0x32)
DEF_BC (R16_XOR, 1, 0x33)
//---------------------------------------------------------------------------*/
DEF_BC (CMP_R8,  1, 0x38)
DEF_BC (CMP_R16, 1, 0x39)
DEF_BC (R8_CMP,  1, 0x3A)
DEF_BC (R16_CMP, 1, 0x3B)
//---------------------------------------------------------------------------*/
DEF_BC (INC_AX,  1, 0x40)
DEF_BC (INC_CX,  1, 0x41)
DEF_BC (INC_DX,  1, 0x42)
DEF_BC (INC_BX,  1, 0x43)
DEF_BC (INC_SP,  1, 0x44)
DEF_BC (INC_BP,  1, 0x45)
DEF_BC (INC_SI,  1, 0x46)
DEF_BC (INC_DI,  1, 0x47)
//---------------------------------------------------------------------------*/
DEF_BC (DEC_AX,  1, 0x48)
DEF_BC (DEC_CX,  1, 0x49)
DEF_BC (DEC_DX,  1, 0x4A)
DEF_BC (DEC_BX,  1, 0x4B)
DEF_BC (DEC_SP,  1, 0x4C)
DEF_BC (DEC_BP,  1, 0x4D)
DEF_BC (DEC_SI,  1, 0x4E)
DEF_BC (DEC_DI,  1, 0x4F)
//---------------------------------------------------------------------------*/
DEF_BC (PUSH_AX, 1, 0x50)
DEF_BC (PUSH_CX, 1, 0x51)
DEF_BC (PUSH_DX, 1, 0x52)
DEF_BC (PUSH_BX, 1, 0x53)
DEF_BC (PUSH_SP, 1, 0x54)
DEF_BC (PUSH_BP, 1, 0x55)
DEF_BC (PUSH_SI, 1, 0x56)
DEF_BC (PUSH_DI, 1, 0x57)
//---------------------------------------------------------------------------*/
DEF_BC (POP_AX,  1, 0x58)
DEF_BC (POP_CX,  1, 0x59)
DEF_BC (POP_DX,  1, 0x5A)
DEF_BC (POP_BX,  1, 0x5B)
DEF_BC (POP_SP,  1, 0x5C)
DEF_BC (POP_BP,  1, 0x5D)
DEF_BC (POP_SI,  1, 0x5E)
DEF_BC (POP_DI,  1, 0x5F)
//---------------------------------------------------------------------------*/
/* 2 assembler commands for further inserting of jump position */
DEF_BC (JB,      2, 0x72, 0x00)
DEF_BC (JNB,     2, 0x73, 0x00)
DEF_BC (JE,      2, 0x74, 0x00)
DEF_BC (JNE,     2, 0x75, 0x00)
DEF_BC (JNA,     2, 0x76, 0x00)
DEF_BC (JA,      2, 0x77, 0x00)
//---------------------------------------------------------------------------*/
DEF_BC (CALL,    1, 0xE8)
DEF_BC (LJMP,    1, 0xE9)
DEF_BC (JMP,     1, 0xEB)
//---------------------------------------------------------------------------*/
DEF_BC (ADD_AX,  2, 0x83, 0xC0)
DEF_BC (ADD_CX,  2, 0x83, 0xC1)
DEF_BC (ADD_DX,  2, 0x83, 0xC2)
DEF_BC (ADD_BX,  2, 0x83, 0xC3)
DEF_BC (ADD_SP,  2, 0x83, 0xC4)
DEF_BC (ADD_BP,  2, 0x83, 0xC5)
DEF_BC (ADD_SI,  2, 0x83, 0xC6)
DEF_BC (ADD_DI,  2, 0x83, 0xC7)

DEF_BC (ADDD_AX, 2, 0x81, 0xC0)
DEF_BC (ADDD_CX, 2, 0x81, 0xC1)
DEF_BC (ADDD_DX, 2, 0x81, 0xC2)
DEF_BC (ADDD_BX, 2, 0x81, 0xC3)
DEF_BC (ADDD_SP, 2, 0x81, 0xC4)
DEF_BC (ADDD_BP, 2, 0x81, 0xC5)
DEF_BC (ADDD_SI, 2, 0x81, 0xC6)
DEF_BC (ADDD_DI, 2, 0x81, 0xC7)
//---------------------------------------------------------------------------*/
DEF_BC (SUB_AX,  2, 0x83, 0xE8)
DEF_BC (SUB_CX,  2, 0x83, 0xE9)
DEF_BC (SUB_DX,  2, 0x83, 0xEA)
DEF_BC (SUB_BX,  2, 0x83, 0xEB)
DEF_BC (SUB_SP,  2, 0x83, 0xEC)
DEF_BC (SUB_BP,  2, 0x83, 0xED)
DEF_BC (SUB_SI,  2, 0x83, 0xEE)
DEF_BC (SUB_DI,  2, 0x83, 0xEF)

DEF_BC (SUBD_AX, 2, 0x81, 0xE8)
DEF_BC (SUBD_CX, 2, 0x81, 0xE9)
DEF_BC (SUBD_DX, 2, 0x81, 0xEA)
DEF_BC (SUBD_BX, 2, 0x81, 0xEB)
DEF_BC (SUBD_SP, 2, 0x81, 0xEC)
DEF_BC (SUBD_BP, 2, 0x81, 0xED)
DEF_BC (SUBD_SI, 2, 0x81, 0xEE)
DEF_BC (SUBD_DI, 2, 0x81, 0xEF)
//---------------------------------------------------------------------------*/
DEF_BC (MOV_R8,  1, 0x88)
DEF_BC (MOV_R16, 1, 0x89)
DEF_BC (R8_MOV,  1, 0x8A)
DEF_BC (R16_MOV, 1, 0x8B)
//---------------------------------------------------------------------------*/
DEF_BC (NOP,     1, 0x90)
DEF_BC (XCHG_CX, 1, 0x91)
DEF_BC (XCHG_DX, 1, 0x92)
DEF_BC (XCHG_BX, 1, 0x93)
DEF_BC (XCHG_SP, 1, 0x94)
DEF_BC (XCHG_BP, 1, 0x95)
DEF_BC (XCHG_SI, 1, 0x96)
DEF_BC (XCHG_DI, 1, 0x97)
//---------------------------------------------------------------------------*/
DEF_BC (MOV_AL,  1, 0xB0)
DEF_BC (MOV_CL,  1, 0xB1)
DEF_BC (MOV_DL,  1, 0xB2)
DEF_BC (MOV_BL,  1, 0xB3)
DEF_BC (MOV_AH,  1, 0xB4)
DEF_BC (MOV_CH,  1, 0xB5)
DEF_BC (MOV_DH,  1, 0xB6)
DEF_BC (MOV_BH,  1, 0xB7)

DEF_BC (MOV_AX,  1, 0xB8)
DEF_BC (MOV_CX,  1, 0xB9)
DEF_BC (MOV_DX,  1, 0xBA)
DEF_BC (MOV_BX,  1, 0xBB)
DEF_BC (MOV_SP,  1, 0xBC)
DEF_BC (MOV_BP,  1, 0xBD)
DEF_BC (MOV_SI,  1, 0xBE)
DEF_BC (MOV_DI,  1, 0xBF)
//---------------------------------------------------------------------------*/
DEF_BC (RET,     1, 0xC3)
DEF_BC (INT,     1, 0xCD)
//---------------------------------------------------------------------------*/
//                Registers combinations codes                               //
//---------------------------------------------------------------------------*/
/* AX AX and so on */
DEF_BC (AX_AX,   1, 0xC0)
DEF_BC (AX_CX,   1, 0xC1)
DEF_BC (AX_DX,   1, 0xC2)
DEF_BC (AX_BX,   1, 0xC3)
DEF_BC (AX_SP,   1, 0xC4)
DEF_BC (AX_BP,   1, 0xC5)
DEF_BC (AX_SI,   1, 0xC6)
DEF_BC (AX_DI,   1, 0xC7)

DEF_BC (CX_AX,   1, 0xC8)
DEF_BC (CX_CX,   1, 0xC9)
DEF_BC (CX_DX,   1, 0xCA)
DEF_BC (CX_BX,   1, 0xCB)
DEF_BC (CX_SP,   1, 0xCC)
DEF_BC (CX_BP,   1, 0xCD)
DEF_BC (CX_SI,   1, 0xCE)
DEF_BC (CX_DI,   1, 0xCF)

DEF_BC (DX_AX,   1, 0xD0)
DEF_BC (DX_CX,   1, 0xD1)
DEF_BC (DX_DX,   1, 0xD2)
DEF_BC (DX_BX,   1, 0xD3)
DEF_BC (DX_SP,   1, 0xD4)
DEF_BC (DX_BP,   1, 0xD5)
DEF_BC (DX_SI,   1, 0xD6)
DEF_BC (DX_DI,   1, 0xD7)

DEF_BC (BX_AX,   1, 0xD8)
DEF_BC (BX_CX,   1, 0xD9)
DEF_BC (BX_DX,   1, 0xDA)
DEF_BC (BX_BX,   1, 0xDB)
DEF_BC (BX_SP,   1, 0xDC)
DEF_BC (BX_BP,   1, 0xDD)
DEF_BC (BX_SI,   1, 0xDE)
DEF_BC (BX_DI,   1, 0xDF)

DEF_BC (SP_AX,   1, 0xE0)
DEF_BC (SP_CX,   1, 0xE1)
DEF_BC (SP_DX,   1, 0xE2)
DEF_BC (SP_BX,   1, 0xE3)
DEF_BC (SP_SP,   1, 0xE4)
DEF_BC (SP_BP,   1, 0xE5)
DEF_BC (SP_SI,   1, 0xE6)
DEF_BC (SP_DI,   1, 0xE7)

DEF_BC (BP_AX,   1, 0xE8)
DEF_BC (BP_CX,   1, 0xE9)
DEF_BC (BP_DX,   1, 0xEA)
DEF_BC (BP_BX,   1, 0xEB)
DEF_BC (BP_SP,   1, 0xEC)
DEF_BC (BP_BP,   1, 0xED)
DEF_BC (BP_SI,   1, 0xEE)
DEF_BC (BP_DI,   1, 0xEF)

DEF_BC (SI_AX,   1, 0xF0)
DEF_BC (SI_CX,   1, 0xF1)
DEF_BC (SI_DX,   1, 0xF2)
DEF_BC (SI_BX,   1, 0xF3)
DEF_BC (SI_SP,   1, 0xF4)
DEF_BC (SI_BP,   1, 0xF5)
DEF_BC (SI_SI,   1, 0xF6)
DEF_BC (SI_DI,   1, 0xF7)

DEF_BC (DI_AX,   1, 0xF8)
DEF_BC (DI_CX,   1, 0xF9)
DEF_BC (DI_DX,   1, 0xFA)
DEF_BC (DI_BX,   1, 0xFB)
DEF_BC (DI_SP,   1, 0xFC)
DEF_BC (DI_BP,   1, 0xFD)
DEF_BC (DI_SI,   1, 0xFE)
DEF_BC (DI_DI,   1, 0xFF)
//---------------------------------------------------------------------------*/
DEF_BC (MUL_AX,  2, 0xF7, 0xE0)
DEF_BC (MUL_CX,  2, 0xF7, 0xE1)
DEF_BC (MUL_DX,  2, 0xF7, 0xE2)
DEF_BC (MUL_BX,  2, 0xF7, 0xE3)
DEF_BC (MUL_SP,  2, 0xF7, 0xE4)
DEF_BC (MUL_BP,  2, 0xF7, 0xE5)
DEF_BC (MUL_SI,  2, 0xF7, 0xE6)
DEF_BC (MUL_DI,  2, 0xF7, 0xE7)

DEF_BC (MUL_AL,  2, 0xF6, 0xE0)
DEF_BC (MUL_CL,  2, 0xF6, 0xE1)
DEF_BC (MUL_DL,  2, 0xF6, 0xE2)
DEF_BC (MUL_BL,  2, 0xF6, 0xE3)

DEF_BC (MUL_AH,  2, 0xF6, 0xE4)
DEF_BC (MUL_CH,  2, 0xF6, 0xE5)
DEF_BC (MUL_DH,  2, 0xF6, 0xE6)
DEF_BC (MUL_BH,  2, 0xF6, 0xE7)
//---------------------------------------------------------------------------*/
DEF_BC (DIV_AX,  2, 0xF7, 0xF0)
DEF_BC (DIV_CX,  2, 0xF7, 0xF1)
DEF_BC (DIV_DX,  2, 0xF7, 0xF2)
DEF_BC (DIV_BX,  2, 0xF7, 0xF3)
DEF_BC (DIV_SP,  2, 0xF7, 0xF4)
DEF_BC (DIV_BP,  2, 0xF7, 0xF5)
DEF_BC (DIV_SI,  2, 0xF7, 0xF6)
DEF_BC (DIV_DI,  2, 0xF7, 0xF7)

DEF_BC (DIV_AL,  2, 0xF6, 0xF0)
DEF_BC (DIV_CL,  2, 0xF6, 0xF1)
DEF_BC (DIV_DL,  2, 0xF6, 0xF2)
DEF_BC (DIV_BL,  2, 0xF6, 0xF3)

DEF_BC (DIV_AH,  2, 0xF6, 0xF4)
DEF_BC (DIV_CH,  2, 0xF6, 0xF5)
DEF_BC (DIV_DH,  2, 0xF6, 0xF6)
DEF_BC (DIV_BH,  2, 0xF6, 0xF7)
//---------------------------------------------------------------------------*/
/* AX [BX] and so on */
DEF_BC (AX_1SI1, 1, 0x44)
DEF_BC (AX_1DI1, 1, 0x45)
DEF_BC (AX_1BP1, 1, 0x46)
DEF_BC (AX_1BX1, 1, 0x47)
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, April 2019                              //
//---------------------------------------------------------------------------*/
