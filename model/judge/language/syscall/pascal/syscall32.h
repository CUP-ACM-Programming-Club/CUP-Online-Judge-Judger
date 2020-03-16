//
// Created by Haoyuan Li on 2020/3/16.
//

#ifndef JUDGE_LIBRARY_SYSCALL32_H
#define JUDGE_LIBRARY_SYSCALL32_H
#include <sys/syscall.h>
int LANG_CV[256] = {0, 1, 2, 3, 4, 5, 8, 9, 11, 12, 20, 21, 59, 63, 89, 158, 231, 240, 272, 511, SYS_time, SYS_read,
                    SYS_uname, SYS_write, SYS_open,
                    SYS_close, SYS_execve, SYS_access, SYS_brk, SYS_munmap, SYS_mprotect,
                    SYS_mmap, SYS_fstat, SYS_set_thread_area, 252, SYS_arch_prctl, 0};
#endif //JUDGE_LIBRARY_SYSCALL32_H
