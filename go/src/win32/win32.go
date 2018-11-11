package win32

import (
	"unsafe"
	)

// windows api return success error info.
const Success = "The operation completed successfully."

func GetVersion() (uint32, error) {
	ret,_,err := procGetVersion.Call()
	return uint32(ret), err
}

func GetLastError() (error) {
	_,_,err := procGetLastError.Call()
	return err
}

func GetStdHandle(stdhandle int) (handle Handle, err error) {
	ret,_,err := procGetStdHandle.Call(uintptr(stdhandle))
	return Handle(ret), err
}

func CloseHandle(handle Handle) (bool, error) {
	ret,_,err := procCloseHandle.Call(uintptr(handle))
	return ret != 0, err
}

func CreateMutexW(lpMutexAttributes LPSECURITY_ATTRIBUTES, bInitialOwner bool, lpName LPCWSTR) (Handle, error) {
	ret,_,err := procCreateMutexW.Call(uintptr(unsafe.Pointer(lpMutexAttributes)), uintptr(typeToBOOL(bInitialOwner)), uintptr(unsafe.Pointer(lpName)))
	return Handle(ret), err
}

func ReleaseMutex(hMutex Handle) (bool, error) {
	ret,_,err := procReleaseMutex.Call(uintptr(hMutex))
	return ret != 0, err
}

func WaitForSingleObject(hHandle Handle, dwMilliseconds uint32) (uint32, error) {
	ret,_,err := procWaitForSingleObject.Call(uintptr(hHandle), uintptr(dwMilliseconds))
	return uint32(ret), err
}

func GetConsoleScreenBufferInfo(hConsoleOutput Handle, lpConsoleScreenBufferInfo PCONSOLE_SCREEN_BUFFER_INFO) (bool, error) {
	ret,_,err := procGetConsoleScreenBufferInfo.Call(uintptr(hConsoleOutput), uintptr(unsafe.Pointer(lpConsoleScreenBufferInfo)))
	return ret != 0, err
}

func SetConsoleTextAttribute(hConsoleOutput Handle, wAttributes uint16) (bool, error) {
	ret,_,err := procSetConsoleTextAttribute.Call(uintptr(hConsoleOutput), uintptr(wAttributes))
	return ret != 0, err
}

func LoadLibraryW(lpLibFileName string) (Handle, error) {
	var libName *uint16
	libName, err := UTF16PtrFromString(lpLibFileName)
	if err != nil {
		return Handle(0), err
	}

	ret,_,err := procLoadLibraryW.Call(uintptr(unsafe.Pointer(libName)))

	return Handle(ret), err
}

func FreeLibrary(handle Handle) (bool, error) {
	ret,_,err := procFreeLibrary.Call(uintptr(handle))
	return ret != 0, err
}

func GetProcAddress(hModule Handle, lpProcName string) (uintptr, error) {
	var procname *byte
	procname, err := BytePtrFromString(lpProcName)
	if err != nil {
		return uintptr(0), err
	}
	ret,_,err := procGetProcAddress.Call(uintptr(hModule), uintptr(unsafe.Pointer(procname)))

	return ret, err
}

func FormatMessageW() {
	//syscall.FormatMessage()
}

func ExitProcess() {

}

