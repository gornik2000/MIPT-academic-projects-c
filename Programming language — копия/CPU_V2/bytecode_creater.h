#define CPU_DEF_CMD(name, num, par, func)                                 \
  {                                                                       \
    if ((strcmp (command, #name) == 0))                                   \
    {                                                                     \
      cmdByteCode[ipCmd] = num;                                           \
      ipCmd ++;                                                           \
                                                                          \
      switch (par)                                                        \
      {                                                                   \
        case 1:                                                           \
          parByteCode[ipPar] = getVal  (parameter,                        \
                                        getType (parameter),              \
                                        labelNames,                       \
                                        labelPositions,                   \
                                        cmdStat[ASM_STAT_LBL_NUM]);       \
          parByteCode[ipPar + 1] = getVal  (parameter,                    \
                                            getType (parameter),          \
                                            labelNames,                   \
                                            labelParameter,               \
                                            cmdStat[ASM_STAT_LBL_NUM]);   \
          ipPar += 2;                                                     \
          break;                                                          \
                                                                          \
        case 2:                                                           \
          cmdByteCode[ipCmd] = getType (parameter);                       \
          parByteCode[ipPar] = getVal  (parameter,                        \
                                        cmdByteCode[ipCmd],               \
                                        labelNames,                       \
                                        labelPositions,                   \
                                        cmdStat[ASM_STAT_LBL_NUM]);       \
          ipCmd ++;                                                       \
          ipPar ++;                                                       \
          break;                                                          \
      }                                                                   \
    }                                                                     \
  }

  #include "commands.h"

  #undef CPU_DEF_CMD
