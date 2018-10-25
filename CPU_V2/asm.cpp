#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.cpp"
#include "C:\Users\Lenovo\Documents\GitHub\safe-morning\Encyclopedia\FileIO.cpp"

const int ASM_MAX_LBL_NUMBER = 100;

const int ASM_MAX_LBL_NAME_LENGTH = 20;

const int ASM_MAX_CMD_SIZE = 80;

/* returns commands  count, labelNames should be cleared */
int firstCompilation (FILE *cmdFile, char **labelNames, int *labelPositions)
{
  assert (cmdFile);
  assert (labelNames);
  assert (labelPositions);

  /* number of commands */
  int    lblNum = 0;
  int    cmdNum = 0;
  int strCmdNum = 0;

  char command[ASM_MAX_LBL_NAME_LENGTH] = "";
  char  cmdStr[ASM_MAX_CMD_SIZE] = "";
  char *label = NULL;

  while (fgets (cmdStr, ASM_MAX_CMD_SIZE, cmdFile) != NULL)
  {
    strCmdNum = sscanf (cmdStr, "%s", command);
    //while (*buf == ' ') buf ++;
    //command = buf;
    //buf = strchr (buf, '\n') + 1;
    //printf (" comm -%s- num %d buf %p \n", command, cmdNum, buf);

    if (strCmdNum ==  0) continue;

    if (*command == ':')
    {
      label = (char *)calloc (strlen (command), sizeof (*label));
      strcpy (label, command);

      labelNames    [lblNum] = label;
      labelPositions[lblNum] = cmdNum + 1;
      lblNum ++;

      continue;
    }

    #define CPU_DEF_CMD(name, num, par, func) \
    { \
      if (strcmp (command, #name) == 0) \
        cmdNum += par + 1; \
    }
    #include "commands.h"
    #undef CPU_DEF_CMD

  }
  return cmdNum;
}

int compilation (const char *cmdFileName, const char *bytecodeFileName)
{
  assert (cmdFileName != NULL);
  assert (bytecodeFileName != NULL);

  FILE *cmdFile = fopen (cmdFileName, "rb");
  assert (cmdFile != NULL);

  //int cmdFileSize = fileSize (cmdFile);
  //char *cmdFileBuf = (char *)calloc(cmdFileSize + 1, sizeof (cmdFileBuf));
  //assert (cmdFileBuf);

  /* puts program from file into buff */
  //int readSize = fread (cmdFileBuf, sizeof (char), cmdFileSize, cmdFile);
  //assert (readSize == cmdFileSize);
  //printf (" %d ", cmdFileSize);

  char **labelNames = (char **)calloc (ASM_MAX_LBL_NUMBER, sizeof (*labelNames));
  int *labelPositions = (int *)calloc (ASM_MAX_LBL_NUMBER, sizeof (*labelPositions));

  //printf (" buf %p \n", cmdFileBuf);
  int cmdCount = firstCompilation (cmdFile, labelNames, labelPositions);
  //printf ("%d", cmdCount);
  //printf (" buf %p \n", cmdFileBuf);

  for (int i =0; i<ASM_MAX_LBL_NUMBER; i++) printf ("\n position %d name %s", labelPositions[i], labelNames[i]);
  fclose (cmdFile);
}

/**/
