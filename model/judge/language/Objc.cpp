//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Objc.h"
#include "util/util.h"
#include <cstring>
#ifdef __i386
#include "syscall/objectivec/syscall32.h"
#else
#include "syscall/objectivec/syscall64.h"
#endif

#include <seccomp.h>
#include "common/seccomp_helper.h"

#define SYSCALL_ARRAY LANG_OV

using std::memset;

extlang createInstanceobjectivec() {
    return new Objc;
}

deslang destroyInstanceobjectivec(Language* language) {
    delete language;
}

void Objc::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir -p %s/proc", work_dir);
    execute_cmd("/bin/mount -o bind /proc %s/proc", work_dir);
    execute_cmd("/bin/mkdir -p %s/lib/", work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libdbus-1.so.3                          %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libgcc_s.so.1                           %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libgcrypt.so.11                         %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libgpg-error.so.0                       %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/libz.so.1                               %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libc.so.6                 %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libdl.so.2                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libm.so.6                 %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libnsl.so.1               %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/libpthread.so.0           %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /lib/tls/i686/cmov/librt.so.1                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libavahi-client.so.3                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libavahi-common.so.3                %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libdns_sd.so.1                      %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libffi.so.5                         %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libgnustep-base.so.1.19             %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libgnutls.so.26                     %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libobjc.so.2                        %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libtasn1.so.3                       %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libxml2.so.2                        %s/lib/ ",
            work_dir);
    execute_cmd(
            "/bin/cp -aL /usr/lib/libxslt.so.1                        %s/lib/ ",
            work_dir);
}

void Objc::initCallCounter(int *call_counter) {
    memset(call_counter, 0, call_array_size);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++)
        call_counter[SYSCALL_ARRAY[i]] = HOJ_MAX_LIMIT;
}

std::string Objc::getFileSuffix() {
    return "m";
}

int Objc::getMemory(rusage ruse, pid_t pid) {
    const char* mark = "VmPeak:";
    const unsigned BUFFER_SIZE = 5 * (1 << 10);
    FILE *pf;
    char fn[BUFFER_SIZE], buf[BUFFER_SIZE];
    int ret = 0;
    sprintf(fn, "/proc/%d/status", pid);
    pf = fopen(fn, "re");
    auto m = static_cast<int>(strlen(mark));
    while (pf && fgets(buf, BUFFER_SIZE - 1, pf)) {

        buf[strlen(buf) - 1] = 0;
        if (strncmp(buf, mark, m) == 0) {
            sscanf(buf + m + 1, "%d", &ret);
        }
    }
    if (pf)
        fclose(pf);
    return ret << 10;
}

void Objc::buildSeccompSandbox() {
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_KILL);
    for (int i = 0; i == 0 || SYSCALL_ARRAY[i]; i++) {
        if (SYSCALL_ARRAY[i] == 59) {
            continue;
        }
        seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_ARRAY[i], 0);
    }
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1, SCMP_A1(SCMP_CMP_EQ, (scmp_datum_t)(getArgs())));
    if (install_helper()) {
        printf("install helper failed");
        exit(1);
    }
    seccomp_load(ctx);
}
