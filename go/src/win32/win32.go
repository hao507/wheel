package win32

import (
	"unsafe"
	"syscall"
)

// windows api return success error info.
const Success = "The operation completed successfully."

func GetLastError() (int, error) {
	err := syscall.GetLastError()

	return int(*err.(*syscall.Errno)), err
}

func CreateMutexW(lpMutexAttributes LPSECURITY_ATTRIBUTES, bInitialOwner bool, lpName LPCWSTR) (syscall.Handle, error) {
	ret,_,err := procCreateMutexW.Call(uintptr(unsafe.Pointer(lpMutexAttributes)), uintptr(typeToBOOL(bInitialOwner)), uintptr(unsafe.Pointer(lpName)))
	return syscall.Handle(ret), err
}

func ReleaseMutex(hMutex syscall.Handle) (bool, error) {
	ret,_,err := procReleaseMutex.Call(uintptr(hMutex))
	return ret != 0, err
}

func GetConsoleScreenBufferInfo(hConsoleOutput syscall.Handle, lpConsoleScreenBufferInfo PCONSOLE_SCREEN_BUFFER_INFO) (bool, error) {
	ret,_,err := procGetConsoleScreenBufferInfo.Call(uintptr(hConsoleOutput), uintptr(unsafe.Pointer(lpConsoleScreenBufferInfo)))
	return ret != 0, err
}

func SetConsoleTextAttribute(hConsoleOutput syscall.Handle, wAttributes uint16) (bool, error) {
	ret,_,err := procSetConsoleTextAttribute.Call(uintptr(hConsoleOutput), uintptr(wAttributes))
	return ret != 0, err
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
