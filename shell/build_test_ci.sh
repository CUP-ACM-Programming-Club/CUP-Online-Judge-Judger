#!/bin/bash
./shell/build_dependency.sh
CPU_CORE=$(nproc)
cmake ./ &&
make -j$CPU_CORE &&
cd model/judge/language &&
cmake ./ &&
make -j$CPU_CORE &&
cd build/out &&
cd ../../../../../ &&
cd external/mysql &&
cmake ./ &&
make -j$CPU_CORE &&
cd ../../ &&
cd external/compare &&
cmake ./ &&
make -j$CPU_CORE &&
cd ../../ &&
cd sim &&
make fresh &&
make exes &&
chmod +x sim* &&
cp sim_c.exe /usr/bin/sim_c &&
cp sim_c++.exe /usr/bin/sim_cc &&
cp sim_java.exe /usr/bin/sim_java &&
cp sim_pasc.exe /usr/bin/sim_pas &&
cp sim_text.exe /usr/bin/sim_text &&
cp sim_lisp.exe /usr/bin/sim_scm &&
cp sim.sh /usr/bin/ &&
cd ../