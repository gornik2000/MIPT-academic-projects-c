cpu_end
cpu_cmd
pu_out
cpu_push
cpu_pop
cpu_add
cpu_sub
cpu_mul
cpu_div
cpu_jmp
cpu_ja
cpu_jb
cpu_jae
cpu_sin )
CPU_DEF_CMD ( cos, 42, 0, 0, cpu_cos )
CPU_DEF_CMD ( tan, 43, 0, 0, cpu_tan )
CPU_DEF_CMD (  ln, 44, 0, 0, cpu_ln  )
CPU_DEF_CMD ( exp, 45, 0, 0, cpu_exp )
CPU_DEF_CMD (sqrt, 46, 0, 0, cpu_sqrt)
CPU_DEF_CMD ( sqr, 47, 0, 0, cpu_sqr )
CPU_DEF_CMD ( pwr, 48, 0, 0, cpu_pwr )

CPU_DEF_CMD (call, 61, 1, 5, cpu_call)
CPU_DEF_CMD ( ret, 62, 0, 0, cpu_ret )
