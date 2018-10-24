/*
  assambler
  транслирует текст в машинный код
  принмиает
  add
  push x
  pop x
  mul
  div
  out
  end

  push r1
  pop r1

  push[]

  jmp
  :1-10
  ja
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* asm parameters */
const int ASM_MAX_CMD_SIZE  = 50;
const int ASM_FLAGS_NUMBER  = 100;
const int ASM_MAX_FLAG_SIZE = 3;
const int ASM_MAX_PAR_SIZE  = 10;

/* asm commands */
enum commands
{
  /* BASE COMMANDS CONSTANTS */
  END  = 1,
  IN   = 2,
  OUT  = 3,
  ADD  = 4,
  SUB  = 5,
  MUL  = 6,
  DIV  = 7,
  PUSH = 8,
  POP  = 9,

  /* MATH COMMANDS CONSTANTS */
  SIN  = 20,
  COS  = 21,
  TAN  = 22,
  LN   = 23,
  SQRT = 24,
  SQR  = 25,
  PWR  = 26,

  /* MACRO COMMANDS CONSTANTS */
  JMP = 40,
  JA  = 41,
  JB  = 42,
  JAE = 43,
};

/* asm registers */
enum registers
{
  AX = 1,
  BX = 2,
  CX = 3,
  DX = 4,
};

/* asm push types */
enum pushTypes
{
  VAL = 1,
  REG = 2,
  RAM = 3,
};

char getCommand(const char *s)
{
  char command[ASM_MAX_CMD_SIZE] = "";
  int parNumber = sscanf (s, "%s", command);
  //printf ("%s\n", command);
  /* empty command string */
  if (parNumber = 0)
  {
    return 0;
  }

  if (!strcmp (command, "END"))  return END;
  if (!strcmp (command, "IN"))   return IN;
  if (!strcmp (command, "OUT"))  return OUT;
  if (!strcmp (command, "ADD"))  return ADD;
  if (!strcmp (command, "SUB"))  return SUB;
  if (!strcmp (command, "MUL"))  return MUL;
  if (!strcmp (command, "DIV"))  return DIV;
  if (!strcmp (command, "PUSH")) return PUSH;
  if (!strcmp (command, "POP"))  return POP;

  if (!strcmp (command, "SIN"))  return SIN;
  if (!strcmp (command, "COS"))  return COS;
  if (!strcmp (command, "TAN"))  return TAN;
  if (!strcmp (command, "LN"))   return LN;
  if (!strcmp (command, "SQRT")) return SQRT;
  if (!strcmp (command, "SQR"))  return SQR;
  if (!strcmp (command, "PWR"))  return PWR;

  if (!strcmp (command, "JMP")) return JMP;
  if (!strcmp (command, "JA"))  return JA;
  if (!strcmp (command, "JB"))  return JB;
  if (!strcmp (command, "JAE")) return JAE;

  /* command was not identified */
  return 0;
}

/* give information about current push/pop command */
int pushPopToByteCode (char *par, int *type, int *value)
{
  assert (par != NULL);
  assert (type != NULL);
  assert (value != NULL);

  //printf("push-pop %s\n", par);
  if (isdigit(par[0]))
  {
    *type = VAL;
    //printf("type %d\n", *type);
    *value = atoi (par);
    return 0;
  }

  if (par[0] == '[')
  {
    *type = RAM;
    //printf ("parameter %s\n", par);
    par ++;
    //printf ("parameter %s\n", par);
    //par[strlen(par)] = '\0';
    *value = atoi(par);
    return 0;
  }

  if (!strcmp (par, "AX"))
  {
    *type = REG;
    *value = AX;
  }

  if (!strcmp (par, "BX"))
  {
    *type = REG;
    *value = BX;
  }

  if (!strcmp (par, "CX"))
  {
    *type = REG;
    *value = CX;
  }

  if (!strcmp (par, "DX"))
  {
    *type = REG;
    *value = DX;
  }
}

/* fills an array of flags, returns number of commands */
int firstCompilation (FILE *file, int *flags)
{
  assert (file);
  assert (flags);

  /* current coursor position, REMAKE it */
  int cmdNum = 0;
  int strCmdNum = 0;

  char *command = (char *)calloc (ASM_MAX_CMD_SIZE + 1, sizeof (*command));

  char par[ASM_MAX_PAR_SIZE]    = "";
  char cmdStr[ASM_MAX_CMD_SIZE] = "";

  while (fgets (cmdStr, ASM_MAX_CMD_SIZE, file) != NULL)
  {
    strCmdNum = sscanf (cmdStr, "%s%s", command, par);
    //printf(" 0 ");
    //printf (" = %d = \n", strCmdNum);
    //printf(" 1 ");
    //printf (" = command %s = \n", command);
    if (strCmdNum ==  0) continue;
    if (*command == '/') continue;

    if (*command == ':')
    {
      //printf("\n%s\n", command);
      flags[atoi(command + 1)] = cmdNum;
      continue;
    }
    //printf(" 2 ");
    //printf (" = %s = \n", command);
    if (!strcmp (command, "PUSH") || !strcmp (command, "POP"))
    {
      cmdNum += 3;
      //printf(" 2 ");
      continue;
    }
    //printf(" 3 ");
    //printf (" = %s = \n", command);
    cmdNum += strCmdNum;
    printf("->%d\n", cmdNum);
  }

  free (command);
  fseek (file, 0, SEEK_SET);
  return cmdNum;
}

int compilation (const char *fileName)
{
  assert (fileName != NULL);

  FILE *cmdFile = fopen (fileName, "rb");
  assert (cmdFile != NULL);

  //change lines to operations
  int labels[ASM_FLAGS_NUMBER] = {0};
  /* number of commands in file, creates array of labels*/
  int cmdCount = firstCompilation (cmdFile, labels);
  //for (int i = 0; i < ASM_FLAGS_NUMBER; i++) printf ("pos %d command %d\n", i, labels[i]);

  int *byteCode = (int *)calloc(cmdCount + 1, sizeof (*byteCode));

  char cmdStr[ASM_MAX_CMD_SIZE] = "";
  char command[ASM_MAX_CMD_SIZE] = "";

  //char par1[ASM_MAX_PAR_SIZE] = "";
  char *par1 = (char *)calloc (ASM_MAX_PAR_SIZE, sizeof (*par1));

  //char par2[ASM_MAX_PAR_SIZE] = "";
  //char par3[ASM_MAX_PAR_SIZE] = "";
  int ip = 0;

  while (fgets(cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    /* get command from current command string*/
    int cmdNumber = getCommand (cmdStr);

    /* commands anallizer */
    if (cmdNumber == 0) continue;
    //if (cmdNumber == END) return 0;
    /* push pop commands */
    if (cmdNumber == PUSH || cmdNumber == POP)
    {
      sscanf (cmdStr, "%s%s", command, par1);

      //printf("push-pop %s\n", par1);
      int type = 0;
      int value = 0;

      pushPopToByteCode (par1, &type, &value);

      byteCode[ip]   = cmdNumber;
      byteCode[ip+1] = type;
      byteCode[ip+2] = value;

      ip += 3;

      continue;
    }

    /* flag commands */
    if (cmdNumber == JMP || cmdNumber == JA  ||
        cmdNumber == JB  || cmdNumber == JAE   )
    {
      sscanf (cmdStr, "%s%s", command, par1);
      //printf ("= %s =\n", par1);

      byteCode[ip] = cmdNumber;

      par1 ++;
      //printf ("= %s =\n", par1);
      //printf ("= %d =\n", atoi(par1));
      //printf ("= %d =\n", labels[atoi(par1)]);

      byteCode[ip + 1] = labels[atoi(par1)];
      //printf ("= %d =\n", byteCode[ip + 1]);
      ip += 2;

      par1 --;
      //printf ("= %s =\n", par1);
      continue;
    }
    //printf(" 2 ");
    /* in other situations */
    byteCode[ip] = cmdNumber;
    ip ++;
    //printf ("- command number %d\n", cmdNumber);
  }
  //printf ("%d", cmdCount);
  fclose (cmdFile);

  FILE *fileCode = fopen ("Byte_Code.txt", "wb");
  assert (fileCode);
  fwrite (byteCode, cmdCount, sizeof(int), fileCode);

  fclose(fileCode);
  free (par1);
  for (int i = 0; i < cmdCount; i++) printf ("command in code %d \n", byteCode[i]);
  return 0;
}

main ()
{
  //printf ("%d", ADD);
  compilation ("input_command.txt");
}
