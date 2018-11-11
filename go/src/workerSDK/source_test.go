package workerSDK

import (
	"testing"
	"time"
	"os"
)

type source struct {

}

func (s *source) In() (data []byte, err error) {
	time.Sleep(1 * time.Second)

	return []byte(time.Now().Format(time.RFC3339)), err
}

func (s *source) Process(in []byte) (out []byte, err error) {
	return in, err
}

func (s *source) Out(data []byte) (n int, err error) {
	return len(data), err
}

func (s *source) Close() error {
	return nil
}

func TestNew(t *testing.T) {
	//var c config
	//js,_ := json.Marshal(c)
	//ioutil.WriteFile(*conf, js, 0622)

	*conf = "source.conf"

	var s source
	w,err := New(SOURCE)
	if err != nil {
		panic(err)
	}

	w.SetCustomIO(&s)
	if err := w.Init(); err != nil {
		panic(err)
	}

	err = w.Run()

	panic(err)

	os.Executable()
}
