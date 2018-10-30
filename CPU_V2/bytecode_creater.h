#define CPU_DEF_CMD(name, num, par, mode, func)                           \
  {                                                                       \
    if ((strcmp (command, #name) == 0))                                   \
    {                                                                     \
      cmdByteCode[ipCmd] = num;                                           \
      ipCmd ++;                                                           \
                                                                          \
      switch (par)                                                        \
      {                                                                   \
        case 1:                                                           \
        {                                                                 \
          parByteCode[ipPar] = getVal  (parameter,                        \
                                        getType (parameter),              \
                                        labelNames,                       \
                                        labelPositions,                   \
                                        cmdStat[ASM_STAT_LBL_NUM]);       \
          ipPar ++;                                                       \
          break;                                                          \
        }                                                                 \
        case 2:                                                           \
        {                                                                 \
          cmdByteCode[ipCmd] = getType (parameter);                       \
          parByteCode[ipPar] = getVal  (parameter,                        \
                                        cmdByteCode[ipCmd],               \
                                        labelNames,                       \
                                        labelPositions,                   \
                                        cmdStat[ASM_STAT_LBL_NUM]);       \
          ipCmd ++;                                                       \
          ipPar ++;                                                       \
          break;                                                          \
        }                                                                 \
      }                                                                   \
    }                                                                     \
  }

  #include "commands.h"

  #undef CPU_DEF_CMD
