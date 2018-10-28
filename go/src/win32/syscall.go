package win32

import (
	"syscall"
	"unicode/utf16"
	"errors"
)

var (
	// windows mode
	modkernel32 = NewLazyDLL("kernel32.dll")
	modadvapi32 = NewLazyDLL("advapi32.dll")
	modshell32  = NewLazyDLL("shell32.dll")
	modmswsock  = NewLazyDLL("mswsock.dll")
	modcrypt32  = NewLazyDLL("crypt32.dll")
	modws2_32   = NewLazyDLL("ws2_32.dll")
	moddnsapi   = NewLazyDLL("dnsapi.dll")
	modiphlpapi = NewLazyDLL("iphlpapi.dll")
	modsecur32  = NewLazyDLL("secur32.dll")
	modnetapi32 = NewLazyDLL("netapi32.dll")
	moduserenv  = NewLazyDLL("userenv.dll")

	// windows function pointer
	procCreateFileW                        = modkernel32.NewProcedure("CreateFileW")
	procReadFile                           = modkernel32.NewProcedure("ReadFile")
	procWriteFile                          = modkernel32.NewProcedure("WriteFile")
	procSetFilePointer                     = modkernel32.NewProcedure("SetFilePointer")
	procFindFirstFileW                     = modkernel32.NewProcedure("FindFirstFileW")
	procFindNextFileW                      = modkernel32.NewProcedure("FindNextFileW")
	procFindClose                          = modkernel32.NewProcedure("FindClose")
	procGetFileInformationByHandle         = modkernel32.NewProcedure("GetFileInformationByHandle")
	procGetCurrentDirectoryW               = modkernel32.NewProcedure("GetCurrentDirectoryW")
	procSetCurrentDirectoryW               = modkernel32.NewProcedure("SetCurrentDirectoryW")
	procCreateDirectoryW                   = modkernel32.NewProcedure("CreateDirectoryW")
	procRemoveDirectoryW                   = modkernel32.NewProcedure("RemoveDirectoryW")
	procDeleteFileW                        = modkernel32.NewProcedure("DeleteFileW")
	procMoveFileW                          = modkernel32.NewProcedure("MoveFileW")
	procGetComputerNameW                   = modkernel32.NewProcedure("GetComputerNameW")
	procSetEndOfFile                       = modkernel32.NewProcedure("SetEndOfFile")
	procGetSystemTimeAsFileTime            = modkernel32.NewProcedure("GetSystemTimeAsFileTime")
	procGetTimeZoneInformation             = modkernel32.NewProcedure("GetTimeZoneInformation")
	procCreateIoCompletionPort             = modkernel32.NewProcedure("CreateIoCompletionPort")
	procGetQueuedCompletionStatus          = modkernel32.NewProcedure("GetQueuedCompletionStatus")
	procPostQueuedCompletionStatus         = modkernel32.NewProcedure("PostQueuedCompletionStatus")
	procCancelIo                           = modkernel32.NewProcedure("CancelIo")
	procCancelIoEx                         = modkernel32.NewProcedure("CancelIoEx")
	procCreateProcessW                     = modkernel32.NewProcedure("CreateProcessW")
	procCreateProcessAsUserW               = modadvapi32.NewProcedure("CreateProcessAsUserW")
	procOpenProcess                        = modkernel32.NewProcedure("OpenProcess")
	procTerminateProcess                   = modkernel32.NewProcedure("TerminateProcess")
	procGetExitCodeProcess                 = modkernel32.NewProcedure("GetExitCodeProcess")
	procGetStartupInfoW                    = modkernel32.NewProcedure("GetStartupInfoW")
	procGetCurrentProcess                  = modkernel32.NewProcedure("GetCurrentProcess")
	procGetProcessTimes                    = modkernel32.NewProcedure("GetProcessTimes")
	procDuplicateHandle                    = modkernel32.NewProcedure("DuplicateHandle")
	procGetTempPathW                       = modkernel32.NewProcedure("GetTempPathW")
	procCreatePipe                         = modkernel32.NewProcedure("CreatePipe")
	procGetFileType                        = modkernel32.NewProcedure("GetFileType")
	procCryptAcquireContextW               = modadvapi32.NewProcedure("CryptAcquireContextW")
	procCryptReleaseContext                = modadvapi32.NewProcedure("CryptReleaseContext")
	procCryptGenRandom                     = modadvapi32.NewProcedure("CryptGenRandom")
	procGetEnvironmentStringsW             = modkernel32.NewProcedure("GetEnvironmentStringsW")
	procFreeEnvironmentStringsW            = modkernel32.NewProcedure("FreeEnvironmentStringsW")
	procGetEnvironmentVariableW            = modkernel32.NewProcedure("GetEnvironmentVariableW")
	procSetEnvironmentVariableW            = modkernel32.NewProcedure("SetEnvironmentVariableW")
	procSetFileTime                        = modkernel32.NewProcedure("SetFileTime")
	procGetFileAttributesW                 = modkernel32.NewProcedure("GetFileAttributesW")
	procSetFileAttributesW                 = modkernel32.NewProcedure("SetFileAttributesW")
	procGetFileAttributesExW               = modkernel32.NewProcedure("GetFileAttributesExW")
	procGetCommandLineW                    = modkernel32.NewProcedure("GetCommandLineW")
	procCommandLineToArgvW                 = modshell32.NewProcedure("CommandLineToArgvW")
	procLocalFree                          = modkernel32.NewProcedure("LocalFree")
	procSetHandleInformation               = modkernel32.NewProcedure("SetHandleInformation")
	procFlushFileBuffers                   = modkernel32.NewProcedure("FlushFileBuffers")
	procGetFullPathNameW                   = modkernel32.NewProcedure("GetFullPathNameW")
	procGetLongPathNameW                   = modkernel32.NewProcedure("GetLongPathNameW")
	procGetShortPathNameW                  = modkernel32.NewProcedure("GetShortPathNameW")
	procCreateFileMappingW                 = modkernel32.NewProcedure("CreateFileMappingW")
	procMapViewOfFile                      = modkernel32.NewProcedure("MapViewOfFile")
	procUnmapViewOfFile                    = modkernel32.NewProcedure("UnmapViewOfFile")
	procFlushViewOfFile                    = modkernel32.NewProcedure("FlushViewOfFile")
	procVirtualLock                        = modkernel32.NewProcedure("VirtualLock")
	procVirtualUnlock                      = modkernel32.NewProcedure("VirtualUnlock")
	procTransmitFile                       = modmswsock.NewProcedure("TransmitFile")
	procReadDirectoryChangesW              = modkernel32.NewProcedure("ReadDirectoryChangesW")
	procCertOpenSystemStoreW               = modcrypt32.NewProcedure("CertOpenSystemStoreW")
	procCertOpenStore                      = modcrypt32.NewProcedure("CertOpenStore")
	procCertEnumCertificatesInStore        = modcrypt32.NewProcedure("CertEnumCertificatesInStore")
	procCertAddCertificateContextToStore   = modcrypt32.NewProcedure("CertAddCertificateContextToStore")
	procCertCloseStore                     = modcrypt32.NewProcedure("CertCloseStore")
	procCertGetCertificateChain            = modcrypt32.NewProcedure("CertGetCertificateChain")
	procCertFreeCertificateChain           = modcrypt32.NewProcedure("CertFreeCertificateChain")
	procCertCreateCertificateContext       = modcrypt32.NewProcedure("CertCreateCertificateContext")
	procCertFreeCertificateContext         = modcrypt32.NewProcedure("CertFreeCertificateContext")
	procCertVerifyCertificateChainPolicy   = modcrypt32.NewProcedure("CertVerifyCertificateChainPolicy")
	procRegOpenKeyExW                      = modadvapi32.NewProcedure("RegOpenKeyExW")
	procRegCloseKey                        = modadvapi32.NewProcedure("RegCloseKey")
	procRegQueryInfoKeyW                   = modadvapi32.NewProcedure("RegQueryInfoKeyW")
	procRegEnumKeyExW                      = modadvapi32.NewProcedure("RegEnumKeyExW")
	procRegQueryValueExW                   = modadvapi32.NewProcedure("RegQueryValueExW")
	procGetCurrentProcessId                = modkernel32.NewProcedure("GetCurrentProcessId")
	procGetConsoleMode                     = modkernel32.NewProcedure("GetConsoleMode")
	procWriteConsoleW                      = modkernel32.NewProcedure("WriteConsoleW")
	procReadConsoleW                       = modkernel32.NewProcedure("ReadConsoleW")
	procCreateToolhelp32Snapshot           = modkernel32.NewProcedure("CreateToolhelp32Snapshot")
	procProcess32FirstW                    = modkernel32.NewProcedure("Process32FirstW")
	procProcess32NextW                     = modkernel32.NewProcedure("Process32NextW")
	procDeviceIoControl                    = modkernel32.NewProcedure("DeviceIoControl")
	procCreateSymbolicLinkW                = modkernel32.NewProcedure("CreateSymbolicLinkW")
	procCreateHardLinkW                    = modkernel32.NewProcedure("CreateHardLinkW")
	procWSAStartup                         = modws2_32.NewProcedure("WSAStartup")
	procWSACleanup                         = modws2_32.NewProcedure("WSACleanup")
	procWSAIoctl                           = modws2_32.NewProcedure("WSAIoctl")
	procsocket                             = modws2_32.NewProcedure("socket")
	procsetsockopt                         = modws2_32.NewProcedure("setsockopt")
	procgetsockopt                         = modws2_32.NewProcedure("getsockopt")
	procbind                               = modws2_32.NewProcedure("bind")
	procconnect                            = modws2_32.NewProcedure("connect")
	procgetsockname                        = modws2_32.NewProcedure("getsockname")
	procgetpeername                        = modws2_32.NewProcedure("getpeername")
	proclisten                             = modws2_32.NewProcedure("listen")
	procshutdown                           = modws2_32.NewProcedure("shutdown")
	procclosesocket                        = modws2_32.NewProcedure("closesocket")
	procAcceptEx                           = modmswsock.NewProcedure("AcceptEx")
	procGetAcceptExSockaddrs               = modmswsock.NewProcedure("GetAcceptExSockaddrs")
	procWSARecv                            = modws2_32.NewProcedure("WSARecv")
	procWSASend                            = modws2_32.NewProcedure("WSASend")
	procWSARecvFrom                        = modws2_32.NewProcedure("WSARecvFrom")
	procWSASendTo                          = modws2_32.NewProcedure("WSASendTo")
	procgethostbyname                      = modws2_32.NewProcedure("gethostbyname")
	procgetservbyname                      = modws2_32.NewProcedure("getservbyname")
	procntohs                              = modws2_32.NewProcedure("ntohs")
	procgetprotobyname                     = modws2_32.NewProcedure("getprotobyname")
	procDnsQuery_W                         = moddnsapi.NewProcedure("DnsQuery_W")
	procDnsRecordListFree                  = moddnsapi.NewProcedure("DnsRecordListFree")
	procDnsNameCompare_W                   = moddnsapi.NewProcedure("DnsNameCompare_W")
	procGetAddrInfoW                       = modws2_32.NewProcedure("GetAddrInfoW")
	procFreeAddrInfoW                      = modws2_32.NewProcedure("FreeAddrInfoW")
	procGetIfEntry                         = modiphlpapi.NewProcedure("GetIfEntry")
	procGetAdaptersInfo                    = modiphlpapi.NewProcedure("GetAdaptersInfo")
	procSetFileCompletionNotificationModes = modkernel32.NewProcedure("SetFileCompletionNotificationModes")
	procWSAEnumProtocolsW                  = modws2_32.NewProcedure("WSAEnumProtocolsW")
	procTranslateNameW                     = modsecur32.NewProcedure("TranslateNameW")
	procGetUserNameExW                     = modsecur32.NewProcedure("GetUserNameExW")
	procNetUserGetInfo                     = modnetapi32.NewProcedure("NetUserGetInfo")
	procNetGetJoinInformation              = modnetapi32.NewProcedure("NetGetJoinInformation")
	procNetApiBufferFree                   = modnetapi32.NewProcedure("NetApiBufferFree")
	procLookupAccountSidW                  = modadvapi32.NewProcedure("LookupAccountSidW")
	procLookupAccountNameW                 = modadvapi32.NewProcedure("LookupAccountNameW")
	procConvertSidToStringSidW             = modadvapi32.NewProcedure("ConvertSidToStringSidW")
	procConvertStringSidToSidW             = modadvapi32.NewProcedure("ConvertStringSidToSidW")
	procGetLengthSid                       = modadvapi32.NewProcedure("GetLengthSid")
	procCopySid                            = modadvapi32.NewProcedure("CopySid")
	procOpenProcessToken                   = modadvapi32.NewProcedure("OpenProcessToken")
	procGetTokenInformation                = modadvapi32.NewProcedure("GetTokenInformation")
	procGetUserProfileDirectoryW           = moduserenv.NewProcedure("GetUserProfileDirectoryW")

	procGetVersion                         = modkernel32.NewProcedure("GetVersion")
	procGetLastError                       = modkernel32.NewProcedure("GetLastError")
	procGetStdHandle                       = modkernel32.NewProcedure("GetStdHandle")
	procCloseHandle                        = modkernel32.NewProcedure("CloseHandle")
	procCreateMutexW                       = modkernel32.NewProcedure("CreateMutexW")
	procReleaseMutex                       = modkernel32.NewProcedure("ReleaseMutex")
	procWaitForSingleObject                = modkernel32.NewProcedure("WaitForSingleObject")
	procGetConsoleScreenBufferInfo         = modkernel32.NewProcedure("GetConsoleScreenBufferInfo")
	procSetConsoleTextAttribute            = modkernel32.NewProcedure("SetConsoleTextAttribute")
	procLoadLibraryW                       = modkernel32.NewProcedure("LoadLibraryW")
	procFreeLibrary                        = modkernel32.NewProcedure("FreeLibrary")
	procGetProcAddress                     = modkernel32.NewProcedure("GetProcAddress")
	procFormatMessageW                     = modkernel32.NewProcedure("FormatMessageW")
	procExitProcess                        = modkernel32.NewProcedure("ExitProcess")
)

type LazyDLL struct {
	syscall.LazyDLL
}

type LazyProc struct {
	syscall.LazyProc
}

func NewLazyDLL (name string) *LazyDLL {
	return &LazyDLL{*syscall.NewLazyDLL(name)}
}

func (d *LazyDLL) NewProcedure(name string) *LazyProc {
	return &LazyProc{*d.NewProc(name)}
}

func (p *LazyProc) Call(a ...uintptr) (r1, r2 uintptr, lastErr error) {
	p.mustFind()
	return p.syscall(a...)
}

func (p *LazyProc) mustFind() {
	e := p.Find()
	if e != nil {
		panic(e)
	}
}

func (p *LazyProc) syscall(a ...uintptr) (r1, r2 uintptr, lastErr error) {
	switch len(a) {
	case 0:
		return syscall.Syscall(p.Addr(), uintptr(len(a)), 0, 0, 0)
	case 1:
		return syscall.Syscall(p.Addr(), uintptr(len(a)), a[0], 0, 0)
	case 2:
		return syscall.Syscall(p.Addr(), uintptr(len(a)), a[0], a[1], 0)
	case 3:
		return syscall.Syscall(p.Addr(), uintptr(len(a)), a[0], a[1], a[2])
	case 4:
		return syscall.Syscall6(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], 0, 0)
	case 5:
		return syscall.Syscall6(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], 0)
	case 6:
		return syscall.Syscall6(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5])
	case 7:
		return syscall.Syscall9(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], 0, 0)
	case 8:
		return syscall.Syscall9(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], 0)
	case 9:
		return syscall.Syscall9(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8])
	case 10:
		return syscall.Syscall12(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], 0, 0)
	case 11:
		return syscall.Syscall12(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], 0)
	case 12:
		return syscall.Syscall12(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11])
	case 13:
		return syscall.Syscall15(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], 0, 0)
	case 14:
		return syscall.Syscall15(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], 0)
	case 15:
		return syscall.Syscall15(p.Addr(), uintptr(len(a)), a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14])
	default:
		panic("Call " + p.Name + " with too many arguments " + itoa(len(a)) + ".")
	}
}

func itoa(val int) string { // do it here rather than with fmt to avoid dependency
	if val < 0 {
		return "-" + uitoa(uint(-val))
	}
	return uitoa(uint(val))
}

func uitoa(val uint) string {
	var buf [32]byte // big enough for int64
	i := len(buf) - 1
	for val >= 10 {
		buf[i] = byte(val%10 + '0')
		i--
		val /= 10
	}
	buf[i] = byte(val + '0')
	return string(buf[i:])
}

// UTF16PtrFromString returns pointer to the UTF-16 encoding of
// the UTF-8 string s, with a terminating NUL added. If s
// contains a NUL byte at any location, it returns (nil, EINVAL).
func UTF16PtrFromString(s string) (*uint16, error) {
	a, err := UTF16FromString(s)
	if err != nil {
		return nil, EINVAL
	}
	return &a[0], nil
}

// UTF16FromString returns the UTF-16 encoding of the UTF-8 string
// s, with a terminating NUL added. If s contains a NUL byte at any
// location, it returns (nil, EINVAL).
func UTF16FromString(s string) ([]uint16, error) {
	for i := 0; i < len(s); i++ {
		if s[i] == 0 {
			return nil, errors.New("invalid argument")
		}
	}
	return utf16.Encode([]rune(s + "\x00")), nil
}

// BytePtrFromString returns a pointer to a NUL-terminated array of
// bytes containing the text of s. If s contains a NUL byte at any
// location, it returns (nil, EINVAL).
func BytePtrFromString(s string) (*byte, error) {
	a, err := ByteSliceFromString(s)
	if err != nil {
		return nil, err
	}
	return &a[0], nil
}

// ByteSliceFromString returns a NUL-terminated slice of bytes
// containing the text of s. If s contains a NUL byte at any
// location, it returns (nil, EINVAL).
func ByteSliceFromString(s string) ([]byte, error) {
	for i := 0; i < len(s); i++ {
		if s[i] == 0 {
			return nil, errors.New("invalid argument")
		}
	}
	a := make([]byte, len(s)+1)
	copy(a, s)
	return a, nil
}