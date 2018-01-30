Requirement:
rpcgen
gcc

Compile step:
cd <the-folder-that-u-put-everything-in>
chmod +x Makefile
./Makefile

Usage:
./fileserver &
./fileclient <host> <file>

Exit:
ps -ef | grep server
kill <PID>

Ex:
//make things
[root@localhost~]#cd rpc/
[root@localhost~]#chmod +x Makefile
[root@localhost~]#./Makefile

//Run server in deamon
[root@localhost~]#./fileserver &
[1] 2264
[root@localhost~]#./fileclient localhost cat.jpg
File sent: cat.jpgsv
Byte sent: 289242

//Find and kill server
[root@localhost~]#ps -ef | grep server
  UID   PID  PPID   C STIME   TTY           TIME CMD
  501  6262     1   0 11:34AM ??         0:00.04 ./fileserver
//Get the PID
[root@localhost~]#kill 6262