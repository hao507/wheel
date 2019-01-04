package utils

import (
	"os"
	"fmt"
	"golang.org/x/crypto/ssh"
	"net"
)

func main() {
	check := func(err error, msg string) {
		if err != nil {
			panic(err)
		}
	}

	client,err := ssh.Dial("tcp", "127.0.0.1:22", &ssh.ClientConfig{
		User: "zzy",
		Auth: []ssh.AuthMethod{ssh.Password("386143717")},
		HostKeyCallback: func(hostname string, remote net.Addr, key ssh.PublicKey) error {
			fmt.Println("hostname:", hostname)
			fmt.Println("remote:", remote)
			fmt.Println("key:", key)
			return nil
		},
	})
	if err != nil {
		panic(err)
	}

	session,err := client.NewSession()
	if err != nil {
		panic(err)
	}

	session.Stdin = os.Stdin
	session.Stdout = os.Stdout
	session.Stderr = os.Stderr
	modes := ssh.TerminalModes{
		ssh.ECHO:          0,
		ssh.TTY_OP_ISPEED: 14400,
		ssh.TTY_OP_OSPEED: 14400,
	}

	err = session.RequestPty("xterm", 25, 100, modes)
	check(err, "request pty")

	err = session.Shell()
	check(err, "start shell")

	err = session.Wait()
	check(err, "return")
}
