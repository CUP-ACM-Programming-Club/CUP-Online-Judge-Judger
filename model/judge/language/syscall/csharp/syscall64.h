//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL64_H
#define JUDGE_LIBRARY_SYSCALL64_H
#include <sys/syscall.h>
int LANG_CSV[256] = {0, 39, 157, 302, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 16, 21, 24, 41, 42, 56, 59, 63, 72, 78,
                     79, 89, 97, 102, 131, 137, 158, 202, 204, 218, 229, 231, 234, 257, 273, 257, 141, 95, 64, 65, 66,
                     83, 24, 42, 41, 158, 117, 60,
                     39, 102, 191, SYS_access, SYS_brk, SYS_chmod, SYS_clock_getres,
                     SYS_clock_gettime, SYS_clone, SYS_close, SYS_execve, SYS_exit_group,
                     SYS_fcntl, SYS_fstat, SYS_ftruncate, SYS_futex, SYS_getcwd,
                     SYS_getdents, SYS_geteuid, SYS_getpid, SYS_getppid, SYS_getrlimit,
                     SYS_gettimeofday, SYS_getuid, SYS_ioctl, SYS_lseek, SYS_lstat, SYS_mmap,
                     SYS_mprotect, SYS_mremap, SYS_munmap, SYS_open, SYS_read, SYS_readlink,
                     SYS_rt_sigaction, SYS_rt_sigprocmask, SYS_sched_getaffinity,
                     SYS_sched_getparam, SYS_sched_get_priority_max,
                     SYS_sched_get_priority_min, SYS_sched_getscheduler, SYS_set_robust_list,
                     SYS_set_thread_area, SYS_set_tid_address, SYS_sigaltstack, SYS_stat,
                     SYS_statfs, SYS_tgkill, SYS_time, SYS_uname, SYS_unlink, SYS_write, 0};
#endif //JUDGE_LIBRARY_SYSCALL64_H
