package win32

import (
	"testing"
	"fmt"
	"syscall"
	"time"
	"github.com/shirou/w32"
)

func TestGetVersion(t *testing.T) {
	if v,err := GetVersion(); err != nil {
		panic(err)
	} else {
		fmt.Println(fmt.Sprintf("windows version: %d.%d (%d)", byte(v), uint8(v>>8), v>>16))
	}
}

func TestGetLastError(t *testing.T) {
	fmt.Println(GetLastError())
}

func TestGetStdHandle(t *testing.T) {
	h,err := GetStdHandle(syscall.STD_OUTPUT_HANDLE)
	if err != nil {
		fmt.Println("error: ", err.Error())
	}
	fmt.Println("handle:", h)
}

func TestCloseHandle(t *testing.T) {
	h,err := GetStdHandle(syscall.STD_OUTPUT_HANDLE)
	if err != nil {
		fmt.Println("error: ", err.Error())
	}
	if ok,err := CloseHandle(h); ok {
		fmt.Println("close handle is success.")
	} else {
		fmt.Println("error: ", err.Error())
	}
}

func TestCreateMutexW(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if err != nil {
		panic(err)
	}

	go func () {
		syscall.WaitForSingleObject(syscall.Handle(h), syscall.INFINITE)
		for i := 1; i < 10; i++ {
			fmt.Println("I'm go run :", i)
		}
		ReleaseMutex(h)
	}()

	time.Sleep(1 * time.Second)
	WaitForSingleObject(h, syscall.INFINITE)
	ReleaseMutex(h)
	CloseHandle(h)

}

func TestReleaseMutex(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if err != nil {
		panic(err)
	}
	i := 1
	t1 := func (id int) {
		for {
			WaitForSingleObject(h, syscall.INFINITE)
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
	WaitForSingleObject(h, syscall.INFINITE)
	ReleaseMutex(h)
	CloseHandle(h)
}

func TestWaitForSingleObject(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if err != nil {
		panic(err)
	}

	go func () {
		syscall.WaitForSingleObject(syscall.Handle(h), syscall.INFINITE)
		for i := 1; i < 10; i++ {
			fmt.Println("I'm go run :", i)
		}
		ReleaseMutex(h)
	}()

	time.Sleep(1 * time.Second)
	WaitForSingleObject(h, syscall.INFINITE)
	ReleaseMutex(h)
	CloseHandle(h)
}

func TestGetConsoleScreenBufferInfo(t *testing.T) {
	h,err := GetStdHandle(syscall.STD_OUTPUT_HANDLE)
	if err != nil {
		panic(err)
	}
	var csbi CONSOLE_SCREEN_BUFFER_INFO

	if ok,err := GetConsoleScreenBufferInfo(h, &csbi); ok && err == nil {
		if ok,err = SetConsoleTextAttribute(h, w32.FOREGROUND_RED | w32.FOREGROUND_GREEN); ok && err == nil {
			fmt.Println("color set success.")
			SetConsoleTextAttribute(h,csbi.wAttributes)
		} else {
			fmt.Println("color set fail.")
		}
	} else {
		fmt.Println("get console attr fail.")
	}
}

func TestSetConsoleTextAttribute(t *testing.T) {
	h,err := GetStdHandle(syscall.STD_OUTPUT_HANDLE)
	if err != nil {
		panic(err)
	}

	if ok,err := SetConsoleTextAttribute(h, w32.FOREGROUND_RED | w32.FOREGROUND_GREEN); ok && err == nil {
		fmt.Println("color set success.")
	} else {
		fmt.Println("color set fail.")
	}
}
