#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Encyclopedia\FileIO.cpp"

const int ASM_MAX_LBL_NUMBER = 100;

const int ASM_MAX_LBL_NAME_LENGTH = 20;

const int ASM_MAX_CMD_SIZE = 80;

const int ASM_STAT_CMD_NUM = 0;

const int ASM_STAT_PAR_NUM = 1;

const int ASM_STAT_LBL_NUM = 2;

typedef elem_cpu elem_asm;

/* returns commands  count [0], parameters count [1], labels count [2];
   function and labelNames should be cleared
*/
int *firstCompilation (FILE *cmdFile, char **labelNames, int *labelPositions)
{
  assert (cmdFile);
  assert (labelNames);
  assert (labelPositions);

  int *stat = (int *)calloc (3, sizeof (*stat));
  assert (stat != NULL);

  /* current coursor position */
  int currentPosition = ftell (cmdFile);
  assert (currentPosition != -1L);

  /* number of labels */
  int strCmdNum = 0;

  char command[ASM_MAX_LBL_NAME_LENGTH] = "";
  char  cmdStr[ASM_MAX_CMD_SIZE]        = "";
  char *label = NULL;

  while (fgets (cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    if (sscanf (cmdStr, "%s", command) ==  0) continue;

    if (*command == ';') continue;

    if (*command == ':')
    {
      label = (char *)calloc (strlen (command), sizeof (*label));
      strcpy (label, command);

      labelNames    [stat[ASM_STAT_LBL_NUM]] = label;
      labelPositions[stat[ASM_STAT_LBL_NUM]] = stat[ASM_STAT_CMD_NUM];

      stat[ASM_STAT_LBL_NUM] ++;
      memset(command, '\0', sizeof(command));

      continue;
    }

    #define CPU_DEF_CMD(name, num, par, mode, func)             \
    {                                                           \
      if (strcmp (command, #name) == 0)                         \
      {                                                         \
        /*printf (" %s \n", command);*/                         \
        stat[ASM_STAT_CMD_NUM] ++;                              \
        stat[ASM_STAT_PAR_NUM] += par;                          \
        memset(command, '\0', sizeof(command));                 \
      }                                                         \
    };
    #include "commands.h"
    #undef CPU_DEF_CMD
  }

  fseek (cmdFile, currentPosition, SEEK_SET);

  //printf ("commands %d pars %d labels %d\n",stat[ASM_STAT_CMD_NUM],stat[ASM_STAT_PAR_NUM],stat[ASM_STAT_LBL_NUM]);
  return stat;
}

elem_asm getType (char *cmd)
{
  switch (cmd[0])
  {
    case '[': return type_ram;
    case 'r': return type_reg;
    case ':': return type_lbl;
    default : return type_val;
  }
}

elem_asm getVal (char *cmd,       char type,
                 char **lblNames, int *lblPos, int lblNumber)
{
  switch (type)
  {
    case type_ram:
      return atof (cmd + 1);
    case type_reg:
      {
      char registerNum = 0;

      #define CPU_DEF_REG(name, num)    \
      {                                 \
        if (strcmp (cmd, #name) == 0)   \
          registerNum = num;            \
      }
      #include "registers.h"
      #undef CPU_DEF_REG

      return registerNum;
      }
    case type_val:
      return atof (cmd);
    case type_lbl:
      for (int i = 0; i < lblNumber; i++)
        if (strcmp(lblNames[i], cmd) == 0)
          return lblPos[i];
  }
  return 0;
}

int compilation (const char *cmdFileName, const char *byteCodeFileName)
{
  assert (cmdFileName != NULL);
  assert (byteCodeFileName != NULL);

  FILE *cmdFile = fopen (cmdFileName, "rb");
  assert (cmdFile != NULL);

  char **labelNames     = (char **)calloc (ASM_MAX_LBL_NUMBER, sizeof (*labelNames));
  int   *labelPositions = (int   *)calloc (ASM_MAX_LBL_NUMBER, sizeof (*labelPositions));

  int *cmdStat = firstCompilation (cmdFile, labelNames, labelPositions);
  //printf ("commands %d pars %d labels %d\n", cmdStat[ASM_STAT_CMD_NUM], cmdStat[ASM_STAT_PAR_NUM], cmdStat[ASM_STAT_LBL_NUM]);

  char *cmdByteCode = (char *)calloc(cmdStat[ASM_STAT_CMD_NUM], sizeof (*cmdByteCode));
  int   ipCmd = 0;
  /*change to all types */
  elem_asm  *parByteCode = (elem_asm  *)calloc(cmdStat[ASM_STAT_PAR_NUM], sizeof (*parByteCode));
  int        ipPar = 0;

  char    cmdStr[ASM_MAX_CMD_SIZE] = "";
  char   command[ASM_MAX_CMD_SIZE] = "";
  char parameter[ASM_MAX_CMD_SIZE] = "";

  while (fgets(cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    sscanf (cmdStr, "%s%s", command, parameter);

    #include "bytecode_creater.h"

    memset(command, '\0', sizeof(command));
  }

  fclose (cmdFile);

  //for (int i = 0; i < cmdStat[ASM_STAT_LBL_NUM]; i++) printf ("\n position %d name %s", labelPositions[i], labelNames[i]);
  //for (int i = 0; i < cmdStat[ASM_STAT_CMD_NUM]; i++) printf ("\n command %d", cmdByteCode[i]);
  //for (int i = 0; i < cmdStat[ASM_STAT_PAR_NUM]; i++) printf ("\n parameter %lf", parByteCode[i]);

  /* write bytecode */
  FILE *byteCodeFile = fopen (byteCodeFileName, "wb");

  fwrite (cmdStat,     sizeof(int),        3,                         byteCodeFile);
  fwrite (cmdByteCode, sizeof (char),      cmdStat[ASM_STAT_CMD_NUM], byteCodeFile);
  fwrite (parByteCode, sizeof (elem_asm) , cmdStat[ASM_STAT_PAR_NUM], byteCodeFile);

  fclose (byteCodeFile);

  for (int i = 0; i < ASM_MAX_LBL_NUMBER; i++) free(labelNames[i]);

  free (parByteCode);
  free (cmdByteCode);
  free (cmdStat);
  free (labelNames);
  free (labelPositions);
}
