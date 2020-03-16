//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
int LANG_BV[256] = {0, 1, 3, 4, 5, 8, 9, 10, 12, 13, 14, 16, 21, 33, 39, 59, 63, 72, 79, 99, 102, 104, 107, 108, 110,
                    111, 158, 231, 257, 302, 1, 2, 3, 4, 5, 8, 9, 10, 12, 13, 14, 16, 21, 33, 39, 59, 63, 72, 79, 97,
                    99, 102, 104, 107, 108, 110, 111, 158, 231,
                    96, 22, 61, 56, 42, 41, 79, 158, 117, 60, 39, 102, 191,
                    183, SYS_access, SYS_arch_prctl, SYS_brk, SYS_close, SYS_dup2,
                    SYS_execve, SYS_exit_group, SYS_fcntl, SYS_fstat, SYS_getegid,
                    SYS_geteuid, SYS_getgid, SYS_getpgrp, SYS_getpid, SYS_getppid,
                    SYS_getrlimit, SYS_getuid, SYS_ioctl, SYS_lseek, SYS_mmap, SYS_mprotect,
                    SYS_munmap, SYS_open, SYS_read, SYS_rt_sigaction, SYS_rt_sigprocmask,
                    SYS_stat, SYS_uname, SYS_write, 14, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
