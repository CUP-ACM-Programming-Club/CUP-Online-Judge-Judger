//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL32_H
#define JUDGE_LIBRARY_SYSCALL32_H
#include <sys/syscall.h>
int LANG_CSV[256] = {3,4,5,6,11,13,33,45,54,78,85,91,99,102,120,122,125,140,141,158,174,175,
                     183,186,191,192,195,197,199,221,240,242,243,252,258,265,266,270,295,
                     311, 11,33,45,192,141, 158, 117, 60, 39, 102, 191, SYS_access, SYS_brk,
                     SYS_chmod, SYS_clock_getres, SYS_clock_gettime, SYS_clone, SYS_close,
                     SYS_execve, SYS_exit_group, SYS_fcntl64, SYS_fstat64, SYS_ftruncate64,
                     SYS_futex, SYS_getcwd, SYS_getdents64, SYS_geteuid32, SYS_getpid,
                     SYS_getppid, SYS_getrlimit, SYS_gettimeofday, SYS_getuid32, SYS_ioctl,
                     SYS__llseek, SYS_lstat64, SYS_mmap2, SYS_mprotect, SYS_mremap,
                     SYS_munmap, SYS_open, SYS_read, SYS_readlink, SYS_rt_sigaction,
                     SYS_rt_sigprocmask, SYS_sched_getaffinity, SYS_sched_getparam,
                     SYS_sched_get_priority_max, SYS_sched_get_priority_min,
                     SYS_sched_getscheduler, SYS_set_robust_list, SYS_set_thread_area,
                     SYS_set_tid_address, SYS_sigaltstack, SYS_stat64, SYS_statfs,
                     SYS_tgkill, SYS_time, SYS_uname, SYS_unlink, SYS_write, 0 };
#endif //JUDGE_LIBRARY_SYSCALL32_H
