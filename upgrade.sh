#!/bin/bash
git fetch --all && \
git reset --hard origin/master && \
cmake ./ && \
make && \
chmod a+x judge_client && \
mv judge_client /usr/bin && \
cd model/judge/language && \
cmake ./ && \
make && \
cd build/out && \
mkdir -p /usr/lib/cupjudge && \
cp * /usr/lib/cupjudge/ && \
cd ../../../../../
