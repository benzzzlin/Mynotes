1.on 27 Nov 2018 dpdk-18.11
2.on 16 Nov 2017 dpdk-17.11
v17.11.1	dpdk-stable-17.11.1.zip  dpdk-stable-17.11.1.tar.gz  dpdk-stable-17.11.1.tar.xz  	Yuanhan Liu	16 months 
v18.11.1	dpdk-stable-18.11.1.zip  dpdk-stable-18.11.1.tar.gz  dpdk-stable-18.11.1.tar.xz  	Kevin Traynor	2 months  //http://git.dpdk.org/dpdk-stable/refs


## 17.11.1   -> 18.11

dpdk实验环境部署

1、实验环境说明

vmware workstatioin 12

centos 7.5.1804

dpdk-stable-18.11.1

2、实验步骤

（1）虚拟机安装

　　http://vault.centos.org/7.5.1804/isos/x86_64/从链接下载CentOS-7-x86_64-DVD-1804.iso，安装时需要准备3张虚拟网卡，一个为上网用，另外两个是仅主机模式用于dpdk使用。内存我选择分配3G，cpu 2核。

（2）

1
2
3
4
5
6
7
8
9
依赖下载：<br>yum install -y numactl-devel*x86_64 make gcc libpcap libpcap-devel
 
下载内核头文件：
wget http://vault.centos.org/7.5.1804/os/x86_64/Packages/kernel-devel-3.10.0-862.el7.x86_64.rpm
 
安装内核头文件：
rpm -ivh kernel-devel-3.10.0-862.el7.x86_64.rpm<br>
内核头文件链接到内核模块中
ln -fs /usr/src/kernels/3.10.0-862.el7.x86_64/ /lib/modules/3.10.0-862.el7.x86_64/build/
　（3）编译

1
2
3
make config T=x86_64-native-linuxapp-gcc
sed -ri 's,(PMD_PCAP=).*,\1y,' build/.config
make
　（4）设置大页内存

1
2
3
echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
mkdir /mnt/huge
mount -t hugetlbfs nodev /mnt/huge
　（5）加载驱动

1
2
3
4
modprobe uio_pci_generic
modprobe uio
modprobe vfio-pci
insmod build/kmod/igb_uio.ko
　（6）查看网卡bind信息

1
usertools/dpdk-devbind.py --status
　

   （7）绑定网卡

1
2
3
4
5
ifconfig ens34 down
ifconfig ens35 down
 
./usertools/dpdk-devbind.py --bind=igb_uio  02:02.0
./usertools/dpdk-devbind.py --bind=igb_uio  02:03.0
　（8）查看绑定信息

1
ls /dev/uio* -l
　

  （9）编译examples

1
make -C examples RTE_SDK=$(pwd) RTE_TARGET=build O=$(pwd)/build/examples
　（10）运行helloworld程序

1
./build/examples/helloworld/build/helloworld
　　

bug修正，

1
进入dpdk-stable-18.11.1/
1
2
vim kernel/linux/igb_uio/igb_uio.c<br>将pci_intx_mask_supported(dev)代码修改为如下的
pci_intx_mask_supported(dev)||true<br><br>修改后重新执行<br>make
1
2
3
modprobe uio_pci_generic
modprobe uio
modprobe vfio-pci<br>rmmod igb_uio.ko
1
insmod build/kmod/igb_uio.ko
1
usertools/dpdk-devbind.py --status
1
2
./usertools/dpdk-devbind.py --bind=igb_uio  02:02.0
./usertools/dpdk-devbind.py --bind=igb_uio  02:03.0
1
./build/examples/helloworld/build/helloworld
　

（11）testpmd测试程序

1
./build/app/testpmd -l 0-1 -n 2 -- -i
　　运行结果

1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
[root@localhost dpdk-stable-18.11.1]# ./build/app/testpmd -l 0-1 -n 2 -- -i
EAL: Detected 2 lcore(s)
EAL: Detected 1 NUMA nodes
EAL: Multi-process socket /var/run/dpdk/rte/mp_socket
EAL: No free hugepages reported in hugepages-1048576kB
EAL: Probing VFIO support...
EAL: VFIO support initialized
EAL: PCI device 0000:02:01.0 on NUMA socket -1
EAL:   Invalid NUMA socket, default to 0
EAL:   probe driver: 8086:100f net_e1000_em
EAL: PCI device 0000:02:02.0 on NUMA socket -1
EAL:   Invalid NUMA socket, default to 0
EAL:   probe driver: 8086:100f net_e1000_em
EAL: PCI device 0000:02:03.0 on NUMA socket -1
EAL:   Invalid NUMA socket, default to 0
EAL:   probe driver: 8086:100f net_e1000_em
Interactive-mode selected
testpmd: create a new mbuf pool <mbuf_pool_socket_0>: n=155456, size=2176, socket=0
testpmd: preferred mempool ops selected: ring_mp_mc
Configuring Port 0 (socket 0)
Port 0: 00:0C:29:D3:37:3B
Configuring Port 1 (socket 0)
Port 1: 00:0C:29:D3:37:45
Checking link statuses...
Done
testpmd> start                                         //start后，等个10s,再执行stop
io packet forwarding - ports=2 - cores=1 - streams=2 - NUMA support enabled, MP allocation mode: native
Logical Core 1 (socket 0) forwards packets on 2 streams:
  RX P=0/Q=0 (socket 0) -> TX P=1/Q=0 (socket 0) peer=02:00:00:00:00:01
  RX P=1/Q=0 (socket 0) -> TX P=0/Q=0 (socket 0) peer=02:00:00:00:00:00
 
  io packet forwarding packets/burst=32
  nb forwarding cores=1 - nb forwarding ports=2
  port 0: RX queue number: 1 Tx queue number: 1
    Rx offloads=0x0 Tx offloads=0x0
    RX queue: 0
      RX desc=256 - RX free threshold=0
      RX threshold registers: pthresh=0 hthresh=0  wthresh=0
      RX Offloads=0x0
    TX queue: 0
      TX desc=256 - TX free threshold=0
      TX threshold registers: pthresh=0 hthresh=0  wthresh=0
      TX offloads=0x0 - TX RS bit threshold=0
  port 1: RX queue number: 1 Tx queue number: 1
    Rx offloads=0x0 Tx offloads=0x0
    RX queue: 0
      RX desc=256 - RX free threshold=0
      RX threshold registers: pthresh=0 hthresh=0  wthresh=0
      RX Offloads=0x0
    TX queue: 0
      TX desc=256 - TX free threshold=0
      TX threshold registers: pthresh=0 hthresh=0  wthresh=0
      TX offloads=0x0 - TX RS bit threshold=0
testpmd> stop
Telling cores to stop...
Waiting for lcores to finish...
 
  ---------------------- Forward statistics for port 0  ----------------------
  RX-packets: 3087933        RX-dropped: 0             RX-total: 3087933
  TX-packets: 3273024        TX-dropped: 0             TX-total: 3273024
  ----------------------------------------------------------------------------
 
  ---------------------- Forward statistics for port 1  ----------------------
  RX-packets: 3273028        RX-dropped: 0             RX-total: 3273028
  TX-packets: 3087929        TX-dropped: 0             TX-total: 3087929
  ----------------------------------------------------------------------------
 
  +++++++++++++++ Accumulated forward statistics for all ports+++++++++++++++
  RX-packets: 6360961        RX-dropped: 0             RX-total: 6360961
  TX-packets: 6360953        TX-dropped: 0             TX-total: 6360953
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
Done.
testpmd> quit
 
Stopping port 0...
Stopping ports...
Done
 
Stopping port 1...
Stopping ports...
Done
 
Shutting down port 0...
Closing ports...
Done
 
Shutting down port 1...
Closing ports...
Done
 
Bye...