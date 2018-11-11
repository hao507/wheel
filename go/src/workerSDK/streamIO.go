package workerSDK

import (
	"net"
	)

type StreamIO struct {
	StreamIn	worker		`json:"in"`
	StreamOut	[]worker	`json:"out"`

	listener	net.Listener
	receiver	map[string]net.Conn
	sender		map[string]net.Conn
	queue		chan []byte
}

func (s *StreamIO) In() (data []byte, err error) {
	return <- s.queue, err
}

func (s *StreamIO) Process(in []byte) (out []byte, err error) {
	return in, err
}

func (s *StreamIO) Out(data []byte) (n int, err error) {
	for _,w := range s.StreamOut {
		if _,ok := s.sender[w.Ip + ":" + w.Port]; !ok {
			conn,e := net.Dial("tcp", w.Ip + ":" + w.Port)
			if e != nil {
				err = e
				continue
			}
			s.sender[w.Ip + ":" + w.Port] = conn
		}
		if _,e := s.sender[w.Ip + ":" + w.Port].Write(data); e != nil {
			err = e
		} else {
			n++
		}
	}

	return n, err
}

func (s *StreamIO) Close() error {
	var err error

	for _,conn := range s.receiver {
		err = conn.Close()
	}

	for _,conn := range s.sender {
		err = conn.Close()
	}

	err = s.listener.Close()

	return err
}

func NewStreamIO(in worker, out []worker) (*StreamIO, error) {
	listener,err := net.Listen("tcp", in.Ip + ":" + in.Port)
	if err != nil {
		return nil, err
	}

	var s StreamIO

	s.queue = make(chan []byte, 1024000)
	s.StreamIn = in
	s.StreamOut = out
	s.listener = listener
	s.receiver = make(map[string]net.Conn)
	s.sender = make(map[string]net.Conn)

	go func(){
		for {
			conn,err := listener.Accept()
			if err == nil {
				go func(){
					s.receiver[conn.RemoteAddr().String()] = conn
					for {
						buf := make([]byte, 10240)
						n,err := conn.Read(buf)
						if err != nil {
							break
						}
						s.queue <- buf[:n]
					}
					delete(s.receiver, conn.RemoteAddr().String())
				}()
			}
		}
	}()

	return &s, err
}