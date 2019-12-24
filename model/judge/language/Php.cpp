//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Php.h"
#include "unistd.h"
void Php::run(int memory) {
    execl("/php", "/php", "Main.php", (char *) nullptr);
}
