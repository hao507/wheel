package win32

// windows type define
type DWORD		uint32
type BOOL		int32
type BYTE		byte
type WORD		uint16
type FLOAT		float32
type CHAR		byte
type LONG		int32
type SHORT		int16
type INT		int
type DWORD64	uint64
type ULONG64	uint64
type ULONGLONG	uint64
type DWORDLONG	ULONGLONG

type PFLOAT		*FLOAT
type PBOOL		*BOOL
type PBYTE		*BYTE
type PINT		*INT
type PWORD		*WORD
type PLONG		*LONG
type PDWORD		*DWORD
type PVOID		uintptr

type LPFLOAT	*FLOAT
type LPBOOL		*BOOL
type LPBYTE		*BYTE
type LPINT		*INT
type LPWORD		*WORD
type LPLONG		*LONG
type LPDWORD	*DWORD
type LPVOID		uintptr

type WCHAR		uint16

type LPCWSTR	*WCHAR

const TRUE		= 1
const FALSE	= 0
const NULL		= 0

type Handle uintptr

const InvalidHandle = ^Handle(0)

type SECURITY_ATTRIBUTES struct {
	nLength					uint32
	lpSecurityDescriptor	uintptr
	bInheritHandle			int32
}
type PSECURITY_ATTRIBUTES *SECURITY_ATTRIBUTES
type LPSECURITY_ATTRIBUTES *SECURITY_ATTRIBUTES


type COORD struct {
	X			int16
	Y			int16
}
type SMALL_RECT struct {
	Left		int16
	Top			int16
	Right		int16
	Bottom		int16
}
type CONSOLE_SCREEN_BUFFER_INFO struct {
	DwSize					COORD
	DwCursorPosition		COORD
	WAttributes				uint16
	SrWindow				SMALL_RECT
	DwMaximumWindowSize		COORD
}
type PCONSOLE_SCREEN_BUFFER_INFO *CONSOLE_SCREEN_BUFFER_INFO


type  MEMORYSTATUSEX struct {
	DwLength					uint32
	DwMemoryLoad				uint32
	UllTotalPhys				uint64
	UllAvailPhys				uint64
	UllTotalPageFile			uint64
	UllAvailPageFile			uint64
	UllTotalVirtual				uint64
	UllAvailVirtual				uint64
	UllAvailExtendedVirtual		uint64
}
type LPMEMORYSTATUSEX *MEMORYSTATUSEX

type ULARGE_INTEGER struct {
	U struct{
		LowPart		uint32
		HighPart	uint32
	}
	QuadPart		uint64
}
type PULARGE_INTEGER *ULARGE_INTEGER

type FILETIME struct {
	DwLowDateTime	uint32
	DwHighDateTime	uint32
}
type PFILETIME *FILETIME
type LPFILETIME *FILETIME

func typeToBool(b BOOL) bool {
	if b != 0 {
		return true
	}

	return false
}
func typeToBOOL(b bool) BOOL {
	if b {
		return TRUE
	}

	return FALSE
}
