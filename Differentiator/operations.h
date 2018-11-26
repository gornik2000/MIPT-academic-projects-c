/* if you need optimization, add to optimization node function */
/* name - value - type - priority */
//-----------------------------------------------------------------------------
/* variables  */
DEF_DIFF ("x", 'x', VAR, 255)
//-----------------------------------------------------------------------------
/* operations */
DEF_DIFF ("+",    '+', OP, 30)
DEF_DIFF ("-",    '-', OP, 30)
DEF_DIFF ("*",    '*', OP, 20)
DEF_DIFF ("/",    '/', OP, 20)
DEF_DIFF ("^",    '^', OP, 20)
DEF_DIFF ("ln",   'l', OP, 10)
DEF_DIFF ("sin",  's', OP, 10)
DEF_DIFF ("cos",  'c', OP, 10)
DEF_DIFF ("tan",  't', OP, 10)
DEF_DIFF ("cot",  '1', OP, 10)
DEF_DIFF ("arcsin", '2', OP, 10)
DEF_DIFF ("arccos", '3', OP, 10)
DEF_DIFF ("arccot", '4', OP, 10)
DEF_DIFF ("arcctg", '5', OP, 10)
//-----------------------------------------------------------------------------
/* constants  */
DEF_DIFF ("pi", 3.141592654, CNST, 255)
DEF_DIFF ("e",  2.718281828, CNST, 255)
