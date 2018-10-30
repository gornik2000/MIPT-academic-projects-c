#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef elem_cpu data_disasm;
#define ELEM_DISASM_PRINT ELEM_CPU_PRINT

/* change to typedef format */
void fprintValName (FILE *file, char parType, data_disasm parTwo)
{
  switch (parType)
  {
    case type_val:
      fprintf (file, ELEM_DISASM_PRINT" ", parTwo);
      break;
    case type_reg:
      #define CPU_DEF_REG(name, num) \
      {                              \
        if (parTwo == num)           \
          fprintf(file, #name" ");   \
      }

      #include "registers.h"

      #undef CPU_DEF_REG
      break;
    case type_ram:
      fprintf (file, "["ELEM_DISASM_PRINT"] ", parTwo);
      break;
    case type_lbl:
      fprintf (file, ELEM_DISASM_PRINT" ", parTwo);
      break;
  }
}

void inverseCompilation (const char *codeFileName, const char *cmdFileName)
{
  FILE *codeFile = fopen (codeFileName, "rb");
  assert (codeFile != NULL);

  /* reading statistics info */
  int statBuf[3];
  fread (statBuf, sizeof (int), 3, codeFile);
  //printf ("\ncommands %d pars %d labels %d\n", statBuf[ASM_STAT_CMD_NUM], statBuf[ASM_STAT_PAR_NUM], statBuf[ASM_STAT_LBL_NUM]);

  /* reading commands */
  char        *cmdBuf = (char *)calloc (statBuf[ASM_STAT_CMD_NUM], sizeof (*cmdBuf));
  fread (cmdBuf, sizeof (char), statBuf[ASM_STAT_CMD_NUM], codeFile);

  //for (int i = 0; i < statBuf[ASM_STAT_CMD_NUM]; i++) printf ("\n command %d", cmdBuf[i]);

  /* reading parameters */
  data_disasm *parBuf = (data_disasm *)calloc (statBuf[ASM_STAT_PAR_NUM], sizeof (*parBuf));
  fread (parBuf, sizeof (data_disasm), statBuf[ASM_STAT_PAR_NUM], codeFile);

  //for (int i = 0; i < statBuf[ASM_STAT_PAR_NUM]; i++) printf ("\n parameter %d", parBuf[i]);

  /* writing information */
  FILE *cmdFile = fopen (cmdFileName, "w");

  int ipPar = 0;
  char  cmd = 0;

  for (int ipCmd = 0; ipCmd < statBuf[ASM_STAT_CMD_NUM]; ipCmd ++)
  {
    cmd = cmdBuf[ipCmd];
    #define CPU_DEF_CMD(name, num, par, mode, func)                     \
    {                                                                   \
      if (cmd == num)                                                   \
      {                                                                 \
        fprintf (cmdFile, #name" ");                                    \
        switch (par)                                                    \
        {                                                               \
          case 1:                                                       \
            fprintValName (cmdFile, type_lbl, parBuf[ipPar]);           \
            ipPar ++;                                                   \
            break;                                                      \
          case 2:                                                       \
            ipCmd ++;                                                   \
            fprintValName (cmdFile, cmdBuf[ipCmd], parBuf[ipPar]);      \
            ipPar ++;                                                   \
            break;                                                      \
        }                                                               \
        fprintf (cmdFile, "\n");                                        \
      }                                                                 \
    }
    #include "commands.h"
    #undef CPU_DEF_CMD
  }

  free(cmdBuf);
  free(parBuf);
}
