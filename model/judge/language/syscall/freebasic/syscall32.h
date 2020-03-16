//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL32_H
#define JUDGE_LIBRARY_SYSCALL32_H
#include <sys/syscall.h>
int LANG_BASICV[256] = {3,4,5,6,11,33,45,54,91,101,122,125,140,174,175,191,192,195,197,240,243,252,258,311,330
        , SYS_access, SYS_brk, SYS_close, SYS_execve,
                        SYS_exit_group, SYS_fstat64, SYS_futex, SYS_getrlimit, SYS_ioctl,
                        SYS_ioperm, SYS_mmap2, SYS_open, SYS_read, SYS_rt_sigaction,
                        SYS_rt_sigprocmask, SYS_set_robust_list, SYS_set_thread_area,
                        SYS_set_tid_address, SYS_stat64, SYS_uname, SYS_write, 0 };
#endif //JUDGE_LIBRARY_SYSCALL32_H
