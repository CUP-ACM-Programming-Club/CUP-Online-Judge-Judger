#!/bin/bash
CPU_CORE=`nproc`
git fetch --all && \
git reset --hard origin/master && \
cmake ./ && \
make -j$CPU_CORE && \
chmod a+x judge_client && \
mv judge_client /usr/bin && \
cd model/judge/language && \
cmake ./ && \
make -j$CPU_CORE && \
cd build/out && \
mkdir -p /usr/lib/cupjudge && \
cp * /usr/lib/cupjudge/ && \
cd ../../../../../ && \
cd external/mysql && \
cmake ./ && \
make -j$CPU_CORE && \
cp -r build/out/* /usr/lib/cupjudge/ && \
cd ../../ && \
chgrp -R judge /usr/lib/cupjudge && \
chown -R judge /usr/lib/cupjudge

if [ ! -d /home/judge/etc ]; then
mkdir -p /home/judge/etc
cp -r etc/* /home/judge/etc/
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
  cp etc/config.json /home/judge/etc/config.json
  echo "请自行配置config.json的文件内容，将judge.conf的内容填写到config.json"
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