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

// TODO: have buf, need debug.
func GetLastError() (uint32, error) {
	ret,_,err := procGetLastError.Call()

	return uint32(ret), err
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

// TODO: need debug test.
func FormatMessageW(dwFlags uint32,
	lpSource uintptr,
	dwMessageId uint32,
	dwLanguageId uint32,
	lpBuffer *uint16,
	nSize uint32,
	Arguments *byte) (uint32, error) {

		ret,_,err := procFormatMessageW.Call(uintptr(dwFlags),
			lpSource,
			uintptr(dwMessageId),
			uintptr(dwLanguageId),
			uintptr(unsafe.Pointer(lpBuffer)),
			uintptr(nSize),
			uintptr(unsafe.Pointer(Arguments)),
		)

	return uint32(ret), err
}

func ExitProcess(uExitCode uint32) error {
	_,_,err := procExitProcess.Call(uintptr(uExitCode))

	return err
}

func GlobalMemoryStatusEx(lpBuffer LPMEMORYSTATUSEX) (bool, error) {
	ret,_,err := procGlobalMemoryStatusEx.Call(uintptr(unsafe.Pointer(lpBuffer)))

	return ret != 0, err
}

func GetDiskFreeSpaceExA(lpDirectoryName *byte,
	lpFreeBytesAvailableToCaller PULARGE_INTEGER,
	lpTotalNumberOfBytes PULARGE_INTEGER,
	lpTotalNumberOfFreeBytes PULARGE_INTEGER) (bool, error) {
	ret,_,err := procGetDiskFreeSpaceExA.Call(uintptr(unsafe.Pointer(lpDirectoryName)),
		uintptr(unsafe.Pointer(lpFreeBytesAvailableToCaller)),
		uintptr(unsafe.Pointer(lpTotalNumberOfBytes)),
		uintptr(unsafe.Pointer(lpTotalNumberOfFreeBytes)))

	return ret != 0, err
}

func GetSystemTimes(lpIdleTime PFILETIME, lpKernelTime PFILETIME, lpUserTime PFILETIME) (bool, error) {
	ret,_,err := procGetSystemTimes.Call(uintptr(unsafe.Pointer(lpIdleTime)), uintptr(unsafe.Pointer(lpKernelTime)), uintptr(unsafe.Pointer(lpUserTime)))

	return ret != 0, err
}

func GetLogicalDriveStringsA(nBufferLength uint32, lpBuffer *byte) (uint32, error) {
	ret,_,err := procGetLogicalDriveStringsA.Call(uintptr(nBufferLength), uintptr(unsafe.Pointer(lpBuffer)))

	return uint32(ret), err
}

func GetVolumeInformationA(lpRootPathName *byte,
	lpVolumeNameBuffer *byte,
	nVolumeNameSize uint32,
	lpVolumeSerialNumber *uint32,
	lpMaximumComponentLength *uint32,
	lpFileSystemFlags *uint32,
	lpFileSystemNameBuffer *byte,
	nFileSystemNameSize uint32) (bool, error) {
		ret,_,err := procGetVolumeInformationA.Call(uintptr(unsafe.Pointer(lpRootPathName)),
			uintptr(unsafe.Pointer(lpVolumeNameBuffer)),
			uintptr(nVolumeNameSize),
			uintptr(unsafe.Pointer(lpVolumeSerialNumber)),
			uintptr(unsafe.Pointer(lpMaximumComponentLength)),
			uintptr(unsafe.Pointer(lpFileSystemFlags)),
			uintptr(unsafe.Pointer(lpFileSystemNameBuffer)),
			uintptr(nFileSystemNameSize))

		return ret != 0, err
}