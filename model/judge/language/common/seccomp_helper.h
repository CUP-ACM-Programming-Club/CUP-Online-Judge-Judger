#ifndef JUDGE_LIBRARY_SECCOMP_HELPER_H
#define JUDGE_LIBRARY_SECCOMP_HELPER_H
#include <csignal>

extern void write_uint(char *buf, unsigned int val);
extern void helper(int nr, siginfo_t *info, void *void_context);
extern int install_helper();
#endif