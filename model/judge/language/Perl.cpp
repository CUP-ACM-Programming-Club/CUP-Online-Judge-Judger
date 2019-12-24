//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Perl.h"
#include "unistd.h"
void Perl::run(int memory) {
    execl("/perl", "/perl", "Main.pl", (char *) nullptr);
}
