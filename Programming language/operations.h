//-----------------------------------------------------------------------------
///
/// Words that can be used:
///   x  y  z
///   pi  e  phi  ls
///   +  -  /  *  ^
///   sin  cos  tan  cot
///   arcsin  arccos  arctan  arccot
///   ln  ()
///
//-----------------------------------------------------------------------------
/// if you need optimization, add to optimization node function */
/// name - value - type - priority - input type - derivative function*/
/*
    input types:
 1: a ? b
 2: ? a
 3: a ? : ?
 4: ?
 5: for
*/
//-----------------------------------------------------------------------------
/* variables  */
//-----------------------------------------------------------------------------
/* operators */
DEF_DIFF ("=",        1,  OP, 10, 1,
          {
            if (n->child[1]->key->type == OP && n->child[1]->key->value == 42)
            {
              fprintf (f, "push in\n");
            }
            else
            {
              subTreeToAsm (n->child[1], f);
            }

            fprintf (f, "pop +%.0f\n", n->child[0]->key->value);
          })

DEF_DIFF ("if",       2,  OP, 10, 3,
          {
            int flag = FLAG;
            FLAG++;

            fprintf (f, "\npush 0\n");
            subTreeToAsm (n->child[0], f);
            fprintf (f, "ja :%d\n", flag);

            for (int i = 1; i < n->childrenNum; i++)
            {
              subTreeToAsm (n->child[i], f);
            }

            fprintf (f, ":%d\n\n", flag); //*/
          })

DEF_DIFF ("else",     3,  OP, 10, 3,
          {
          })

DEF_DIFF ("elif",     4,  OP, 10, 3,
          {
          })

DEF_DIFF ("while",    5,  OP, 10, 3,
          {///
            int flag = FLAG;
            FLAG++;

            fprintf (f, ":start_%d\n", flag);

            fprintf (f, "\npush 0\n");
            subTreeToAsm (n->child[0], f);
            fprintf (f, "ja :end_%d\n", flag);

            for (int i = 1; i < n->childrenNum; i++)
            {
              subTreeToAsm (n->child[i], f);
            }

            fprintf (f, "jmp :start_%d\n", flag);
            fprintf (f, ":end_%d\n\n", flag); //*/
          })

DEF_DIFF ("for",      6,  OP, 10, 5,
          {
          })

DEF_DIFF ("break",    7,  OP, 10, 2,
          {
          })

DEF_DIFF ("continue", 8,  OP, 10, 5,
          {
          })

DEF_DIFF ("def",      9,  OP, 10, 4,
          {
          })

DEF_DIFF ("return",   10, OP, 10, 2,
          {
            subTreeToAsm (n->child[0], f);
            //fprintf (f,"ret\n");
          })

DEF_DIFF ("<",        11, OP, 10, 1,
          {
            subTreeToAsm (n->child[1], f);
            subTreeToAsm (n->child[0], f);
            fprintf (f, "smlr\n");
          })

DEF_DIFF (">",        12, OP, 10, 1,
          {
            subTreeToAsm (n->child[1], f);
            subTreeToAsm (n->child[0], f);
            fprintf (f, "bgr\n");
          })

DEF_DIFF ("==",       13, OP, 10, 1,
          {
            subTreeToAsm (n->child[0], f);
            subTreeToAsm (n->child[1], f);
            fprintf (f, "neql\n");
          })

DEF_DIFF ("<=",       14, OP, 10, 1,
          {
            subTreeToAsm (n->child[1], f);
            subTreeToAsm (n->child[0], f);
            fprintf (f, "sub\n");
          })

DEF_DIFF (">=",       15, OP, 10, 1,
          {
            subTreeToAsm (n->child[0], f);
            subTreeToAsm (n->child[1], f);
            fprintf (f, "sub\n");
          })

DEF_DIFF ("and",      16, OP, 10, 1,
          {
          })

DEF_DIFF ("or",       17, OP, 10, 1,
          {
          })

DEF_DIFF ("+",        30, OP, 10, 1,
          {
            subTreeToAsm (n->child[0], f);
            subTreeToAsm (n->child[1], f);
            fprintf (f, "add\n");
          })

DEF_DIFF ("-",        31, OP, 10, 1,
          {
            subTreeToAsm (n->child[0], f);
            subTreeToAsm (n->child[1], f);
            fprintf (f, "sub\n");
          })

DEF_DIFF ("!=",       32, OP, 10, 1,
          {
            subTreeToAsm (n->child[0], f);
            subTreeToAsm (n->child[1], f);
            fprintf (f, "eql\n");
          })

DEF_DIFF ("^",        33, OP, 10, 1,
          {
          })

DEF_DIFF ("*",        34, OP, 10, 1,
          {
            subTreeToAsm (n->child[0], f);
            subTreeToAsm (n->child[1], f);
            fprintf (f, "mul\n");
          })

DEF_DIFF ("/",        35, OP, 10, 1,
          {
            subTreeToAsm (n->child[0], f);
            subTreeToAsm (n->child[1], f);
            fprintf (f, "div\n");
          })

DEF_DIFF ("%",        36, OP, 10, 1,
          {
          })

DEF_DIFF ("//",       37, OP, 10, 1,
          {
          })

DEF_DIFF ("ln",       38, OP, 10, 2,
          {
            subTreeToAsm (n->child[0], f);
            fprintf (f, "in\n");
          })

DEF_DIFF ("sin",      39, OP, 10, 2,
          {
            subTreeToAsm (n->child[0], f);
            fprintf (f, "sin\n");
          })

DEF_DIFF ("cos",      40, OP, 10, 2,
          {
            subTreeToAsm (n->child[0], f);
            fprintf (f, "cos\n");
          })

DEF_DIFF ("tan",      41, OP, 10, 2,
          {
            subTreeToAsm (n->child[0], f);
            fprintf (f, "tan\n");
          })

DEF_DIFF ("input",    42, OP, 10, 5,
          {
            fprintf (f, "in\n");
          })

DEF_DIFF ("print",    43, OP, 10, 2,
          {
            subTreeToAsm (n->child[0], f);
            fprintf (f, "out\n");
            fprintf (f, "pop\n");
          })

DEF_DIFF ("sqrt",    44, OP, 10, 2,
          {
            subTreeToAsm (n->child[0], f);
            fprintf (f, "sqrt\n");
          })
//-----------------------------------------------------------------------------
/* constants  */
DEF_DIFF ("pi",  3.141592654, CNST, 255, 4, ;)
DEF_DIFF ("exp", 2.718281828, CNST, 255, 4, ;)
DEF_DIFF ("phi", 1.618033989, CNST, 255, 4, ;)
DEF_DIFF ("lc",    299792458, CNST, 255, 4, ;)
//-----------------------------------------------------------------------------
//#undef D
//#undef NCC
//#undef NC
//#undef DC
//-----------------------------------------------------------------------------
