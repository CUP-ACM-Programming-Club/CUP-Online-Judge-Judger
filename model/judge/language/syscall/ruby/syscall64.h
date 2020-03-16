//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
int LANG_RV[256] = {0, 1, 2, 3, 4, 5, 9, 10, 12, 13, 14, 16, 21, 22, 56, 59, 72, 97, 98, 107, 108, 131, 158, 202, 218,
                    231, 273, 96, 340, 4, 126, SYS_access, SYS_arch_prctl, SYS_brk,
                    SYS_close, SYS_execve, SYS_exit_group, SYS_fstat, SYS_futex,
                    SYS_getegid, SYS_geteuid, SYS_getgid, SYS_getuid, SYS_getrlimit,
                    SYS_mmap, SYS_mprotect, SYS_munmap, SYS_open, SYS_read,
                    SYS_rt_sigaction, SYS_rt_sigprocmask, SYS_set_robust_list,
                    SYS_set_tid_address, SYS_write, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
