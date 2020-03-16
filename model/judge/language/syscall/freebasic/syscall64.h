//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
//freebasic
int LANG_BASICV[256] = {0, 1, 2, 3, 4, 5, 9, 10, 12, 13, 14, 16, 21, 59, 97, 158, 173, 202, 218, 231, 273,
                        101, 54, 122, 175, 174, 240, 311, 258, 243, 6, 197,
                        252, 146, 195, 192, 33, 45, 125, 191, SYS_access, SYS_brk, SYS_close,
                        SYS_execve, SYS_exit_group, SYS_fstat, SYS_futex, SYS_getrlimit,
                        SYS_ioctl, SYS_ioperm, SYS_mmap, SYS_open, SYS_read, SYS_rt_sigaction,
                        SYS_rt_sigprocmask, SYS_set_robust_list, SYS_set_thread_area,
                        SYS_set_tid_address, SYS_stat, SYS_uname, SYS_write, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
