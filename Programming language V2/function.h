//---------------------------------------------------------------------------*/
struct myFunction
{
  const char    *name;
  char           argNum;
  char           varNum;
  unsigned char *start;
};
typedef struct myFunction cmd_fun;
//---------------------------------------------------------------------------*/
cmd_fun *funCtor (void)
{
  cmd_fun *fun = (cmd_fun *)calloc (1, sizeof (*fun));

  fun->argNum = 0;
  fun->varNum = 0;
  fun->name   = NULL;
  fun->start  = NULL;

  return fun;
}
//---------------------------------------------------------------------------*/
cmd_fun *funDtor (cmd_fun *fun)
{
  fun->argNum = -1;
  fun->varNum = -1;
  fun->name   = NULL;
  fun->start  = NULL;

  free (fun);
  fun = NULL;

  return fun;
}
//---------------------------------------------------------------------------*/
//               © Gorbachev Nikita, December 2018 + April 2019              //
//---------------------------------------------------------------------------*/
