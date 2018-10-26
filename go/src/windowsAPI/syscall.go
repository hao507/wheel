package windowsAPI

import "syscall"

var (
	kernel32                         = syscall.NewLazyDLL("kernel32.dll")

	createMutexW                     = kernel32.NewProc("CreateMutexW")
	releaseMutex                     = kernel32.NewProc("ReleaseMutex")
	getConsoleScreenBufferInfo       = kernel32.NewProc("GetConsoleScreenBufferInfo")
	setConsoleTextAttribute          = kernel32.NewProc("SetConsoleTextAttribute")
	getVersion                       = kernel32.NewProc("GetVersion")
)
