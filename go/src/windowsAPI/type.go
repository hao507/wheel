package windowsAPI

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




type SECURITY_ATTRIBUTES struct {
	nLength					DWORD
	lpSecurityDescriptor	LPVOID
	bInheritHandle			BOOL
}
type PSECURITY_ATTRIBUTES *SECURITY_ATTRIBUTES
type LPSECURITY_ATTRIBUTES *SECURITY_ATTRIBUTES


type COORD struct {
	X			SHORT
	Y			SHORT
}
type SMALL_RECT struct {
	Left		SHORT
	Top			SHORT
	Right		SHORT
	Bottom		SHORT
}
type CONSOLE_SCREEN_BUFFER_INFO struct {
	dwSize					COORD
	dwCursorPosition		COORD
	wAttributes				WORD
	srWindow				SMALL_RECT
	dwMaximumWindowSize		COORD
}
type PCONSOLE_SCREEN_BUFFER_INFO *CONSOLE_SCREEN_BUFFER_INFO


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
