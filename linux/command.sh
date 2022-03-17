valgrind --error-limit=no --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --log-file=./mem.log
tcpdump -i eth0 'port 3000' -X -c 3
ulimit -c unlimited

tailf log/log_center.txt

patchelf 去掉开源动态库的soname

网络端口转发：
    iptables -t filter -P FORWARD ACCEPT
    vi /etc/sysctl.conf  ->    net.ipv4.ip_forward =1
    iptables -t nat -A PREROUTING -d 30.30.3.216 -p tcp -dport 9092 -j DNAT --to 192.0.24.221:22
    iptables -t nat -A POSTROUTING -p -tcp -j MASQUREAD
dd烧写：
    sudo fdisk -u -l /dev/sdc
    dd bs=512 count=[end +1] if=/dev/sdc | gzip -6 > xxx.img
    gzip -dc ./xxx.img | dd of=/dev/sdb
