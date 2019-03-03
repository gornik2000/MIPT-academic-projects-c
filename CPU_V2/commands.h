/* name-number-par number-function body */

CPU_DEF_CMD ( end,  1, 0,
{
  *ipCmd += 1000000000;
})

CPU_DEF_CMD ( out,  3, 0,
{
  printf (" # ");

  for (elem_t i = 0; i < (&(cpu->mem))->size; i++)
  {
    printf (ELEM_CPU_PRINT"   ", ((&(cpu->mem))->data)[i]);
  }

  *ipCmd += 1;
  printf (";\n");
})

CPU_DEF_CMD (push,  4, 2,
{
  switch (cmdBuf[*ipCmd + 1])
  {
    case (type_val):
      stackPush (&(cpu->mem), parBuf[*ipPar]);
      break;

    case (type_reg):
      stackPush (&(cpu->mem), (cpu->reg)[(int)parBuf[*ipPar]]);
      break;

    case (type_ram):
      stackPush (&(cpu->mem), (cpu->ram)[(int)parBuf[*ipPar]]);
      break;

    case (type_lbl):
      stackPush (&(cpu->mem), parBuf[*ipPar]);
      break;

    case (type_inp):
    {
      data_cpu x = 0;
      scanf (ELEM_CPU_PRINT, &x);
      stackPush (&(cpu->mem), x);
      break;
    }
  }

  *ipPar += 1;
  *ipCmd += 2;
})

CPU_DEF_CMD ( pop,  5, 2,
{
  switch (cmdBuf[*ipCmd + 1])
  {
    case (type_val):
      stackPop (&(cpu->mem));
      break;

    case (type_reg):
      (cpu->reg)[(int)parBuf[*ipPar]] = stackPop (&(cpu->mem));
      break;

    case (type_ram):
      (cpu->ram)[(int)parBuf[*ipPar]] = stackPop (&(cpu->mem));
      break;

    case (type_lbl):
      stackPop (&(cpu->mem));
      break;
  }

  *ipPar += 1;
  *ipCmd += 2;
})

CPU_DEF_CMD ( add,  6, 0,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b + a);
  *ipCmd += 1;
})

CPU_DEF_CMD ( mul,  7, 0,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b * a);
  *ipCmd += 1;
})

CPU_DEF_CMD ( sub,  8, 0,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b - a);
  *ipCmd += 1;
})

CPU_DEF_CMD ( div,  9, 0,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), b / a);
  *ipCmd += 1;
})

CPU_DEF_CMD ( jmp, 21, 1,
{
  *ipCmd  = parBuf[*ipPar];
  *ipPar  = parBuf[*ipPar + 1];
})

CPU_DEF_CMD (  ja, 22, 1,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a > b)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD (  jb, 23, 1,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a < b)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD (  je, 24, 1,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a == b)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD ( jae, 25, 1,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a >= b)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD ( jbe, 26, 1,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a <= b)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD ( jne, 27, 1,
{
  data_cpu a = stackPop (&(cpu->mem));
  data_cpu b = stackPop (&(cpu->mem));

  if (a != b)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD ( jiz, 28, 1,
{
  data_cpu a = stackPop (&(cpu->mem));

  if (a == 0)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD (jinz, 29, 1,
{
  data_cpu a = stackPop (&(cpu->mem));

  if (a != 0)
  {
    *ipCmd  = parBuf[*ipPar];
    *ipPar  = parBuf[*ipPar + 1];
  }
  else
  {
    *ipCmd += 1;
    *ipPar += 2;
  }
})

CPU_DEF_CMD ( sin, 41, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), sin (a));
  *ipCmd += 1;
})

CPU_DEF_CMD ( cos, 42, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), cos (a));
  *ipCmd += 1;
})

CPU_DEF_CMD ( tan, 43, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), tan (a));
  *ipCmd += 1;
})

CPU_DEF_CMD (  ln, 44, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), log (a));
  *ipCmd += 1;
})

CPU_DEF_CMD ( exp, 45, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), exp (a));
  *ipCmd += 1;
})

CPU_DEF_CMD (sqrt, 46, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), sqrt (a));
  *ipCmd += 1;
})

CPU_DEF_CMD ( sqr, 47, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), a * a);
  *ipCmd += 1;
})

CPU_DEF_CMD (asin, 48, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), asin (a));
  *ipCmd += 1;
})

CPU_DEF_CMD (acos, 49, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), acos (a));
  *ipCmd += 1;
})

CPU_DEF_CMD (atan, 50, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), atan (a));
  *ipCmd += 1;
})

CPU_DEF_CMD (csgn, 51, 0,
{
  data_cpu a = stackPop (&(cpu->mem));

  stackPush (&(cpu->mem), -a);
  *ipCmd += 1;
})

CPU_DEF_CMD (call, 61, 1,
{
  /* put in function stack next command pointer */
  stackPush (&(cpu->fun), (*ipCmd + 1));
  stackPush (&(cpu->fun), (*ipPar + 1));

  /* jump to label */
  *ipCmd  = parBuf[*ipPar];
  *ipPar  = parBuf[*ipPar + 1];
})

CPU_DEF_CMD ( ret, 62, 0,
{
  /* returns last position */
  *ipPar  = stackPop (&(cpu->fun));
  *ipCmd  = stackPop (&(cpu->fun));
})

CPU_DEF_CMD ( mya, 63, 0,
{
  printf ("^_^\n");
  *ipCmd += 1;
})

