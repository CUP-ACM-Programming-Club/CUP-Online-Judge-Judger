//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL32_H
#define JUDGE_LIBRARY_SYSCALL32_H
#include <sys/syscall.h>
int LANG_BV[256] = { 7, 120, 42, 158, 117, 60, 39, 102, 191, 183, SYS_access,
                     SYS_brk, SYS_close, SYS_dup2, SYS_execve, SYS_exit_group, SYS_fcntl64,
                     SYS_fstat64, SYS_getegid32, SYS_geteuid32, SYS_getgid32, SYS_getpgrp,
                     SYS_getpid, SYS_getppid, SYS_getrlimit, SYS_gettimeofday, SYS_getuid32,
                     SYS_ioctl, SYS__llseek, SYS_mmap2, SYS_mprotect, SYS_munmap, SYS_open,
                     SYS_read, SYS_rt_sigaction, SYS_rt_sigprocmask, SYS_set_thread_area,
                     SYS_stat64, SYS_time, SYS_uname, SYS_write,0 };
#endif //JUDGE_LIBRARY_SYSCALL32_H
