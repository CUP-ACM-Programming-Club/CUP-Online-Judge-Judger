//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL32_H
#define JUDGE_LIBRARY_SYSCALL32_H
#include <sys/syscall.h>
int LANG_JV[256] = { 295, SYS_fcntl64, SYS_getdents64, SYS_ugetrlimit,
                     SYS_rt_sigprocmask, SYS_futex, SYS_read, SYS_mmap2, SYS_stat64,
                     SYS_open, SYS_close, SYS_execve, SYS_access, SYS_brk, SYS_readlink,
                     SYS_munmap, SYS_close, SYS_uname, SYS_clone, SYS_uname, SYS_mprotect,
                     SYS_rt_sigaction, SYS_sigprocmask, SYS_getrlimit, SYS_fstat64,
                     SYS_getuid32, SYS_getgid32, SYS_geteuid32, SYS_getegid32,
                     SYS_set_thread_area, SYS_set_tid_address, SYS_set_robust_list,
                     SYS_exit_group, 0 };
#endif //JUDGE_LIBRARY_SYSCALL32_H
