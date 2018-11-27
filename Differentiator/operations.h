/* if you need optimization, add to optimization node function */
/* name - value - type - priority - derivative function*/
//-----------------------------------------------------------------------------
/* variables  */
DEF_DIFF ("x", 'x', VAR, 255,
          {
            //printf ("\ndiff x\n");
            //printf ("dN %p\n", dN);
            dN->key->type     = CNST;
            dN->key->value    = 1;
            dN->key->priority = 255;
            //printf ("\ndiff x\n");
          })
//-----------------------------------------------------------------------------
/* operations */
DEF_DIFF ("+",      '+', OP, 3,
          {
            /* node - +, right child - diff(right), left child - diff(left),
            right left child - -1, right right child - cos(copy) */
            dN->key->type     = OP;
            dN->key->value    = '+';
            dN->key->priority = 3;

            dN->leftChild  = nodeCtor ();
            dN->leftChild  = nodeDerivative (n->leftChild,  dN->leftChild);

            dN->rightChild = nodeCtor ();
            dN->rightChild = nodeDerivative (n->rightChild, dN->rightChild);
          })
DEF_DIFF ("-",      '-', OP, 3,
          {
            /* node - -, right child - diff(right), left child - diff(left),
            right left child - -1, right right child - cos(copy) */
            dN->key->type     = OP;
            dN->key->value    = '-';
            dN->key->priority = 3;

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

            dN->key = ddataCreate (2, OP, '*');
            /*dN->key->priority = 2;
            dN->key->type     = OP;
            dN->key->value    = '*'; */

            dN->leftChild    = nodeCtor ();
            dN->leftChild    = nodeDerivative (n->leftChild, dN->leftChild);

            dN->rightChild                = nodeCtor ();
            dN->rightChild->key->type     = OP;
            dN->rightChild->key->value    = 'c';
            dN->rightChild->key->priority = 1;

            dN->rightChild->leftChild  = nodeCreateCopy (n->leftChild);
          })
DEF_DIFF ("cos",    'c', OP, 1,
          {
            /* node - *, right child - *, left child - diff(),
            right left child - -1, right right child - cos(copy) */
            dN->key->type     = OP;
            dN->key->value    = '*';
            dN->key->priority = 2;

            dN->leftChild    = nodeCtor ();
            dN->leftChild    = nodeDerivative (n->leftChild, dN->leftChild);

            dN->rightChild                = nodeCtor ();
            dN->rightChild->key->type     = OP;
            dN->rightChild->key->value    = '*';
            dN->rightChild->key->priority = 2;

            dN->rightChild->leftChild             = nodeCtor ();
            dN->rightChild->leftChild->key->type  = CNST;
            dN->rightChild->leftChild->key->value = -1;
            dN->rightChild->key->priority          = 255;

            dN->rightChild->rightChild             = nodeCtor ();
            dN->rightChild->rightChild->key->type  = OP;
            dN->rightChild->rightChild->key->value = 's';
            dN->rightChild->key->priority          = 1;

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
