package workerSDK

import (
		"net"
	)

type ReportIO struct {
	host
	conn	net.Conn
}

const reportInMessage = `this is report in...` + "\n"
const reportOutMessage = `this is report out...` + "\n"

func (r *ReportIO) In() (data []byte, err error) {
	r.conn.Write([]byte(reportInMessage))
	return []byte(reportInMessage), err
}

func (r *ReportIO) Process(in []byte) (out []byte, err error) {
	return in, err
}

func (r *ReportIO) Out(data []byte) (n int, err error) {
	if cap(data) <= 0 {
		return 0, err
	}

	out := []byte(reportOutMessage)
	length := len(out)
	if cap(data) < length {
		length = cap(data)
	}

	for i := 0; i< length; i++ {
		data[i] = out[i]
	}

	r.conn.Write([]byte(reportOutMessage))

	return length, err
}

func (r *ReportIO) Close() error {
	return r.conn.Close()
}

func NewReportIO(h host) (*ReportIO, error) {
	var r ReportIO
	r.host = h

	conn,err := net.Dial("tcp", r.Ip + ":" + r.Port)
	if err != nil {
		return nil, err
	}

	r.conn = conn

	return &r, err
}