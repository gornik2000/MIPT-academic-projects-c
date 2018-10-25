#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.cpp"
#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Encyclopedia\FileIO.cpp"

const int ASM_MAX_LBL_NUMBER = 100;

const int ASM_MAX_LBL_NAME_LENGTH = 20;

const int ASM_MAX_CMD_SIZE = 80;

const int ASM_STAT_CMD_NUM = 0;
const int ASM_STAT_PAR_NUM = 1;
const int ASM_STAT_LBL_NUM = 2;

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
  char  cmdStr[ASM_MAX_CMD_SIZE] = "";
  char *label = NULL;

  while (fgets (cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    if (sscanf (cmdStr, "%s", command) ==  0) continue;

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

    #define CPU_DEF_CMD(name, num, par, mode, func) \
    { \
      if (strcmp (command, #name) == 0) \
      { \
        printf (" %s \n", command); \
        stat[ASM_STAT_CMD_NUM] ++; \
        stat[ASM_STAT_PAR_NUM] += par; \
        memset(command, '\0', sizeof(command)); \
      } \
    };
    #include "commands.h"
    #undef CPU_DEF_CMD
  }
  fseek (cmdFile, currentPosition, SEEK_SET);

  printf ("commands %d pars %d labels %d\n",stat[0],stat[1],stat[2]);
  return stat;
}

int compilation (const char *cmdFileName, const char *bytecodeFileName)
{
  assert (cmdFileName != NULL);
  assert (bytecodeFileName != NULL);

  FILE *cmdFile = fopen (cmdFileName, "rb");
  assert (cmdFile != NULL);

  char **labelNames = (char **)calloc (ASM_MAX_LBL_NUMBER, sizeof (*labelNames));
  int *labelPositions = (int *)calloc (ASM_MAX_LBL_NUMBER, sizeof (*labelPositions));

  int *cmdStat = firstCompilation (cmdFile, labelNames, labelPositions);
  printf ("commands %d pars %d labels %d\n",cmdStat[0],cmdStat[1],cmdStat[2]);

  char *cmdByteCode = (char *)calloc(cmdStat[ASM_STAT_CMD_NUM], sizeof (*cmdByteCode));
  int ipCmd = 0;
  /*change to all types */
  int *parByteCode = (int *)calloc(cmdStat[ASM_STAT_PAR_NUM], sizeof (*cmdByteCode));
  int ipPar = 0;

  char    cmdStr[ASM_MAX_CMD_SIZE] = "";
  char   command[ASM_MAX_CMD_SIZE] = "";
  char parameter[ASM_MAX_CMD_SIZE] = "";

  while (fgets(cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    sscanf (cmdStr, "%s%s", command, parameter);
    #define CPU_DEF_CMD(name, num, par, mode, func)     \
    {                                                   \
      if (strcmp (command, #name) == 0)                 \
      {                                                 \
        cmdByteCode[ipCmd] = num;                       \
        ipCmd ++;                                       \
                                                        \
        switch (par)                                    \
        {                                               \
        case 1:                                         \
          for (int i = 0; i < cmdStat[2]; i++)          \
            if (strcmp(labelNames[i], parameter) == 0)  \
              parByteCode[ipPar] = labelPositions[i];   \
                                                        \
          ipPar ++;                                     \
          break;                                        \
                                                        \
        case 2:                                         \
        /*#define CPU_DEF_REG(regName, regNum)          \
          {                                             \
            if (strcmp (parameter, #regName) == 0)      \
            {                                           \
              parByteCode[ipPar] = type_reg;            \
              parByteCode[ipPar + 1 ] = reg_##regName;  \
              ipPar += 2;                               \
              continue;                                 \
            }                                           \
          }                                             \
          #include "registers.h"                        \
          #undef CPU_DEF_REG */                         \
                                                        \
          parByteCode[ipPar] = type_val;                \
          parByteCode[ipPar + 1 ] = atof (command);     \
                                                        \
          ipPar += 2;                                   \
          break;                                        \
        }                                               \                                     \
      }                                                 \
    };
    #include "commands.h"
    #undef CPU_DEF_CMD
  }
  for (int i =0; i<cmdStat[2]; i++) printf ("\n position %d name %s", labelPositions[i], labelNames[i]);

  for (int i = 0; i < ASM_MAX_LBL_NUMBER; i++) free(labelNames[i]);
  free (cmdStat);
  free (labelNames);
  free (labelPositions);
  fclose (cmdFile);
}

/**/
