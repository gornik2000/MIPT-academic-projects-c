#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.cpp"
#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Encyclopedia\FileIO.cpp"

const int ASM_MAX_LBL_NUMBER = 100;

const int ASM_MAX_LBL_NAME_LENGTH = 20;

const int ASM_MAX_CMD_SIZE = 80;

/* returns commands  count [0], parameters count [1], labels count [2];
   function and labelNames should be cleared
*/
int *firstCompilation (FILE *cmdFile, char **labelNames, int *labelPositions)
{
  assert (cmdFile);
  assert (labelNames);
  assert (labelPositions);

  const int STAT_CMD_NUM = 0;
  const int STAT_PAR_NUM = 1;
  const int STAT_LBL_NUM = 2;

  int *stat = (int *)calloc (3, sizeof (*stat));
  assert (stat != NULL);

  /* current coursor position */
  int currentPosition = ftell (cmdFile);
  assert (currentPosition != -1L);

  /* number of commands */
  int    lblNum = 0;
  int strCmdNum = 0;

  char command[ASM_MAX_LBL_NAME_LENGTH] = "";
  char  cmdStr[ASM_MAX_CMD_SIZE] = "";
  char *label = NULL;

  while (fgets (cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    strCmdNum = sscanf (cmdStr, "%s", command);

    if (strCmdNum ==  0) continue;

    if (*command == ':')
    {
      label = (char *)calloc (strlen (command), sizeof (*label));
      strcpy (label, command);

      labelNames    [lblNum] = label;
      labelPositions[lblNum] = stat[STAT_CMD_NUM] + 1;
      lblNum ++;

      stat[STAT_LBL_NUM] += 1;

      continue;
    }

    #define CPU_DEF_CMD(name, num, par, mode, func) \
    { \
      if (strcmp (command, #name) == 0) \
      { \
        stat[STAT_CMD_NUM] += 1; \
        stat[STAT_PAR_NUM] += par; \
      } \
    };
    #include "commands.h"
    #undef CPU_DEF_CMD
    //printf ("commands %d pars %d labels %d\n",stat[0],stat[1],stat[2]);
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
  /*
  int *cmdByteCode = (int *)calloc(cmdCount + 1, sizeof (*cmdByteCode));

  char  cmdStr[ASM_MAX_CMD_SIZE] = "";
  char command[ASM_MAX_CMD_SIZE] = "";

  //char *par1 = (char *)calloc (ASM_MAX_PAR_SIZE, sizeof (*par1));

  int ip = 0;
  /*
  while (fgets(cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    #define CPU_DEF_CMD(name, num, par, func) \
    { \
      if (strcmp (command, #name) == 0) \
      { \
        if ((par) > 0) \
        { \
          \
        } \
        cmdByteCode[ip] =
        ip += par + 1 \
      } \
    }
    #include "commands.h"
    #undef CPU_DEF_CMD

    int cmdNumber = getCommand (cmdStr);


    if (cmdNumber == 0) continue;
    //if (cmdNumber == END) return 0;

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
    byteCode[ip] = cmdNumber;
    ip ++;
    //printf ("- command number %d\n", cmdNumber);
  }
  //printf ("%d", cmdCount);
  fclose (cmdFile);

  FILE *fileCode = fopen (fileByteCodeName, "wb");
  assert (fileCode);
  fwrite (byteCode, cmdCount, sizeof(int), fileCode); */

  //fclose(fileCode);


  for (int i =0; i<cmdStat[2]; i++) printf ("\n position %d name %s", labelPositions[i], labelNames[i]);

  for (int i = 0; i < ASM_MAX_LBL_NUMBER; i++) free(labelNames[i]);
  free (cmdStat);
  free (labelNames);
  free (labelPositions);
  fclose (cmdFile);
}

/**/
