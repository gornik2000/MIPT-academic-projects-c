#define CPU_DEF_CMD(name, num, par, mode, func)                           \
  {                                                                       \
    if ((strcmp (command, #name) == 0))                                   \
    {                                                                     \
      cmdByteCode[ipCmd] = num;                                           \
      ipCmd ++;                                                           \
      switch (par)                                                        \
      {                                                                   \
        case 1:                                                           \
        {                                                                 \
          parByteCode[ipPar]     = getVal  (parameter,                    \
                                            getType (parameter),          \
                                            labelNames,                   \
                                            labelPositions,               \
                                            cmdStat[ASM_STAT_LBL_NUM]);   \
          ipPar += 1;                                                     \
          break;                                                          \
        }                                                                 \
        case 2:                                                           \
        {                                                                 \
          parByteCode[ipPar]     = getType (parameter);                   \
          parByteCode[ipPar + 1] = getVal  (parameter,                    \
                                            parByteCode[ipPar],           \
                                            labelNames,                   \
                                            labelPositions,               \
                                            cmdStat[ASM_STAT_LBL_NUM]);   \
          ipPar += 2;                                                     \
          break;                                                          \
        }                                                                 \
      }                                                                   \
    }                                                                     \
  }

  #include "commands.h"

  #undef CPU_DEF_CMD
