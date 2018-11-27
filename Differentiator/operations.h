/* if you need optimization, add to optimization node function */
/* name - value - type - priority - derivative function*/
//-----------------------------------------------------------------------------
/* variables  */
DEF_DIFF ("x", 'x', VAR, 255,
          {
            dN->key = ddataCreate (CNST, 255, 1);
          })
//-----------------------------------------------------------------------------
/* operations */
DEF_DIFF ("+",      '+', OP, 3,
          {
            /* node - +, right child - diff(right), left child - diff(left),
            right left child - -1, right right child - cos(copy) */

            dN->key = ddataCreate (OP, 3, '+');

            dN->leftChild  = nodeCtor ();
            dN->leftChild  = nodeDerivative (n->leftChild,  dN->leftChild);

            dN->rightChild = nodeCtor ();
            dN->rightChild = nodeDerivative (n->rightChild, dN->rightChild);
          })
DEF_DIFF ("-",      '-', OP, 3,
          {
            /* node - -, right child - diff(right), left child - diff(left),
            right left child - -1, right right child - cos(copy) */

            dN->key = ddataCreate (OP, 3, '-');

            dN->leftChild  = nodeCtor ();
            dN->leftChild  = nodeDerivative (n->leftChild,  dN->leftChild);

            dN->rightChild = nodeCtor ();
            dN->rightChild = nodeDerivative (n->rightChild, dN->rightChild);
          })
DEF_DIFF ("*",      '*', OP, 2,
          {
          })
DEF_DIFF ("/",      '/', OP, 2,
          {
          })
DEF_DIFF ("^",      '^', OP, 2,
          {
          })
DEF_DIFF ("ln",     'l', OP, 1,
          {
          })
DEF_DIFF ("sin",    's', OP, 1,
          {
            /* node - *, left - diff(), right - cos(copy) */

            dN->key = ddataCreate (OP, 2, '*');

            dN->leftChild = nodeCtor ();
            dN->leftChild = nodeDerivative (n->leftChild, dN->leftChild);

            dN->rightChild      = nodeCtor ();
            dN->rightChild->key = ddataCreate (OP, 1, 'c');

            dN->rightChild->leftChild  = nodeCreateCopy (n->leftChild);
          })
DEF_DIFF ("cos",    'c', OP, 1,
          {
            /* node - *, right child - *, left child - diff(),
            right left child - -1, right right child - cos(copy) */
            dN->key = ddataCreate (OP, 2, '*');

            dN->leftChild    = nodeCtor ();
            dN->leftChild    = nodeDerivative (n->leftChild, dN->leftChild);

            dN->rightChild      = nodeCtor ();
            dN->rightChild->key = ddataCreate (OP, 2, '*');

            dN->rightChild->leftChild       = nodeCtor ();
            dN->rightChild->leftChild->key  = ddataCreate (CNST, 255, -1);

            dN->rightChild->rightChild      = nodeCtor ();
            dN->rightChild->rightChild->key = ddataCreate (OP, 1, 's');

            dN->rightChild->rightChild->leftChild  = nodeCreateCopy (n->leftChild);
          })
DEF_DIFF ("tan",    't', OP, 1,
          {
          })
DEF_DIFF ("cot",    '1', OP, 1,
          {
          })
DEF_DIFF ("arcsin", '2', OP, 1,
          {
          })
DEF_DIFF ("arccos", '3', OP, 1,
          {
          })
DEF_DIFF ("arccot", '4', OP, 1,
          {
          })
DEF_DIFF ("arcctg", '5', OP, 1,
          {
          })
//-----------------------------------------------------------------------------
/* constants  */
DEF_DIFF ("pi", 3.141592654, CNST, 255, ;)
DEF_DIFF ("e",  2.718281828, CNST, 255, ;)
