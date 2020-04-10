//
// Created by Haoyuan Li on 2020/4/10.
//

#include "seccomp_helper.h"
#include <cstdio>
#include "syscall-name.h"
#include <unistd.h>
#include <cstring>
#include <cstddef>
#if defined(__i386__)
#define REG_RESULT    REG_EAX
#define REG_SYSCALL    REG_EAX
#define REG_ARG0    REG_EBX
#define REG_ARG1    REG_ECX
#define REG_ARG2    REG_EDX
#define REG_ARG3    REG_ESI
#define REG_ARG4    REG_EDI
#define REG_ARG5    REG_EBP
#elif defined(__x86_64__)
#define REG_RESULT    REG_RAX
#define REG_SYSCALL    REG_RAX
#define REG_ARG0    REG_RDI
#define REG_ARG1    REG_RSI
#define REG_ARG2    REG_RDX
#define REG_ARG3    REG_R10
#define REG_ARG4    REG_R8
#define REG_ARG5    REG_R9
#endif

#ifndef SYS_SECCOMP
#define SYS_SECCOMP 1
#endif
const char * const msg = "system call invalid: ";

/* Since "sprintf" is technically not signal-safe, reimplement %d here. */
void write_uint(char *buf, unsigned int val)
{
    int width = 0;
    unsigned int tens;

    if (val == 0) {
        strcpy(buf, "0");
        return;
    }
    for (tens = val; tens; tens /= 10)
        ++ width;
    buf[width] = '\0';
    for (tens = val; tens; tens /= 10)
        buf[--width] = (char) ('0' + (tens % 10));
}

void helper(int nr, siginfo_t *info, void *void_context) {
    char buf[255];
    ucontext_t *ctx = (ucontext_t *)(void_context);
    unsigned int syscall;
    if (info->si_code != SYS_SECCOMP)
        return;
    if (!ctx)
        return;

    syscall = (unsigned int) ctx->uc_mcontext.gregs[REG_SYSCALL];
    strcpy(buf, msg);
    if (syscall < sizeof(syscall_names)) {
        strcat(buf, syscall_names[syscall]);
        strcat(buf, "(");
    }
    write_uint(buf + strlen(buf), syscall);
    if (syscall < sizeof(syscall_names))
        strcat(buf, ")");
    strcat(buf, "\n");
    write(STDERR_FILENO, buf, strlen(buf));
    _exit(1);
}

int install_helper() {
    struct sigaction act;
    sigset_t mask;
    memset(&act, 0, sizeof(act));
    sigemptyset(&mask);
    sigaddset(&mask, SIGSYS);

    act.sa_sigaction = &helper;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSYS, &act, NULL) < 0) {
        perror("sigaction");
        return -1;
    }
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL)) {
        perror("sigprocmask");
        return -1;
    }
    return 0;
}