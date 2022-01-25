// +build linux

package main

import (
	"flag"
	"fmt"
	"os"
	"syscall"
	"time"

	"github.com/cilium/ebpf"
)

func main() {
	const SO_ATTACH_BPF = 50
	index := flag.Int("index", 1, "specify ethernet index")
	flag.Parse()

	args := os.Args[1:]
	bpfLoadProg := "bpf_program.o"
	if len(args) > 1 {
		bpfLoadProg = args[0]
	}

	fmt.Println("We arg going to load prog:  ", bpfLoadProg)
	collSpce, err := ebpf.LoadCollectionSpec(bpfLoadProg)
	if err != nil {
		fmt.Println("Load ELF BPF file failed for ", bpfLoadProg, err.Error())
		return
	}

	fmt.Printf("%#v\n", collSpce.Programs["socket_prog"])

	var objs struct {
		Prog  *ebpf.Program `ebpf:"socket_prog"`
		Stats *ebpf.Map     `ebpf:"countmap"`
	}

	if err := collSpce.LoadAndAssign(&objs, nil); err != nil {
		panic(err)
	}
	defer objs.Prog.Close()
	defer objs.Stats.Close()

	sock, err := openRawSock(*index)
	if err != nil {
		panic(err)
	}
	defer syscall.Close(sock)

	if err := syscall.SetsockoptInt(sock, syscall.SOL_SOCKET, SO_ATTACH_BPF, objs.Prog.FD()); err != nil {
		panic(err)
	}

	fmt.Printf("Filtering on eth index: %d\n", *index)
	fmt.Println("Packet stats:")

	for {
		const (
			ICMP = 0x01
			TCP  = 0x06
			UDP  = 0x11
		)

		time.Sleep(time.Second)
		var icmp uint32
		var tcp  uint32
		var udp  uint32
/*
360: array  name countmap  flags 0x0
	key 4B  value 4B  max_entries 256  memlock 4096B
*/
		err := objs.Stats.Lookup(uint32(ICMP), &icmp)
		if err != nil {
			panic(err)
		}
		err = objs.Stats.Lookup(uint32(TCP), &tcp)
		if err != nil {
			panic(err)
		}
		err = objs.Stats.Lookup(uint32(UDP), &udp)
		if err != nil {
			panic(err)
		}
		fmt.Printf("\r\033[m\tICMP: %d TCP: %d UDP: %d", icmp, tcp, udp)
	}
}

func openRawSock(index int) (int, error) {
	const ETH_P_ALL uint16 = 0x300

	sock, err := syscall.Socket(syscall.AF_PACKET, syscall.SOCK_RAW|syscall.SOCK_NONBLOCK|syscall.SOCK_CLOEXEC, int(ETH_P_ALL))
	if err != nil {
		return 0, err
	}
	sll := syscall.SockaddrLinklayer{}
	sll.Protocol = ETH_P_ALL
	sll.Ifindex = index
	if err := syscall.Bind(sock, &sll); err != nil {
		return 0, err
	}
	return sock, nil
}

