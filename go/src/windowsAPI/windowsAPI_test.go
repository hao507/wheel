package windowsAPI

import (
	"testing"
	"fmt"
	"syscall"
	"time"
)

func TestGetVersion(t *testing.T) {
	if v,err := GetVersion(); err != nil {
		panic(err)
	} else {
		fmt.Println(fmt.Sprintf("windows version: %d.%d (%d)", byte(v), uint8(v>>8), v>>16))
	}
}

func TestCreateMutexW(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if err != nil {
		panic(err)
	}

	go func () {
		syscall.WaitForSingleObject(h, syscall.INFINITE)
		for i := 1; i < 10; i++ {
			fmt.Println("I'm go run :", i)
			time.Sleep(1 * time.Second)
		}
		ReleaseMutex(h)
	}()

	time.Sleep(1 * time.Second)
	syscall.WaitForSingleObject(h, syscall.INFINITE)
	ReleaseMutex(h)
	syscall.CloseHandle(h)

}

func TestReleaseMutex(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if err != nil {
		panic(err)
	}
	i := 1
	t1 := func (id int) {
		for {
			syscall.WaitForSingleObject(h, syscall.INFINITE)
			if i > 1000 {
				ReleaseMutex(h)
				break
			}
			fmt.Println("I'm go ", id, " run :", i)
			i++
			ReleaseMutex(h)
		}
	}

	for i := 0; i < 5; i++ {
		go t1(i + 1)
	}

	time.Sleep(5 * time.Second)
	syscall.WaitForSingleObject(h, syscall.INFINITE)
	ReleaseMutex(h)
	syscall.CloseHandle(h)
}