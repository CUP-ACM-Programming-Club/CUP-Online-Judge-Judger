//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Perl.h"
#include "unistd.h"
#include "util/util.h"

void Perl::run(int memory) {
    execl("/perl", "/perl", "Main.pl", (char *) nullptr);
}

void Perl::buildRuntime(const char *work_dir) {
    Language::buildRuntime(work_dir);
    execute_cmd("/bin/mkdir %s/usr", work_dir);
    execute_cmd("/bin/mkdir %s/usr/lib", work_dir);
    execute_cmd("/bin/cp /usr/lib/libperl* %s/usr/lib/", work_dir);
    execute_cmd("/bin/cp /usr/bin/perl* %s/", work_dir);
}

extlang createInstanceperl () {
    return new Perl;
}

deslang destroyInstanceperl (Language* language) {
    delete language;
}
