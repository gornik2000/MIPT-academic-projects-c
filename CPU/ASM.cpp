int firstCompilation (FILE *file, int *flags);

int compilation (const char *fileCmdName,
                 const char *fileByteCodeName = "ByteCode.txt");

/* fills an array of labels, returns number of commands */
int firstCompilation (FILE *file, int *flags)
{
  assert (file);
  assert (flags);

  /* current coursor position */
  int currentPosition = ftell (file);
  assert (currentPosition != -1L);

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
    //printf("->%d\n", cmdNum);
  }

  free (command);
  fseek (file, currentPosition, SEEK_SET);
  return cmdNum;
}

int compilation (const char *fileCmdName, const char *fileByteCodeName)
{
  assert (fileCmdName != NULL);
  assert (fileByteCodeName != NULL);

  FILE *cmdFile = fopen (fileCmdName, "rb");
  assert (cmdFile != NULL);

  //change lines to operations
  int labels[ASM_FLAGS_NUMBER] = {0};
  /* number of commands in file, creates array of labels*/
  int cmdCount = firstCompilation (cmdFile, labels);
  //for (int i = 0; i < ASM_FLAGS_NUMBER; i++) printf ("pos %d command %d\n", i, labels[i]);

  int *byteCode = (int *)calloc(cmdCount + 1, sizeof (*byteCode));

  char  cmdStr[ASM_MAX_CMD_SIZE] = "";
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

      /* make a label number */
      par1 ++;
      //printf ("= %s =\n", par1);
      //printf ("= %d =\n", atoi(par1));
      //printf ("= %d =\n", labels[atoi(par1)]);
      /* get label */
      byteCode[ip + 1] = labels[atoi(par1)];
      //printf ("= %d =\n", byteCode[ip + 1]);
      ip += 2;

      /* return back memory */
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

  FILE *fileCode = fopen (fileByteCodeName, "wb");
  assert (fileCode);
  fwrite (byteCode, cmdCount, sizeof(int), fileCode);

  fclose(fileCode);
  free (par1);
  //for (int i = 0; i < cmdCount; i++) printf ("command in code %d \n", byteCode[i]);
  return 0;
}

/*main ()
{
  //printf ("%d", ADD);
  compilation ("input_command.txt");
}
*/
