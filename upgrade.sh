#!/bin/bash
CPU_CORE=`nproc`
./shell/build_dependency.sh
git fetch --all && \
git reset --hard origin/master && \
./shell/build_source_code.sh && \
chgrp -R judge /usr/lib/cupjudge && \
chown -R judge /usr/lib/cupjudge

if [ ! -d /home/judge/etc ]; then
mkdir -p /home/judge/etc
cp -r etc/* /home/judge/etc/
./shell/build_config.sh
else
if [ ! -f /home/judge/etc/language.json ]; then
    cp etc/language.json /hone/judge/etc/language.json
fi

if [ ! -f /home/judge/etc/java0.policy ]; then
  cp etc/java0.policy /home/judge/etc/java0.policy
fi

if [ ! -f /home/judge/etc/judge.conf ]; then
  cp etc/judge.conf /home/judge/etc/judge.conf
fi

if [ ! -f /home/judge/etc/compile.json ]; then
  cp etc/compile.json /home/judge/etc/compile.json
fi

if [ ! -f /home/judge/etc/config.json ]; then
  ./shell/build_config.sh
fi
fi

if [ ! -f /usr/bin/sim.sh ]; then
  cd sim && \
  make fresh && \
  make exes && \
  chmod +x sim* && \
  cp sim_c.exe /usr/bin/sim_c && \
  cp sim_c++.exe /usr/bin/sim_cc && \
  cp sim_java.exe /usr/bin/sim_java && \
  cp sim_pasc.exe /usr/bin/sim_pas && \
  cp sim_text.exe /usr/bin/sim_text && \
  cp sim_lisp.exe /usr/bin/sim_scm && \
  cp sim.sh /usr/bin/ && \
  chmod +x /usr/bin/sim.sh
fi