//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
int LANG_OV[256] = {0, 1, 2, 3, 4, 5, 9, 10, 12, 21, 59, 158, 231, 102, 191, SYS_access, SYS_brk, SYS_close,
                    SYS_execve, SYS_exit_group, SYS_fstat, SYS_futex, SYS_getcwd,
                    SYS_getrlimit, SYS_gettimeofday, SYS_mmap, SYS_mprotect, SYS_munmap,
                    SYS_open, SYS_read, SYS_readlink, SYS_rt_sigaction, SYS_rt_sigprocmask,
                    SYS_set_robust_list, SYS_set_thread_area, SYS_set_tid_address,
                    SYS_uname, SYS_write, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
