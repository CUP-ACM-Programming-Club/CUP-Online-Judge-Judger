# CUP-Online-Judge-Judger
CUP Online Judge's judger
基于[HUSTOJ](https://github.com/zhblue/hustoj)

## 部署

### 第一次部署
* `git clone`
* 执行目录下的`./build.sh`
* 前往`/home/judge/etc/judge.conf`,修改以下三项:
```
OJ_HOST_NAME= #数据库地址
OJ_USER_NAME= #用户名
OJ_PASSWORD= #密码
```
* 将`wsjudged`移动到`CUP-Online-Judge-Judger-Daemon-Service`下
* 修改`/home/judge/etc/compile.json`中不同语言的编译器所在地址
* 启动`CUP-Online-Judge-Judger-Daemon-Service`
