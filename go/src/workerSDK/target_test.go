package workerSDK

import (
	"testing"
	"fmt"
)

type target struct {

}

func (s *target) In() (data []byte, err error) {
	return data, err
}

func (s *target) Process(in []byte) (out []byte, err error) {
	return in, err
}

func (s *target) Out(data []byte) (n int, err error) {
	fmt.Println("I'm target, receive :", string(data))
	return len(data), err
}

func (s *target) Close() error {
	return nil
}

func TestNew2(tt *testing.T) {
	//var c config
	//js,_ := json.Marshal(c)
	//ioutil.WriteFile(*conf, js, 0622)

	*conf = "target.conf"
	var t target
	w,err := New(TARGET)
	if err != nil {
		panic(err)
	}

	w.SetCustomIO(&t)
	if err := w.Init(); err != nil {
		panic(err)
	}
	err = w.Run()

	panic(err)
}