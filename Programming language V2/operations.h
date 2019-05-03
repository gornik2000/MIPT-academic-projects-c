//---------------------------------------------------------------------------*/
/*
        Words that can be used:
      x  y  z
      pi  e  phi  ls
      +  -  /  *  ^
      sin  cos  tan  cot
      arcsin  arccos  arctan  arccot
      ln  ()

//---------------------------------------------------------------------------*/
/*      if you need optimization, add to optimization node function
        name - value - type - priority - input type - derivative function

      input types:
      1: a ? b
      2: ? a
      3: a ? : ?
      4: ?
      5: for
//---------------------------------------------------------------------------*/
/* operators */
DEF_OP ("=",        1,  OP, 1,
       {
         buf = nodeToBuf (n->child[1], buf);
         $(POP_AX );                                             // pop  ax
         $(MOV_R16); $(AX_1BP1); c(n->child[0]->key->value * 2); // mov  [bp + value], ax
       })

DEF_OP ("if",       2,  OP, 3,
       {
         buf = nodeToBuf (n->child[0], buf);
         /* remembering jmp position */
         unsigned char *outPos = buf - 1;
         for (int i = 1; i < n->childrenNum; i++)
         {
           buf = nodeToBuf (n->child[i], buf);
         }
         /* setting jmp length */
         *outPos = buf - outPos - 1;
       })

DEF_OP ("while",    5,  OP, 3,
       {
         /* remembering jmp position */
         unsigned char *startPos = buf;
         buf = nodeToBuf (n->child[0], buf);
         /* remembering jmp position */
         unsigned char *outPos   = buf - 1;
         for (int i = 1; i < n->childrenNum; i++)
         {
           buf = nodeToBuf (n->child[i], buf);
         }

         /* setting jmp length */
         $(JMP); c(256 - (buf - startPos));

         *outPos = buf - outPos - 1;
       })

DEF_OP ("def",      9,  OP, 4,
       {
       })

DEF_OP ("return",   10, OP, 2,
       {
         buf = nodeToBuf (n->child[0], buf);
         $(POP_AX );
         $(SUB_BP ); c(4    );
         $(R16_MOV); $(SP_BP);
         $(POP_BP );
       })

DEF_OP ("<",        11, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_CMP); $(AX_BX);
         $(JNB);
       })

DEF_OP (">",        12, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_CMP); $(AX_BX);
         $(JNA);
       })

DEF_OP ("==",       13, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_CMP); $(AX_BX);
         $(JNE);
       })

DEF_OP ("<=",       14, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_CMP); $(AX_BX);
         $(JA);
       })

DEF_OP (">=",       15, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_CMP); $(AX_BX);
         $(JB);
       })

DEF_OP ("+",        30, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_ADD); $(AX_BX);
         $(PUSH_AX);
       })

DEF_OP ("-",        31, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_SUB); $(AX_BX);
         $(PUSH_AX);
       })

DEF_OP ("!=",       32, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(R16_CMP); $(AX_BX);
         $(JE);
       })

DEF_OP ("^",        33, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_CX );
         $(DEC_BX );
         $(R16_MOV); $(AX_CX);
         $(MUL_CL );
         $(DEC_BX );
         $(JNE    );
         $(PUSH_AX);
         //$(POWER);
       })

DEF_OP ("*",        34, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(POP_BX );
         $(POP_AX );
         $(MUL_BX );
         $(PUSH_AX);
       })

DEF_OP ("/",        35, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(R16_XOR); $(DX_DX);
         $(POP_BX );
         $(POP_AX );
         $(DIV_BX );
         $(PUSH_AX);
       })

DEF_OP ("%",        36, OP, 1,
       {
         buf = nodeToBuf (n->child[0], buf);
         buf = nodeToBuf (n->child[1], buf);
         $(R16_XOR); $(DX_DX);
         $(POP_BX );
         $(POP_AX );
         $(DIV_BX );
         $(PUSH_DX);
       })

DEF_OP ("input",    42, OP, 5,
       {
         $(INPUT);
       })

DEF_OP ("print",    43, OP, 2,
       {
         buf = nodeToBuf (n->child[0], buf);
         $(OUTPUT);
       })

DEF_OP ("putc",     44, OP, 2,
       {
         buf = nodeToBuf (n->child[0], buf);
         $(PUTC);
       })

DEF_OP ("get_h",    45, OP, 5,
       {
         $(HOUR);
       })

DEF_OP ("get_m",    46, OP, 5,
       {
         $(MIN);
       })

DEF_OP ("get_s",    47, OP, 5,
       {
         $(SEC);
       })

DEF_OP ("get_ms",   48, OP, 5,
       {
         $(MSEC);
       })
//---------------------------------------------------------------------------*/
/* constants  */
DEF_OP ("pi",  3, CNST, 4, ;)
DEF_OP ("exp", 3, CNST, 4, ;)
DEF_OP ("phi", 2, CNST, 4, ;)
//---------------------------------------------------------------------------*/
//#undef D
//#undef NCC
//#undef NC
//#undef DC
//---------------------------------------------------------------------------*/
//            © Gorbachev Nikita, December 2018 + April 2019           //
//---------------------------------------------------------------------------*/
