apt install build-essential cmake clang fp-compiler gcc g++ libgmp-dev libmysqlclient-dev
cmake ./ && make && chmod a+x judge_client && mv ./judge_client /usr/bin
mkdir /home/judge
mkdir /home/judge/etc
mkdir /dev/shm/cupoj
mkdir /dev/shm/cupoj/submission
useradd -m -u 1536 judge
cp -r etc/* /home/judge/etc/ && \
chown -R judge /home/judge && \
chgrp -R judge /home/judge && \
cd model/judge/language && \
cmake ./ && \
make && \
cd build/out && \
mkdir -p /usr/lib/cupjudge && \
cp * /usr/lib/cupjudge/ && \
cd ../../../../../