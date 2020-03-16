//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
int LANG_PHV[256] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 16, 21, 59, 79, 97, 158, 202, 218, 231, 257, 273,
                     257, 20, 146, 78, 158, 117, 60, 39, 102, 191, SYS_access,
                     SYS_brk, SYS_clone, SYS_close, SYS_execve, SYS_exit_group, SYS_fcntl,
                     SYS_fstat, SYS_futex, SYS_getcwd, SYS_getdents64, SYS_getrlimit,
                     SYS_gettimeofday, SYS_ioctl, SYS_lseek, SYS_lstat, SYS_mmap,
                     SYS_mprotect, SYS_munmap, SYS_open, SYS_read, SYS_readlink,
                     SYS_rt_sigaction, SYS_rt_sigprocmask, SYS_set_robust_list,
                     SYS_set_thread_area, SYS_set_tid_address, SYS_stat, SYS_time, SYS_uname,
                     SYS_write, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
