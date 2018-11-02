// +build linux

package mutex

import (
	"os"
	"syscall"
	)

const (
	LOCK_EX = 0x2
	LOCK_NB = 0x4
	LOCK_SH = 0x1
	LOCK_UN = 0x8
)

type Mutex struct{
	name string
	file *os.File
}

// create or open a mutex lock
func NewMutex(name string)(*Mutex, error){
	file,err := os.OpenFile(name, os.O_RDWR | os.O_CREATE, 0666)
	if err != nil {
		return nil,err
	}

	return &Mutex{name, file},nil
}

// lock mutex
func (this *Mutex)Lock()(bool, error){

	err := syscall.Flock(int(this.file.Fd()), LOCK_EX)
	if err != nil {
		return false, err
	}

	return true, err
}

// try lock mutex
func (this *Mutex)TryLock()(bool, error){

	err := syscall.Flock(int(this.file.Fd()), LOCK_EX|LOCK_NB)
	if err != nil {
		return false, err
	}

	return true, err
}

// unlock mutex
func (this * Mutex)Unlock()(bool, error){

	err := syscall.Flock(int(this.file.Fd()), syscall.LOCK_UN)
	if err != nil {
		return false, err
	}

	return true, err
}

// destroy mutex
func (this * Mutex)Destroy() error {
	return this.file.Close()
}