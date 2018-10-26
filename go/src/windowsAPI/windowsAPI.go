package windowsAPI

import (
	"syscall"
	"unsafe"
)

// windows api return success error info.
const Success = "The operation completed successfully."

func SysCall(p *syscall.LazyProc, a ...uintptr) (r1, r2 uintptr, lastErr error) {
	r1,r2,lastErr = p.Call(a...)
	if lastErr.Error() == Success {
		lastErr = nil
	}

	return
}

func CreateMutexW(lpMutexAttributes LPSECURITY_ATTRIBUTES, bInitialOwner bool, lpName LPCWSTR) (syscall.Handle, error) {
	ret,_,err := SysCall(createMutexW, uintptr(unsafe.Pointer(lpMutexAttributes)), uintptr(typeToBOOL(bInitialOwner)), uintptr(unsafe.Pointer(lpName)))
	return syscall.Handle(ret), err
}

func ReleaseMutex(hMutex syscall.Handle) (bool, error) {
	ret,_,err := SysCall(releaseMutex, uintptr(hMutex))
	return ret != 0, err
}

func GetConsoleScreenBufferInfo(hConsoleOutput syscall.Handle, lpConsoleScreenBufferInfo PCONSOLE_SCREEN_BUFFER_INFO) (bool, error) {
	ret,_,err := SysCall(getConsoleScreenBufferInfo, uintptr(hConsoleOutput), uintptr(unsafe.Pointer(lpConsoleScreenBufferInfo)))
	return ret != 0, err
}

func SetConsoleTextAttribute(hConsoleOutput syscall.Handle, wAttributes WORD) (bool, error) {
	ret,_,err := SysCall(setConsoleTextAttribute, uintptr(hConsoleOutput), uintptr(wAttributes))
	return ret != 0, err
}

func GetVersion() (uint32, error) {
	ret,_,err := SysCall(getVersion)
	return uint32(ret), err
}