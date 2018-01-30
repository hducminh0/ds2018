Compile step:
rpcgen -c square.x
gcc -c client.c -o client.o
gcc -c square_clnt.c -o square_clnt.o
gcc -c square_xdr.c -o square_xdr.o
gcc -o client client.o square_clnt.o square_xdr.o
gcc -c client.c server.c square_xdr.c
gcc -c square_svc.c -o square_svc.o
gcc -o server server.o square_svc.o square_xdr.o

[root@localhost~]#rpcgen -C square.x
[root@localhost~]#gcc -c client.c -o client.o
[root@localhost~]#gcc -c square_clnt.c -o square_clnt.o
[root@localhost~]#gcc -c square_xdr.c -o square.xdr.o
[root@localhost~]#gcc -o client client.o square_clnt.o square_xdr.o
[root@localhost~]#gcc -c client.c server.c square_xdr.c
[root@localhost~]#gcc -c server.c -o server.o
[root@localhost~]#gcc -c square_svc.c -o square_svc.o
[root@localhost~]#gcc -o server server.o square_svc.o square_xdr.o
[root@localhost~]#./server &
[1] 2264
[root@localhost~]#./client localhost 4
result is: 16


More detail in:
http://www.c4learn.com/c-programming/c-gcc-program-rpc/