// +build windows

package mutex

import (
	"syscall"
	"unsafe"
)

const INFINITE = 0xffffffff
const success = "The operation completed successfully."
const exists = "Cannot create a file when that file already exists."

type Mutex struct{
	name string
	lock syscall.Handle
}

// create or open a mutex lock
func NewMutex(name string)(*Mutex, error){

	var mutex Mutex

	// get windows api function.
	kernel32 := syscall.NewLazyDLL("kernel32.dll")
	CreateMutexW := kernel32.NewProc("CreateMutexW")

	handle,_,err := CreateMutexW.Call(0, 0, uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(name))))
	if err != nil && err.Error() != success && err.Error() != exists {
		return nil, err
	} else {
		err = nil
	}

	/*
	lib, err := syscall.LoadLibrary("kernel32.dll")
	if err != nil {
		return nil, err
	}

	cm, err := syscall.GetProcAddress(lib, "CreateMutexW")
	if err != nil {
		syscall.FreeLibrary(lib)
		return nil, err
	}

	rm, err := syscall.GetProcAddress(lib, "ReleaseMutex")
	if err != nil {
		syscall.FreeLibrary(lib)
		return nil, err
	}

	handle, _, _ := syscall.Syscall(cm, 3, 0,0, uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(name))))
	if err != nil {
		syscall.FreeLibrary(lib)
		return nil, err
	}
	*/

	mutex.lock = syscall.Handle(handle)

	return &mutex, err
}

// lock mutex
func (m * Mutex)Lock()(bool, error){

	ret, err := syscall.WaitForSingleObject(m.lock, INFINITE)
	if err != nil{
		return false, err
	}

	return ret == syscall.WAIT_OBJECT_0 || ret == syscall.WAIT_ABANDONED, nil
}

// try lock mutex
func (m * Mutex)TryLock()(bool, error){

	ret, err := syscall.WaitForSingleObject(m.lock, 0)
	if err != nil{
		return false, err
	}

	return ret == syscall.WAIT_OBJECT_0 || ret == syscall.WAIT_ABANDONED, nil
}

// unlock mutex
func (m * Mutex)Unlock()(bool, error){

	// get windows api function.
	kernel32 := syscall.NewLazyDLL("kernel32.dll")
	ReleaseMutex := kernel32.NewProc("ReleaseMutex")

	ret,_,err := ReleaseMutex.Call(uintptr(m.lock))
	if err != nil  && err.Error() != success {
		return false, err
	}

	//ret,_,err := syscall.Syscall(m.releaseMutex, 1, uintptr(m.lock),0, 0)

	return ret != 0, err
}

// destroy mutex
func (m * Mutex)Destroy() error {
	return syscall.CloseHandle(m.lock)
}