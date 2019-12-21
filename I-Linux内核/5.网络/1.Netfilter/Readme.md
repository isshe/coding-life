
# Readme
> 开局一张图，内容全靠编~

![](./Netfilter-packet-flow.svg)

# 1. Netfilter Hook
> 参见：<netfilter.h>/<netfilter_ipv4.h>/<netfilter_ipv6.h>

## 1.1 挂钩点
* `NF_IP_PRE_ROUTING`: `接收`到的包进入协议栈/完整性校验后，在进行选路（将包发往哪里）之前。
* `NF_IP_LOCAL_IN`: `接收`到的包经过选路后，目的是本机则触发此 hook。
* `NF_IP_FORWARD`: `接收`到的包经过选路后，目的不是本机则触发此 hook。
* `NF_IP_LOCAL_OUT`: 本机进程产生的准备`发送`的包，在进入协议栈后立即触发此 hook。
* `NF_IP_POST_ROUTING`: `发送`或者`转发`的包，在经过选路(且在`NF_IP_FORWARD`或`NF_IP_LOCAL_OUT`)后触发此 hook。

## 1.2 优先级
* 注册处理函数的时候，必须提供优先级。
* 优先级数值越大优先级越低。
* 内核定义的优先级：
```c
enum nf_ip_hook_priorities {
	NF_IP_PRI_FIRST = INT_MIN,
	NF_IP_PRI_RAW_BEFORE_DEFRAG = -450,
	NF_IP_PRI_CONNTRACK_DEFRAG = -400,
	NF_IP_PRI_RAW = -300,
	NF_IP_PRI_SELINUX_FIRST = -225,
	NF_IP_PRI_CONNTRACK = -200,
	NF_IP_PRI_MANGLE = -150,
	NF_IP_PRI_NAT_DST = -100,
	NF_IP_PRI_FILTER = 0,
	NF_IP_PRI_SECURITY = 50,
	NF_IP_PRI_NAT_SRC = 100,
	NF_IP_PRI_SELINUX_LAST = 225,
	NF_IP_PRI_CONNTRACK_HELPER = 300,
	NF_IP_PRI_CONNTRACK_CONFIRM = INT_MAX,
	NF_IP_PRI_LAST = INT_MAX,
};
```

# 2. Tables and Chains(表和链)
`iptables`通过`table(表)`来组织规则，在table内部，规则被进一步组织为`chain(链)`。`内置的chain`的名称和`netfilter hook`名称是一一对应的。

## 2.1 table(表)分类
* `filter`: 用于判断是否让一个包通过（让包按预期通过还是丢弃/拒绝）。
    * filter表示最常用的表之一。
* `nat`：用于实现`网络地址转换`规则。
    * 当数据包进入协议栈时，这些规则决定是否修改以及如何修改包的源/目的地址，以影响包的路由方式。
    * nat表通常用于将包路由到无法直接访问的网络。（打洞之类的？！）
* `mangle`: 用于修改包的IP头。
    * 例如：修改数据包的`TTL(生存时间)`值，以增加或减少数据包可以经过的跳数。
    * 这个table还可以打`只在内核内有效`的标记，后续的table或其他工具可以使用这些标记。标记不会影响到数据包本身。
* `raw`: 提供一个机制，让打了标记的包可以有选择地进入[/退出]连接跟踪。
    * iptable防火墙是有状态的，意味着会评估当前包和之前的包的关系。
    * 建立在netfilter上的连接跟踪使得iptables可以将包看作已有连接/会话的一部分，而不是一个由独立、不相关的包组成的流。
* `security`: 给包打上`SELinux`标记，以影响SELinux或其他可以解读SELinux安全上下文的系统处理包的行为。
    * 这些标记可以基于单个包，也可以基于连接。

## 2.2 chain(链)
> 注意：内置的链和挂钩(hook)一一对应

|   表↓/链→    | PREROUTING | INPUT | FORMARD | OUTPUT | POSTROUTING |
| :----------: | :--------: | :---: | :-----: | :----: | :---------: |
| （路由判断） |            |       |         |   ✓    |             |
|     raw      |     ✓      |       |         |   ✓    |             |
|  (连接跟踪)  |     ✓      |       |         |   ✓    |             |
|    mangle    |     ✓      |   ✓   |    ✓    |   ✓    |      ✓      |
|   nat-DNAT   |     ✓      |       |         |   ✓    |             |
| （路由判断） |     ✓      |       |         |   ✓    |             |
|    filter    |            |   ✓   |    ✓    |   ✓    |             |
|   security   |            |   ✓   |    ✓    |   ✓    |             |
|   nat-SNAT   |            |   ✓   |         |        |      ✓      |
* 数据包的处理流程大致有以下几种：
    * 收到目的为本机的包：`PREROUTING -> INPUT`。
    * 收到目的不为本机的包：`PREROUTING -> FORWARD -> POSTROUTING`。
    * 本机进程发送的包：`OUTPUT -> POSTROUTING`。
* 不同的table在同一个hook(列)的优先级从上到下。例如：POSTROUTING中，先mangle，在nat-SNAT。
* 触发哪个hook(列)和数据包的方向(ingress/egress)、路由判断、过滤条件等有关。
* 特定事件会导致table中的chain被跳过。
    * 例如，只有每个连接的第一个包会去匹配NAT规则，对这个包的动作会应用于这个连接后的所有包。

## 2.3 用户自定义链
只能通过另一个规则跳转到自定义的chain，因为自定义的chain没有注册到netfilter hook。
跳转需要用到：`跳转目标(jump targets)`——一种特殊的`非终止目标`(下文有解释)，作用是跳到另一个chain继续处理动作。

# 3. iptables
* iptables规则分为两部分：`匹配`部分和`目标`部分。(想想内核中的target和match相关的内容！)
    * 匹配：数据包满足规则的匹配部分指定的条件，就会针对包执行相应的动作部分。
        * 规则可以匹配：协议类型、目的地址/端口、源地址/端口、接收/发送的网口、连接状态等等。
    * 目标：匹配后出发的`动作`，叫做`目标`。目标分为两种：
        * 终止目标(terminating targets)：终止chain的匹配，将控制权转移回netfilter hook。根据返回值的不同，hook将数据包丢弃或进行下一阶段的处理。
        * 非终止目标(non-terminating targets): 继续chain的匹配。
    * 每个chain最终都会回到一个终止目标，但是在这之前可以执行任意多的非终止目标。
* 当chain被调用的时候，包会依次匹配chain里面的规则，规则优先级从上到下(`iptables -t filter -L`可以看到)。

# 4. 连接跟踪
* 构建在netfilter上的连接跟踪系统。
* 连接跟踪在包进入协议栈之后很快就会开始工作。
* 在给数据包分配连接之前只检查`raw`表和一些基本的完整性检查。
* 连接跟踪系统将数据包和连接进行比较，如果数据包存在所属的连接，则更新连接状态；否则，就创建一个新的连接。
* 如果`raw`表的某个chain对数据包标记为目标是`NOTRACK`，则这个包会跳过连接跟踪系统。

## 4.1 连接跟踪和iptables
连接跟踪使得iptables基于`连接上下文`而不是`单个包`来做出规则判断，给iptables提供了有状态操作的功能。【？？？】

## 4.2 连接的状态
连接跟踪系统中的连接状态有：
* `NEW`：到达的数据包是合法的，并且没有匹配到任何已有连接，就位这个数据包创建一个新连接。
    * TCP和UDP都适用。（和协议是否面向连接无关）
* `ESTABLISHED`：一个连接收到应答方向的合法数据包，状态就会从`NEW`转换为`ESTABLISHED`。
    * 对于TCP，这个合法数据包就是`SYN/ACK`。
    * 对于UDP/ICMP，就是源和目的IP相反的数据包。
* `RELATED`：数据包不属于已有的连接，但是和已有连接有一定关系，可能是辅助连接（helper connection）。
    * 如，FTP数据传输连接；
    * 如，其他协议试图建立连接时的ICMP应答包。
* `INVALID`：数据包不属于已有的连接，并且由于某些原因不能用于创建新连接。
    * 如，无法识别、无法路由的数据包。
* `UNTRACKED`：在`raw`表中被标记为目标`UNTRACKED`(应该是`NOTRACK`？)的数据包，这个包不会进入连接跟踪系统。
* `SNAT`：进行SNAT后进入的`虚拟状态`。由连接跟踪系统使用，以便知道在回复数据包中更改`源地址`。
* `DNAT`：进行DNAT后进入的`虚拟状态`。由连接跟踪系统使用，以便知道在`路由(routing)`回复数据包时更改`目标地址`。

# A. 参考
* [A Deep Dive into Iptables and Netfilter Architecture](https://www.digitalocean.com/community/tutorials/a-deep-dive-into-iptables-and-netfilter-architecture)
* [【译】深入理解 iptables 和 netfilter 架构](https://arthurchiao.github.io/blog/deep-dive-into-iptables-and-netfilter-arch-zh/)

# B. 拓展
* [更多iptable使用方式](https://www.digitalocean.com/community/tutorials/how-to-choose-an-effective-firewall-policy-to-secure-your-servers)