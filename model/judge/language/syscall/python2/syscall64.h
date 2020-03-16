//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
int LANG_YV[256] = {0, 39, 99, 302, 99, 32, 72, 131, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 16, 21, 59, 78, 79, 89,
                    97, 102, 104, 107, 108, 158, 202, 218, 231, 257, 273, 41, 42, 146, SYS_mremap, 158, 117, 60, 39,
                    102, 191,
                    SYS_access, SYS_arch_prctl, SYS_brk, SYS_close, SYS_execve,
                    SYS_exit_group, SYS_fcntl, SYS_fstat, SYS_futex, SYS_getcwd,
                    SYS_getdents, SYS_getegid, SYS_geteuid, SYS_getgid, SYS_getrlimit,
                    SYS_getuid, SYS_ioctl, SYS_lseek, SYS_lstat, SYS_mmap, SYS_mprotect,
                    SYS_munmap, SYS_open, SYS_read, SYS_readlink, SYS_rt_sigaction,
                    SYS_rt_sigprocmask, SYS_set_robust_list, SYS_set_tid_address, SYS_stat,
                    SYS_write, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
