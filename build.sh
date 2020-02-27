cmake ./ && make && chmod a+x judge_client && mv ./judge_client /usr/bin
mkdir /home/judge
mkdir /home/judge/etc
cp -r etc/* /home/judge/etc/