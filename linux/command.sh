valgrind --error-limit=no --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --log-file=./mem.log
tcpdump -i eth0 'port 3000' -X -c 3
ulimit -c unlimited

tailf log/log_center.txt

patchelf 去掉开源动态库的soname
