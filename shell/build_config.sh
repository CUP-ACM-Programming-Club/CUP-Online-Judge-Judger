#!/bin/bash
CPU_CORE=`nproc`
HOSTNAME=''
USERNAME=''
PASSWORD=''
PORT=3306
BUF=''
echo "请输入hostname，即mysql的ip/url, 默认为127.0.0.1";
read BUF
if [ "$BUF" = '' ]; then
  HOSTNAME="127.0.0.1"
else
  HOSTNAME=$BUF
fi
echo "请输入数据库账号"
read USERNAME

echo "请输入密码"
read PASSWORD

echo "请输入端口号"
read PORT

cat >/home/judge/etc/config.json <<-EOF {
  "hostname": "$HOSTNAME",
  "username": "$USERNAME",
  "password": "$PASSWORD",
  "db_name": "jol",
  "port": $PORT,
  "running": $CPU_CORE,
  "sleep_time": 4,
  "total": 1,
  "mod": 0,
  "java_time_bonus": 2,
  "java_memory_bonus": 128,
  "java_xms": "-Xms64M",
  "java_xmx": "-Xmx512M",
  "sim_enable": 1,
  "full_diff": 1,
  "judger_name": "RATH",
  "shm_run": 0,
  "use_max_time": 1,
  "use_ptrace": 1
}
EOF

echo "config.json文件已写入到/home/judge/etc。内容为"
cat /home/judge/etc/config.json