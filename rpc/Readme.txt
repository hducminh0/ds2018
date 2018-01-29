Compile step:
rpcgen filetrans.x
gcc -c fileclient.c -o fileclient.o
gcc -c filetrans_clnt.c -o filetrans_clnt.o
gcc -c filetrans_xdr.c -o filetrans.xdr.o
gcc -o fileclient fileclient.o filetrans_clnt.o filetrans_xdr.o
gcc -c fileclient.c fileserver.c filetrans_xdr.c
gcc -c fileserver.c -o fileserver.o
gcc -c filetrans_svc.c -o filetrans_svc.o
gcc -o fileserver fileserver.o filetrans_svc.o filetrans_xdr.o

[root@localhost~]#rpcgen -C filetrans.x
[root@localhost~]#gcc -c fileclient.c -o fileclient.o
[root@localhost~]#gcc -c filetrans_clnt.c -o filetrans_clnt.o
[root@localhost~]#gcc -c filetrans_xdr.c -o filetrans.xdr.o
[root@localhost~]#gcc -o fileclient fileclient.o filetrans_clnt.o filetrans_xdr.o
[root@localhost~]#gcc -c fileclient.c fileserver.c filetrans_xdr.c
[root@localhost~]#gcc -c fileserver.c -o fileserver.o
[root@localhost~]#gcc -c filetrans_svc.c -o filetrans_svc.o
[root@localhost~]#gcc -o fileserver fileserver.o filetrans_svc.o filetrans_xdr.o
//Run server in deamon
[root@localhost~]#./fileserver &
[1] 2264
[root@localhost~]#./fileclient localhost test.txt

//Find and kill server
ps -ef | grep server
kill <PID>

[root@localhost~]#ps -ef | grep server
  UID   PID  PPID   C STIME   TTY           TIME CMD
  501  6262     1   0 11:34AM ??         0:00.04 ./fileserver
//Get the PID
[root@localhost~]#kill 6262