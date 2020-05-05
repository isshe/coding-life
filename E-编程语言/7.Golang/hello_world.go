package main

import (
	"crypto/hmac"
	"crypto/sha1"
	"encoding/json"
	"fmt"
	"time"
)

type CmdRequest struct {
	RequestTime int64  `json:"request_time"`
	Cmd         string `json:"cmd"`
	Timeout     int    `json:"timeout"`
}

func main() {
	cmd := fmt.Sprintf("%s", "bypass")
	timeout := 10
	body, err := json.Marshal(&CmdRequest{
		RequestTime: 1587273257, // time.Now().Unix(),
		Cmd:         cmd,
		Timeout:     timeout,
	})
	if err != nil {
		fmt.Println("error!")
		return
	}
	fmt.Println("body = ", body)
	mac := hmac.New(sha1.New, []byte("abc"))
	mac.Write(body)
	token := fmt.Sprintf("%x", mac.Sum(nil))
	fmt.Println("mac = ", mac)
	fmt.Println(mac.Sum(nil))
	fmt.Println("token = ", token)
	fmt.Println(time.Now().Unix())
}
