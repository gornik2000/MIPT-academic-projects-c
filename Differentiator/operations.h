/* if you need optimization, add to optimization node function */
/* name - value - type - priority - arg number - derivative function*/
//-----------------------------------------------------------------------------
#define D(n)         nodeCreateDerivative (n)
#define C(n)         nodeCreateCopy (n)
#define NC(k, l, r)  nodeCreate (l, r, k)
#define DC(t, p, v)  ddataCreate(t, p, v)
//-----------------------------------------------------------------------------
/* variables  */
DEF_DIFF ("x", 'x', VAR, 255, 0,
          {
            dN = NC (DC (CNST, 255, 1),
                     NULL,
                     NULL
                     );
          })
DEF_DIFF ("y", 'y', VAR, 255, 0,
          {
            dN = NC (DC (CNST, 255, 1),
                     NULL,
                     NULL
                     );
          })

DEF_DIFF ("z", 'z', VAR, 255, 0,
          {
            dN = NC (DC (CNST, 255, 1),
                     NULL,
                     NULL
                     );
          })
//-----------------------------------------------------------------------------
/* operations */
DEF_DIFF ("+",      '+', OP, 30, 2,
          {
            /* node - +, right child - diff(right), left child - diff(left),
            right left child - -1, right right child - cos(copy) */
            dN = NC (DC (OP, 30, '+'),
                     D  (n->leftChild),
                     D  (n->rightChild)
                     );
          })
DEF_DIFF ("-",      '-', OP, 30, 2,
          {
            /* node - -, right child - diff(right), left child - diff(left),
            right left child - -1, right right child - cos(copy) */
            dN = NC (DC (OP, 30, '-'),
                     D  (n->leftChild),
                     D  (n->rightChild)
                     );
          })
DEF_DIFF ("*",      '*', OP, 20, 2,
          {
            dN = NC (DC (OP, 30, '+'),
                     NC (DC (OP, 20, '*'),
                         D  (n->leftChild),
                         C  (n->rightChild)
                         ),
                     NC (DC (OP, 20, '*'),
                         C  (n->leftChild),
                         D  (n->rightChild)
                         )
                     );
          })
DEF_DIFF ("/",      '/', OP, 20, 2,
          {
            dN = NC (DC (OP, 20, '/'),
                     NC (DC (OP, 30, '-'),
                         NC (DC (OP, 20, '*'),
                             D  (n->leftChild),
                             C  (n->rightChild)
                            ),
                         NC (DC (OP, 20, '*'),
                             C  (n->leftChild),
                             D  (n->rightChild)
                            )
                         ),
                     NC (DC (OP, 20, '*'),
                         C  (n->rightChild),
                         C  (n->rightChild)
                         )
                     );
          })
DEF_DIFF ("^",      '^', OP, 15, 2,
          {
            if (n->rightChild->key->type == CNST)
            {
              dN = NC (DC (OP, 20, '*'),
                       NC (DC (OP, 20, '*'),
                           C  (n->rightChild),
                           NC (DC (OP, 15, '^'),
                               C (n->leftChild),
                               NC (DC (CNST, 255, n->rightChild->key->value - 1),
                                   NULL,
                                   NULL
                                   )
                               )
                           ),
                       D (n->leftChild)
                       );
            }
            else
            {
              dN = NC (DC (OP, 20, '*'),
                       NC (DC (OP, 15, '^'),
                           C  (n->leftChild),
                           C  (n->rightChild)
                           ),
                       NC (DC (OP, 30, '+'),
                           NC (DC (OP, 20, '*'),
                               D  (n->rightChild),
                               NC (DC (OP, 10, 'l'),
                                   C  (n->leftChild),
                                   NULL
                                   )
                               ),
                           NC (DC (OP, 20, '*'),
                               D  (n->leftChild),
                               NC (DC (OP, 20, '/'),
                                   C  (n->rightChild),
                                   C  (n->leftChild)
                                   )
                               )
                           )
                       );
            }
          })
DEF_DIFF ("ln",     'l', OP, 10, 1,
          {
            dN = NC (DC (OP, 20, '*'),
                     D  (n->leftChild),
                     NC (DC (OP, 20, '/'),
                         NC (DC (CNST, 255, 1),
                             NULL,
                             NULL
                             ),
                         C  (n->leftChild)
                         )
                     );
          })
DEF_DIFF ("sin",    's', OP, 10, 1,
          {
            /* node - *, left - diff(), right - cos(copy) */
            dN = NC (DC (OP, 20, '*'),
                     D  (n->leftChild),
                     NC (DC (OP, 10, 'c'),
                         C  (n->leftChild),
                         NULL
                         )
                     );
          })
DEF_DIFF ("cos",    'c', OP, 10, 1,
          {
            /* node - *, right child - *, left child - diff(),
            right left child - -1, right right child - cos(copy) */
            dN = NC (DC (OP, 20, '*'),
                     D  (n->leftChild),
                     NC (DC (OP, 30, '-'),
                         NC (DC (CNST, 255, 0),
                             NULL,
                             NULL
                             ),
                         NC (DC (OP, 10, 's'),
                             C  (n->leftChild),
                             NULL
                             )
                         )
                     );
          })
DEF_DIFF ("tan",    't', OP, 10, 1,
          {
            dN = NC (DC (OP, 20, '*'),
                     D  (n->leftChild),
                     NC (DC (OP, 15, '^'),
                         NC (DC (OP, 10, 'c'),
                             C  (n->leftChild),
                             NULL
                             ),
                         NC (DC (CNST, 255, -2),
                             NULL,
                             NULL
                             )
                         )
                     );
          })
DEF_DIFF ("cot",    '1', OP, 10, 1,
          {
            dN = NC (DC (OP, 20, '*'),
                     D  (n->leftChild),
                     NC (DC (OP, 15, '^'),
                         NC (DC (OP, 10, 's'),
                             C  (n->leftChild),
                             NULL
                             ),
                         NC (DC (CNST, 255, -2),
                             NULL,
                             NULL
                             )
                         )
                     );
          })
DEF_DIFF ("arcsin", '2', OP, 10, 1,
          {
            dN = NC (DC (OP, 20, '*'),
                     D  (n->leftChild),
                     NC (DC (OP, 15, '^'),
                         NC (DC (OP, 30, '-'),
                             NC (DC (CNST, 255, 1),
                                 NULL,
                                 NULL
                                ),
                            NC (DC (OP, 15, '^'),
                                C (n->leftChild),
                                NC (DC (CNST, 255, 2),
                                    NULL,
                                    NULL
                                   )
                               )
                            ),
                         NC (DC (CNST, 255, -0.5),
                             NULL,
                             NULL
                             )
                         )
                     );
          })
DEF_DIFF ("arccos", '3', OP, 10, 1,
          {
            dN = NC (DC (OP, 20, '*'),
                     NC (DC (OP, 30, '-'),
                         NC (DC (CNST, 255, 0),
                             NULL,
                             NULL
                             ),
                         D  (n->leftChild)
                         ),
                     NC (DC (OP, 15, '^'),
                         NC (DC (OP, 30, '-'),
                             NC (DC (CNST, 255, 1),
                                 NULL,
                                 NULL
                                ),
                            NC (DC (OP, 15, '^'),
                                C (n->leftChild),
                                NC (DC (CNST, 255, 2),
                                    NULL,
                                    NULL
                                   )
                               )
                            ),
                         NC (DC (CNST, 255, -0.5),
                             NULL,
                             NULL
                             )
                         )
                     );
          })
DEF_DIFF ("arctan", '4', OP, 10, 1,
          {
            dN = NC (DC (OP, 20, '*'),
                     D  (n->leftChild),
                     NC (DC (OP, 15, '^'),
                         NC (DC (OP, 30, '+'),
                             NC (DC (CNST, 255, 1),
                                 NULL,
                                 NULL
                                ),
                            NC (DC (OP, 15, '^'),
                                C (n->leftChild),
                                NC (DC (CNST, 255, 2),
                                    NULL,
                                    NULL
                                   )
                               )
                            ),
                         NC (DC (CNST, 255, -1),
                             NULL,
                             NULL
                             )
                         )
                     );
          })
DEF_DIFF ("arccot", '5', OP, 10, 1,
          {
            dN = NC (DC (OP, 20, '*'),
                     NC (DC (OP, 30, '-'),
                         NC (DC (CNST, 255, 0),
                             NULL,
                             NULL
                             ),
                         D  (n->leftChild)
                         ),
                     NC (DC (OP, 15, '^'),
                         NC (DC (OP, 30, '+'),
                             NC (DC (CNST, 255, 1),
                                 NULL,
                                 NULL
                                ),
                            NC (DC (OP, 15, '^'),
                                C (n->leftChild),
                                NC (DC (CNST, 255, 2),
                                    NULL,
                                    NULL
                                   )
                               )
                            ),
                         NC (DC (CNST, 255, -1),
                             NULL,
                             NULL
                             )
                         )
                     );
          })
//-----------------------------------------------------------------------------
/* constants  */
DEF_DIFF ("pi",  3.141592654, CNST, 255, 0, ;)
DEF_DIFF ("e",   2.718281828, CNST, 255, 0, ;)
DEF_DIFF ("phi", 1.618033989, CNST, 255, 0, ;)
DEF_DIFF ("c",   299792458,   CNST, 255, 0, ;)
//-----------------------------------------------------------------------------
#undef D
#undef NCC
#undef NC
#undef DC
//-----------------------------------------------------------------------------
