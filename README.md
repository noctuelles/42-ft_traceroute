# ft_traceroute

## Principle

The traceroute program is used to trace a packet path to a network host. The GNU **traceroute** program from *inetutils* by default, works by sending an **UDP** datagram to the network host (called **probe**). At the beginning, the **TTL** (time-to-live) of the probes are set to 1 : everytime a router will receive an IP datagram, the **TTL** value is going to be decremented.

If the **TTL** value is equals to *zero*, the packet will be discarded, and an error is gonna be reported to the sender, and this where the **ICMP** protocol comes into play ; for such error, an ICMP of type 11 (*Time Exceeded*) code 0 (*Time-to-Live Exceeded in Transit*) is send back to the origin sender.

By incrementing the **TTL** value of the probes, we will in theory, receive for each router crossed by the packet, an ICMP message, and we will be able display the IP address of the router.
Each router encountered is called a **hop**, and the time taken to reach the router and receiving the ICMP error message is called the **RTT** (Round-Trip Time).

If a probe reach the destination and if the port **is not in use** on the network host, we should receive an ICMP of type 1 (*Destination Unreachable*) code 3 (*Port Unreachable*), ending the traceroute program.

![The traceroute program](./assets/readme/traceroute.jpg)

By default, the **traceroute** program sends 3 probes per hop, and each time an ICMP error message for a probe is received, the packet RTT is displayed. If the response message is not seen under *3 secondes*, the programs prints an `*`.

The algorithm is very simple to understand ! But, there's...

## Many things to consider :

UDP is a simple, unreliable datagram protocol. There'is absolutly no guarantee that such datagram ever reach their intended destination, and received in the order they are sent.

Also, by opening an ICMP socket with the `socket` syscall (`socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)`), the kernel will pass to the user-process all the ICMP messages received by the host, and that includes ICMP messages not related to the traceroute program.

### The content of an ICMP Error Message

<ins>Each ICMP error message is mandated to contains :</ins>

![IMCP Err](https://www.computernetworkingnotes.com/wp-content/uploads/networking-tutorials/images/nt21-01-icmp-header.png)

- The original IP header of the packet that caused the error.
- The first 8 bytes of the original datagram that caused the error.

By reading the ICMP error message content, we can see :

- If the ICMP message is not of type 11 code 0.
- If failed ip packet transport protocol is not UDP.

But this is not enough : what if another program on host computer, sent UDP datagram to the same network host (like another **traceroute** running in parralel for example) and lead to a router sending ICMP type 11 code 0, encapsulating UDP ? How can we differenciate ?

To treat this issue, the technique is to **bind** a particular port to the outgoing datagram : we take the 16 bit value of the **PID** of the **traceroute** process instance, and set the high order byte to one. Translated to C code on an *Unix* OS, this gives :

```
source_port = htons((getpid() & 0xFFFF) | (1 << 15))
```

This way, we can easely identify if an ICMP error message is indeed coming from our **traceroute**.

If we launch the *GNU inetutils* **traceroute** distinctively two times and examine the packets with **Wireshark**, we can see that the source port is different :

![](./assets/readme/traceroute_sport_1.png)
![](./assets/readme/traceroute_sport_2.png)

Do you think you're out of the woods ? There's still one **issue**.