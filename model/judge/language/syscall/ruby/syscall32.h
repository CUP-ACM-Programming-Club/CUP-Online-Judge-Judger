//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL32_H
#define JUDGE_LIBRARY_SYSCALL32_H
#include <sys/syscall.h>
int LANG_RV[256] = { 3,4,5,6,11,33,42,45,54,77,78,91,120,122,125,174,175,183,186,191,192,
                     195,196,197,199,200,201,202,221,240,243,252,258,311,
                     340, 126, SYS_access, SYS_brk, SYS_close, SYS_execve,
                     SYS_exit_group, SYS_fstat64, SYS_futex, SYS_getegid32, SYS_geteuid32,
                     SYS_getgid32, SYS_getrlimit, SYS_gettimeofday, SYS_getuid32, SYS_mmap2,
                     SYS_mprotect, SYS_munmap, SYS_open, SYS_read, SYS_rt_sigaction,
                     SYS_rt_sigprocmask, SYS_set_robust_list, SYS_set_thread_area,
                     SYS_set_tid_address, SYS_uname, SYS_write, 191, 195, 120, 7, 0 };
#endif //JUDGE_LIBRARY_SYSCALL32_H
