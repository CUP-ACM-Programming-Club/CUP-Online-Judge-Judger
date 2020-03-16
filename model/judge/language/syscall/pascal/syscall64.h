//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
int LANG_PV[256] = {0, 1, 2, 3, 4, 9, 11, 13, 16, 59, 89, 97, 201, 231, 511, SYS_open, SYS_set_thread_area, SYS_brk,
                    SYS_read,
                    SYS_uname, SYS_write, SYS_execve, SYS_ioctl, SYS_readlink, SYS_mmap,
                    SYS_rt_sigaction, SYS_getrlimit, 252, 191, 158, 231, SYS_close,
                    SYS_exit_group, SYS_munmap, SYS_time, 4, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
