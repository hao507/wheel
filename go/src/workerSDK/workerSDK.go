package workerSDK

import "fmt"

const (
	SOURCE = 0
	PROCESS = 1
	TARGET = 2
)

type IO interface{
	In() (data []byte, err error)
	Process(in []byte) (out []byte, err error)
	Out(data []byte) (n int, err error)
	Close() error
}

type info struct {
	LineId		int		`json:"line_id"`
	AgentId		int		`json:"agent_id"`
	WorkerId	int		`json:"worker_id"`

	WorkerType	int		`json:"worker_type"`
}

type host struct {
	Ip		string		`json:"ip"`
	Port	string		`json:"port"`
}

type worker struct {
	host
	info
}

type Worker struct {
	workerConf	config

	streamIO	IO
	reportIO	IO
	customIO	IO
}

func New(workerType int) (*Worker,error) {
	if workerType < SOURCE || workerType > TARGET {
		return nil, workerTypeError
	}

	c,err := loadConfig()
	if err != nil {
		return nil, err
	}

	return &Worker{workerConf:*c},err
}

func (w *Worker) onIn() (data []byte, err error) {
	fmt.Println("worker type", w.workerConf.Worker.WorkerType)
	switch w.workerConf.Worker.WorkerType {
	case SOURCE:
		return w.customIO.In()
	}

	return w.streamIO.In()
}

func (w *Worker) onProcess(in []byte) (out []byte, err error) {
	// stream process
	//out,err = w.streamIO.Process(in)

	// custom process
	if w.customIO != nil && w.customIO.Process != nil {
		return w.customIO.Process(in)
	}

	return in, err
}

func (w *Worker) onOut(data []byte) (n int, err error) {
	switch w.workerConf.Worker.WorkerType  {
	case TARGET:
		return w.customIO.Out(data)
	}

	return w.streamIO.Out(data)
}

func (w *Worker) Init() error {
	r,err := NewReportIO(w.workerConf.Report)
	if err != nil {
		return err
	}

	s,err := NewStreamIO(w.workerConf.Worker, w.workerConf.Next)
	if err != nil {
		r.Close()
		return err
	}

	w.reportIO = r
	w.streamIO = s

	return err
}

func (w *Worker) SetCustomIO(io IO) {
	w.customIO = io
}

func (w *Worker) Run() error {
	if w.customIO == nil || w.customIO.In == nil || w.customIO.Out == nil {
		return customNullError
	}

	for {
		fmt.Println()

		data,err := w.onIn()
		if err != nil {
			fmt.Println("ERROR onIn:", err.Error())
		} else {
			fmt.Println("SUCCESS onIn:", string(data))
			w.reportIO.In()
		}

		d,err := w.onProcess(data)
		if err != nil {
			fmt.Println("ERROR onProcess:", err.Error())
		} else {
			data = d
			fmt.Println("SUCCESS onProcess:", string(data))
		}

		n,err := w.onOut(data)
		if err != nil {
			fmt.Println("ERROR onOut:", err.Error())
		} else {
			fmt.Printf("SUCCESS onOut[%d]:%s\n", n, string(data))
			w.reportIO.Out(nil)
		}
	}

	return nil
}