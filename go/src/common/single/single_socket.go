package single

import (
	"net"
	"strconv"
	"errors"
)

type singleSocket struct {
	port    int
}

func NewSingleSocket(port int) *singleSocket {
	return &singleSocket{port:port}
}

func (s *singleSocket) localListen() error {
	listener,err := net.Listen("tcp", ":" + strconv.Itoa(s.port))
	if err != nil {
		return err
	} else {
		go listener.Accept()
	}

	return nil
}

func (s *singleSocket) localConnect() error {
	conn,err := net.Dial("tcp", ":" + strconv.Itoa(s.port))
	if err != nil {
		return err
	} else {
		conn.Read(nil)
		conn.Close()
	}

	return nil
}

func (s *singleSocket) Start() error {
RESTART:
	if s.localListen() != nil {
		if s.localConnect() != nil {
			return errors.New("start single fail")
		} else {
			goto RESTART
		}
	}

	return nil
}
