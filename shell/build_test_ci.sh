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
cd ../