# Elastic-TCP: Flexible Congestion Control Algorithm to Adapt for High-BDP Networks

The paper: https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=8642512

## Test environment
* NetEm on receiver side
  - `qdisc netem 8004: dev ifb0 root refcnt 2 limit 865 delay 100.0ms rate 100Mbit`
* 100Mbps
* 100ms RTT

Test result:

![image](https://user-images.githubusercontent.com/980728/125387081-eedb0a80-e3cf-11eb-92a1-6f6dfdcce280.png)

compared to RENO

![image](https://user-images.githubusercontent.com/980728/130021405-a16e6be4-296d-43d3-b3d9-74635aa5305c.png)

compared to CUBIC

![image](https://user-images.githubusercontent.com/980728/130021501-8f97a30b-49a0-43d0-9695-5599ebfa03a4.png)

