//
// Created by Haoyuan Li on 2019/12/24.
//

#include "Php.h"
#include "unistd.h"
void Php::run(int memory) {
    execl("/php", "/php", "Main.php", (char *) nullptr);
}
extlang createInstancephp() {
    return new Php;
}

deslang destroyInstancephp(Language* language) {
    delete language;
}
