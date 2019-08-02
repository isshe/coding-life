[TOC]

# tcpdump
tcpdump - dump traffic on a network（转储网络上的流量）。

## 用法
```bash
tcpdump [ -AbdDefhHIJKlLnNOpqStuUvxX# ] [ -B buffer_size ]
       [ -c count ]
       [ -C file_size ] [ -G rotate_seconds ] [ -F file ]
       [ -i interface ] [ -j tstamp_type ] [ -m module ] [ -M secret ]
       [ --number ] [ -Q in|out|inout ]
       [ -r file ] [ -V file ] [ -s snaplen ] [ -T type ] [ -w file ]
       [ -W filecount ]
       [ -E spi@ipaddr algo:secret,...  ]
       [ -y datalinktype ] [ -z postrotate-command ] [ -Z user ]
       [ --time-stamp-precision=tstamp_precision ]
       [ --immediate-mode ] [ --version ]
       [ expression ]
```

## 选项
```bash
       -A     以ASCII格式打印每个数据包（减去其链接级别标题）。方便捕获网页。

       -b     以ASDOT表示法而不是ASPLAIN表示法打印BGP数据包中的AS编号。

       -B buffer_size
       --buffer-size=buffer_size
              将操作系统捕获缓冲区大小设置为buffer_size。

       -c count
              收到count个包后退出。

       -C file_size
              在将原始数据包写入保存文件之前，检查该文件当前是否大于file_size，如果是，关闭当前保存文件并打开一个新文件。
              第一个文件之后的文件将使用 -w 标志指定名称 ，后面带一个数字，从1开始向上。
              file_size的单位是数百万字节（1,000,000字节，而不是1,048,576字节）。

       -d     将编译后的数据包匹配代码以人类可读的形式转储到标准输出并停止。

       -dd    将数据包匹配代码转储为 C 程序片段。

       -ddd   将数据包匹配代码转储为十进制数字（以计数开头）。

       -D
       --list-interfaces
              打印系统上可用的网络接口列表以及 tcpdump 可以捕获数据包的列表。
              对于每个网络接口，打印数字和接口名称，可能后跟接口的文本描述。
              可以将接口名称或编号提供给 -i 标志以指定要捕获的接口。
              这对于没有列出命令的系统很有用。
              如果tcpdump是用缺少pcap_findalldevs()的libpcap建立的，则不支持此选项(-D)。

       -e     在每个转储行上打印链接级标题。例如，这可以用于打印诸如以太网和IEEE 802.11之类的协议的MAC层地址。

       -E     使用 spi@ipaddr algo:secret来解密发送到addr并包含安全参数索引值 spi的 IPsec ESP数据包。
              可以用逗号或换行符分隔重复该组合。
              请注意，此时支持为IPv4 ESP数据包设置机密。
              算法可以是 des-cbc， 3des-cbc， blowfish-cbc， rc3-cbc， cast128-cbc或 none。默认为des-cbc。
              只有在启用加密编译tcpdump的情况下才能解密数据包。
              secret是ESP密钥的ASCII文本。如果前面带有0x，则将读取十六进制值。
              此选项在RFC2406提出，不是在RFC1827。
              该选项仅用于调试目的，不鼓励使用此选项和真正的“secret”密钥。通过在命令行上显示IPsec密钥，您可以通过ps和其他场合将其显示给其他人。
              除了上面的语法之外，语法文件名可以用来让tcpdump读取提供的文件。
              文件在收到第一个ESP数据包后打开，所以tcpdump应该具备所有可能需要的特殊权限。

       -f     以数字方式而不是符号方式打印“外来(foreign)”IPv4地址。
              “外部”IPv4地址的测试是使用正在进行捕获的接口的IPv4地址和网络掩码完成的。

       -F file
              使用file作为过滤器表达式的输入。命令行上给出的附加表达式将被忽略。

       -G rotate_seconds
              如果指定，每rotate_second秒回转(rotate)-w指定的文件。【回转？？？】
              保存文件将具有-w指定的名称，该名称 应包含strftime定义的时间格式。
              如果未指定时间格式，则每个新文件都将覆盖前一个。
              每当生成的文件名不唯一时，tcpdump将覆盖预先存在的数据; 因此，不建议提供比捕获周期更粗糙的时间规范。
              如果与 -C 选项一起使用，文件名将采用"file<count>"的形式。

       -h
       --help 打印tcpdump和libpcap版本字符串，打印用法消息，然后退出。
       --version
              打印tcpdump和libpcap版本字符串并退出。

       -H     尝试检测802.11s草图网格标头。

       -i interface
       --interface=interface
              指定监听的接口。
              如果未指定，tcpdump将在系统接口列表中搜索编号最小的已配置接口（不包括环回），这可能会变成例如“eth0”。
              在具有2.2或更高版本内核的Linux系统上， 可以使用“any” 的 接口参数来捕获来自所有接口的数据包。
              请注意，“any”设备上的捕获将不会以混杂模式完成。
              如果 支持-D标志，则如果系统上没有任何接口将该数字作为名称，则该标志打印的接口号可用作 接口参数。

       -I
       --monitor-mode
              将接口置于“监控模式(monitor mode)”; 这仅在IEEE 802.11 Wi-Fi接口上受支持，并且仅在某些操作系统上受支持。
              请注意，在监视器模式下，适配器可能与与其关联的网络取消关联，因此您将无法使用具有该适配器的任何无线网络。
              如果您在监视器模式下捕获并且未使用其他适配器连接到另一个网络，则可能会无法访问网络服务器上的文件或解析主机名或网络地址。
              该标志将影响-L 标志的输出 。
              如果未指定-I，则仅显示未处于监控模式时可用的链路层类型; 如果指定-I，则仅显示处于监视模式时可用的链接层类型。

       --immediate-mode
              在“立即模式(immediate mode)”下进行数据包捕获。
              在此模式下，数据包一到达就会传送到tcpdump，而不是为了提高效率而进行缓冲。
              这是打印数据包时的默认设置。

       -j tstamp_type
       --time-stamp-type=tstamp_type
              将捕获的时间戳类型设置为tstamp_type。
              pcap-tstamp中给出时间戳类型的名称; 并非所有列出的类型都必须对任何给定的接口有效。

       -J
       --list-time-stamp-types
              列出接口支持的时间戳类型并退出。如果无法为接口设置时间戳类型，则不会列出时间戳类型。

       --time-stamp-precision=tstamp_precision
              捕获时，将捕获的时间戳精度设置为 tstamp_precision。
              请注意，高精度时间戳（纳秒）的可用性及其实际精度取决于平台和硬件。
              还要注意，当以纳秒级精度将捕获写入保存文件时，时间戳以纳秒分辨率写入，并且文件使用不同的幻数编写，以指示时间戳以秒和纳秒为单位;
              并非所有读取pcap文件的程序都能读取这些捕获。
              读取保存文件时，将时间戳转换为timestamp_precision指定的精度，并以该分辨率显示它们。
              如果指定的精度小于文件中时间戳的精度，则转换将失去精度。
              为支持的值timestamp_precision是微为微秒分辨率和纳米为十亿分之一秒分辨率。
              默认值为微秒分辨率。

       -K
       --dont-verify-checksums
              不尝试验证IP，TCP或UDP校验和。
              这对于在硬件中执行部分或全部校验和计算的接口非常有用; 
              否则，所有传出的TCP校验和都将被标记为错误。

       -l     使stdout行缓冲。 如果您想在捕获数据时查看数据，则非常有用。
              例如：tcpdump -l | tee dat
              或：  tcpdump -l > dat & tail -f dat

              请注意，在Windows上，“行缓冲”意味着“无缓冲”，因此如果指定了-l，WinDump将单独写入每个字符。
              -U 在其行为上类似于 -l，但它会导致输出为“数据包缓冲” 因此输出在每个数据包的末尾而不是在每行的末尾写入stdout; 
              这是在所有平台上缓冲的，包括Windows。
       -L
       --list-data-link-types
              在指定模式下列出接口的已知数据链接类型，然后退出。
              已知数据链接类型列表可能取决于指定的模式;
              例如，在某些平台上，Wi-Fi接口可能在不处于监控模式时支持一组数据链路类型和处于监控模式时的另一组数据链路类型。

       -m module
              从文件模块加载SMI MIB模块定义。可以多次使用此选项将多个MIB模块加载到tcpdump中。

       -M secret
              使用secret作为共享密钥，使用TCP-MD5选项（RFC 2385）验证TCP段中的摘要（如果存在）。

       -n     不要将地址（即主机地址，端口号等）转换为名称。

       -N     不要打印主机名的域名限定。例如，如果你指定这个标记，那么tcpdump将打印"nic"而不是"nic.ddn.mil"。

       -#
       --number
              在行的开头打印一个可选的包号。

       -O
       --no-optimize
              不要运行数据包匹配代码优化器。仅当您怀疑优化器中存在错误时，此选项才有用。

       -p
       --no-promiscuous-mode
              不要将接口置于混杂模式。
              请注意，由于某些其他原因，接口可能处于混杂模式; 
              因此，"-p"不能用作"ether host {local-hw-addr}"或"ether broadcast"的缩写。

       -Q direction
       --direction=direction
              选择发送/接收方向上的哪些数据包应该被捕获。可能的值是“in”，“out”和“inout”。并非适用于所有平台。

       -q     快速（安静？）输出。打印较少的协议信息，因此输出线更短。

       -r file
              从文件读取数据包（使用 -w 选项或其他编写pcap或pcapng文件的工具创建 ）。如果文件是"-"，则使用标准输入。

       -S
       --absolute-tcp-sequence-numbers
              打印绝对而非相对的TCP序列号(sequence numbers)。默认是相对。

       -s snaplen
       --snapshot-length=snaplen
              Snarf snaplen来自每个数据包的数据字节而不是默认值262144字节。
              由于快照有限而被截断的数据包在输出中用"[|proto]"，其中proto 是发生截断的协议级别的名称。
              请注意，拍摄较大的快照会增加处理数据包所需的时间，并有效地减少数据包缓冲量。这可能会导致数据包丢失。
              另请注意，拍摄较小的快照会丢弃传输层上方协议的数据，这会丢失可能很重要的信息。
              例如，NFS和AFS请求和回复非常大，如果选择了太短的快照长度，则很多细节将不可用。
              如果您需要将快照大小减小到默认值以下，则应将snaplen限制为捕获您感兴趣的协议信息的最小数量。
              将snaplen设置 为0会将其设置为默认值262144，以便向后兼容最近的旧版本tcpdump的版本 。

       -T type
              强制将"expression"选择的数据包解释为指定的类型。
              目前已知的类型是 aodv（Ad-hoc按需距离矢量协议），carp（通用地址冗余协议），
              cnfp（Cisco NetFlow协议）， lmp（链路管理协议）， pgm（实用通用多播），
              pgm_zmtp1（ZMTP/1.0内部PGM/EPGM），resp（REdis序列化协议）， radius（RADIUS），
              rpc（远程过程调用）， rtp（实时应用程序协议）， rtcp（实时应用程序控制协议），
              snmp（简单网络管理协议）， tftp（普通文件传输协议）， 增值税（视觉音频工具），
              wb（分布式白板）， zmtp1（ZeroMQ消息传输协议1.0）和 vxlan（虚拟可扩展局域网）。
              请注意，上面的pgm类型仅影响UDP解释，无论如何，本机PGM始终被识别为IP协议113。
              UDP封装的PGM通常称为"EPGM"或"PGM/UDP"。
              请注意，上面的pgm_zmtp1类型会同时影响本机PGM和UDP的解释。
              在本机PGM解码期间，ODATA/RDATA分组的应用数据将被解码为具有ZMTP/1.0帧的ZeroMQ数据报。
              在UDP解码期间，除了任何UDP分组之外，任何UDP分组都将被视为封装的PGM分组。

       -t     不在每个转储行上打印时间戳。

       -tt    打印时间戳，自1970年1月1日00:00:00，UTC以及自该时间以来的每秒分数，在每个转储行上。

       -ttt   在每个转储行上的当前行和上一行之间 打印增量（微秒或纳秒分辨率，具体取决于--time-stamp-precision选项）。
              默认值为微秒分辨率。

       -tttt  在每个转储行上打印一个时间戳，以小时，分钟，秒和从午夜开始的一小时一秒为止。

       -ttttt 在每个转储行上的当前行和第一行之间打印增量（微秒或纳秒分辨率，具体取决于 --time-stamp-precision选项）。默认值为微秒分辨率。
       
       -u     打印未解码的NFS句柄。

       -U
       --packet-buffered
              如果未指定 -w 选项，或者如果指定了 -w 选项但是也指定了 --print 标志，则使打印的数据包输出"packet-buffered"；
              即，当打印每个数据包的内容的描述时，它将被写入标准输出，而不是，在不写入终端时，仅在输出缓冲器填满时写入。
              ( i.e.,  as  the description of the contents of each packet is printed, it
               will be written to the standard output, rather than, when not writing to a  terminal,
               being written only when the output buffer fills.)
              如果指定了 -w 选项，则保存的原始数据包输出"packet-buffered"; 
              即，在保存每个数据包时，它将被写入输出文件，而不是仅在输出缓冲区填满时才写入。
              
              该 -U 如果标志将不被支持, 如果旧版本tcpdump内置的libpcap缺少pcap_dump_flush（3PCAP） 函数。

       -v     解析和打印时，产生（略多）详细输出。 
              例如，打印IP包中的生存时间，标识，总长度和选项。
              还可以启用其他数据包完整性检查，例如验证IP和ICMP标头校验和。
              使用-w 选项写入文件时 ，每秒报告一次捕获的数据包数。

       -vv    更详细的输出。例如，从NFS回复数据包打印其他字段，并完全解码SMB数据包。

       -vvv   更详细的输出。例如，telnet SB ... SE选项全部打印。与 -X 的Telnet选项被印刷在十六进制为好。

       -V file
              从文件中读取文件名列表。如果文件是"-"，则使用标准输入。

       -w file
              将原始数据包写入文件而不是解析并打印出来。
              稍后可以使用-r选项打印它们。如果文件是“ - ”，则使用标准输出。
              如果写入文件或管道，则此输出将被缓冲，因此从文件或管道读取的程序在收到后可能无法在任意时间内看到数据包。
              使用 -U 标志可以在收到数据包后立即写入数据包。
              MIME类型application/vnd.tcpdump.pcap已在IANA注册pcap文件。
              文件扩展名.pcap是最常用的。
              但是，许多操作系统和应用程序将使用扩展（如果存在）并建议添加一个（例如.pcap）
              读取捕获文件时Tcpdump本身不检查扩展名，并且在写入时不添加扩展名（它在文件头中使用幻数）。
              有关文件格式的说明，请参阅 pcap-savefile。

       -W     与 -C 选项一起使用时 ，这将限制创建的文件数量达到指定的数量，并从头开始覆盖文件，从而创建一个“旋转”缓冲区。
              此外，它会将具有足够前导0的文件命名为支持最大文件数，从而允许它们正确排序。
              与 -G 选项一起使用时 ，这将限制创建的旋转转储文件的数量，在达到限制时退出状态0。
              如果两者配合使用 -C 和 -G， 该 -W 选项将目前被忽略，并且只会影响的文件名。

       -x     解析和打印时，除了打印每个数据包的标头外，还要以十六进制格式打印每个数据包的数据（减去其链接级别标题）。
              将打印整个数据包或snaplen字节中较小的一个 。
              请注意，这是整个链路层数据包，因此对于填充（例如以太网）的链路层，当较高层数据包短于所需填充时，也将打印填充字节。

       -xx    解析和打印时，除了打印每个数据包的标头外，还要打印每个数据包的数据， 包括其链接级别标题，以十六进制表示。


       -X     解析和打印时，除了打印每个数据包的标题外，还要以十六进制和ASCII格式打印每个数据包的数据（减去其链接级别标题）。
              这对于分析新协议非常方便。

       -XX    解析和打印时，除了打印每个数据包的标头外，还要以 十六进制和ASCII格式打印每个数据包的数据， 包括其链接级别标题。

       -y datalinktype
       --linktype=datalinktype
              设置要在将数据包捕获到datalinktype时使用的数据链接类型。

       -z postrotate-command
              与-C 或 -G 选项一起使用时 ，这将使 tcpdump 运行"postrotate-command file"，
              其中 file 是每次轮换后关闭的savefile（保存文件，-w创建）。
              例如，指定 -z gzip 或 -z bzip2 将使用gzip或bzip2压缩每个savefile。
              请注意，tcpdump将使用最低优先级并行执行命令，以便不会干扰捕获过程。
              如果您想使用一个本身带有标志或不同参数的命令，您总是可以编写一个shell脚本，
              它将保存文件名作为唯一参数，制作标志和参数排列并执行您想要的命令。

       -Z user
       --relinquish-privileges=user
              如果 tcpdump 以root运行，打开所述捕获装置或输入savefiles之后，
              但在打开任何savefiles输出之前，将用户ID改为user和组ID改为user的主组。
              默认情况下，在编译时也可以启用此行为。

        expression
              选择要转储的数据包。如果没有给出表达式(expression)，则网络上的所有数据包都将被转储。
              否则，只会转储表达式为"true"的数据包。
              有关表达式语法，请参阅 pcap-filter。
              该表达式(expression)参数可被传递给和tcpdump作为单一壳牌参数，或作为多个shell参数，取其更方便。
              通常，如果表达式包含Shell元字符，例如用于转义协议名称的反斜杠，则更容易将其作为单个引用参数传递，
              而不是转义Shell元字符。在解析之前，多个参数与空格连接在一起。
```

## 示例
### 指定接口+主机，写到文件
> tcpdump -i br-lan1 -w x_xxxx.cap -v host xx.xx.xx.xx


### 指定端口范围
> tcpdump -i ens33 udp portrange 67-68 -v

### 捕获和指定主机相关的数据包
> tcpdump host sundown

### 打印指定主机间的流量
> tcpdump host helios and \( hot or ace \)
* helios 和 hot 、 helios 和 ace之间的流量。

### 打印所有本地主机和Berkeley主机间的流量
> tcpdump net ucb-ether

### 通过互联网网关snup打印所有ftp流量(请注意，引用该表达式是为了防止shell（错误地）解释括号）
> tcpdump 'gateway snup and (port ftp or ftp-data)'

### 要打印既不是来自本地主机也不是发往本地主机的流量(如果你通往另一个网络，那么这些东西永远不应该进入你的本地网络)
> tcpdump ip and not net localnet

### 打印涉及非本地主机的每个TCP对话的`开始`和`结束`数据包（SYN和FIN数据包）
> tcpdump 'tcp[tcpflags] & (tcp-syn|tcp-fin) != 0 and not src and dst net localnet'

### 打印与端口80之间的所有IPv4 HTTP数据包
> 仅打印包含数据的数据包，而不打印例如SYN和FIN数据包以及仅ACK数据包
> tcpdump 'tcp port 80 and (((ip[2:2] - ((ip[0]&0xf)<<2)) - ((tcp[12]&0xf0)>>2)) != 0)'

### 打印通过网关snup发送的长度超过576字节的IP数据包
> tcpdump 'gateway snup and ip[2:2] > 576'

### 打印未通过以太网广播或多播发送的IP广播或多播数据包
> tcpdump 'ether[0] & 1 = 0 and ip[16] >= 224'

### 打印所有不是回应请求/回复的ICMP数据包（即不ping数据包）
> tcpdump 'icmp[icmptype] != icmp-echo and icmp[icmptype] != icmp-echoreply'


## 疑问

## 参考
> https://www.tcpdump.org/manpages/tcpdump.1.html
> linux man page