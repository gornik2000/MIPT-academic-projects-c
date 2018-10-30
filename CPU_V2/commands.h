/* name-num-par number-mode-func */
/* mode: 1 - stack, 2 - registers, 3 - ram, 4 - reg and ram, 5 - labels */
CPU_DEF_CMD ( end,  1, 0, 0, cpu_end )
CPU_DEF_CMD (  in,  2, 0, 4, cpu_in  )
CPU_DEF_CMD ( out,  3, 0, 0, cpu_out )
CPU_DEF_CMD (push,  4, 2, 4, cpu_push)
CPU_DEF_CMD ( pop,  5, 2, 4, cpu_pop )
CPU_DEF_CMD ( add,  6, 0, 0, cpu_add )
CPU_DEF_CMD ( sub,  7, 0, 0, cpu_sub )
CPU_DEF_CMD ( mul,  8, 0, 0, cpu_mul )
CPU_DEF_CMD ( div,  9, 0, 0, cpu_div )

CPU_DEF_CMD ( jmp, 21, 1, 5, cpu_jmp )
CPU_DEF_CMD (  ja, 22, 1, 5, cpu_ja  )
CPU_DEF_CMD (  jb, 23, 1, 5, cpu_jb  )
CPU_DEF_CMD ( jae, 24, 1, 5, cpu_jae )

CPU_DEF_CMD ( sin, 41, 0, 0, cpu_sin )
CPU_DEF_CMD ( cos, 42, 0, 0, cpu_cos )
CPU_DEF_CMD ( tan, 43, 0, 0, cpu_tan )
CPU_DEF_CMD (  ln, 44, 0, 0, cpu_ln  )
CPU_DEF_CMD ( exp, 45, 0, 0, cpu_exp )
CPU_DEF_CMD (sqrt, 46, 0, 0, cpu_sqrt)
CPU_DEF_CMD ( sqr, 47, 0, 0, cpu_sqr )
CPU_DEF_CMD ( pwr, 48, 0, 0, cpu_pwr )

CPU_DEF_CMD (call, 61, 1, 5, cpu_call)
CPU_DEF_CMD ( ret, 62, 0, 0, cpu_ret )
