# signald

*开发中*

这是一个 Unix 系统下的后台程序，开发它只是因为我遇到了形如以下形式的困难。

在 mac 中。我使用 nginx 并将其 master 进程运行在 root 用户下。我需要使用 logrotate 切割 nginx 日志，但 launchctl 定时执行 logrotate 时将当前用户权限赋予它，导致其无权执行 kill -USR1 nginx 无法重新打开日志文件。

本程序旨在解决该类问题，允许指定用户将指定用户的指定信号以特定形式发送给（不限于本用户的）进程。

### TODO

- 配置文件
- 权限分配
- 日志