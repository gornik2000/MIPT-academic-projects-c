//-----------------------------------------------------------------------------
#define CPU_DEF_CMD(name, num, par, func) cmd_##name = (num),
enum commands
{
  #include "commands.h"
};
#undef CPU_DEF_CMD
//-----------------------------------------------------------------------------
#define CPU_DEF_REG(name, num) reg_##name = (num),
enum registers
{
  #include "registers.h"
};
#undef CPU_DEF_REG
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define CPU_DEF_TYPE(name, num) reg_##name = (num),
enum types
{
  #include "types.h"
};
#undef CPU_DEF_TYPE
//-----------------------------------------------------------------------------
