#!/bin/bash
CPU_CORE=$(nproc)
./shell/build_dependency.sh
cmake ./ && make -j$CPU_CORE && chmod a+x judge_client && mv ./judge_client /usr/bin
useradd -m -u 1536 judge
mkdir /home/judge
mkdir /home/judge/etc
mkdir /dev/shm/cupoj
mkdir /dev/shm/cupoj/submission
mkdir /home/judge/submission
chown judge /home/judge/*
chgrp judge /home/judge/*
cp -r etc/* /home/judge/etc/ &&
  chown -R judge /home/judge &&
  chgrp -R judge /home/judge &&
  cd model/judge/language &&
  cmake ./ &&
  make -j$CPU_CORE &&
  cd build/out &&
  mkdir -p /usr/lib/cupjudge &&
  cp * /usr/lib/cupjudge/ &&
  cd ../../../../../ &&
  cd external/mysql &&
  cmake ./ &&
  make -j$CPU_CORE &&
  cp -r build/out/* /usr/lib/cupjudge/ &&
  cd ../../ &&
  chgrp -R judge /usr/lib/cupjudge &&
  chown -R judge /usr/lib/cupjudge &&
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
  chmod +x /usr/bin/sim.sh &&
  cd ../
./shell/build_config.sh
