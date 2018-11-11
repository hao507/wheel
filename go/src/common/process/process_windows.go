// +build windows

package process

import (
	"syscall"
	"unsafe"
)

// kill a process
func KillProcess(pid uint32)(bool){
	handle, err := syscall.OpenProcess(syscall.PROCESS_TERMINATE, false, pid)
	if err != nil{
		return false
	}

	err = syscall.TerminateProcess(handle, 0)
	if err != nil{
		return false
	}

	return syscall.CloseHandle(handle) == nil
}

// kill a process tree(current process except)
func KillProcessTree(pid uint32)(bool){
	var info syscall.ProcessEntry32
	info.Size = uint32(unsafe.Sizeof(info))

	handle, err := syscall.CreateToolhelp32Snapshot(syscall.TH32CS_SNAPPROCESS, pid)
	if err != nil{
		return false
	}

	err = syscall.Process32First(handle, &info)

	for err == nil{
		if pid == info.ParentProcessID && info.ProcessID != uint32(syscall.Getpid()){
			KillProcessTree(info.ProcessID)
			KillProcess(info.ProcessID)
		}

		err = syscall.Process32Next(handle, &info)
	}

	if FindProcess(pid){
		KillProcess(pid)
	}

	return true
}

// find process
func FindProcess(pid uint32)(bool){
	var info syscall.ProcessEntry32
	info.Size = uint32(unsafe.Sizeof(info))

	handle, err := syscall.CreateToolhelp32Snapshot(syscall.TH32CS_SNAPPROCESS, pid)
	if err != nil{
		return false
	}

	err = syscall.Process32First(handle, &info)

	for err == nil{
		if pid == info.ProcessID{
			return true
		}

		err = syscall.Process32Next(handle, &info)
	}

	return false
}
