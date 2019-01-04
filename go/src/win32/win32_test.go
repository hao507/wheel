package win32

import (
	"testing"
	"fmt"
	"syscall"
	"time"
	"github.com/shirou/w32"
	"unsafe"
)


// test mutex
func TestCreateMutexW(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if h == NULL {
		panic(err)
	}

	go func () {
		syscall.WaitForSingleObject(syscall.Handle(h), syscall.INFINITE)
		for i := 1; i < 10; i++ {
			fmt.Println("I'm go run :", i)
		}
		ReleaseMutex(h)
	}()

	time.Sleep(1 * time.Second)
	WaitForSingleObject(h, INFINITE)
	ReleaseMutex(h)
	CloseHandle(h)

}

func TestReleaseMutex(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if h == NULL {
		panic(err)
	}
	i := 1
	t1 := func (id int) {
		for {
			WaitForSingleObject(h, syscall.INFINITE)
			if i > 1000 {
				ReleaseMutex(h)
				break
			}
			fmt.Println("I'm go ", id, " run :", i)
			i++
			ReleaseMutex(h)
		}
	}

	for i := 0; i < 5; i++ {
		go t1(i + 1)
	}

	time.Sleep(5 * time.Second)
	WaitForSingleObject(h, syscall.INFINITE)
	ReleaseMutex(h)
	CloseHandle(h)
}

func TestWaitForSingleObject(t *testing.T) {
	h,err := CreateMutexW(nil, false, nil)
	if h == NULL {
		panic(err)
	}

	go func () {
		WaitForSingleObject(h, INFINITE)
		for i := 1; i < 10; i++ {
			fmt.Println("I'm go run :", i)
		}
		ReleaseMutex(h)
	}()

	time.Sleep(1 * time.Second)
	WaitForSingleObject(h, INFINITE)
	ReleaseMutex(h)
	CloseHandle(h)
}

// test terminal
func TestGetConsoleScreenBufferInfo(t *testing.T) {
	h,err := GetStdHandle(syscall.STD_OUTPUT_HANDLE)
	if h == NULL {
		panic(err)
	}
	var csbi CONSOLE_SCREEN_BUFFER_INFO

	if ok,err := GetConsoleScreenBufferInfo(h, &csbi); ok {
		if ok,err = SetConsoleTextAttribute(h, w32.FOREGROUND_RED | w32.FOREGROUND_GREEN); ok {
			fmt.Println("color set success.")
			SetConsoleTextAttribute(h,csbi.WAttributes)
		} else {
			fmt.Println("color set fail:", err.Error())
		}
	} else {
		fmt.Println("get console attr fail:", err.Error())
	}
}

func TestSetConsoleTextAttribute(t *testing.T) {
	h,err := GetStdHandle(syscall.STD_OUTPUT_HANDLE)
	if h == NULL {
		panic(err)
	}

	if ok,err := SetConsoleTextAttribute(h, w32.FOREGROUND_RED | w32.FOREGROUND_GREEN); ok {
		fmt.Println("color set success.")
	} else {
		fmt.Println("color set fail:", err.Error())
	}
}

// test memory
func TestGlobalMemoryStatusEx(t *testing.T) {
	var statex MEMORYSTATUSEX
	statex.DwLength = uint32(unsafe.Sizeof(statex))
	if ok,err := GlobalMemoryStatusEx(&statex); !ok {
		panic(err)
	}
	fmt.Printf("There is  %d percent of memory in use.\n", statex.DwMemoryLoad)
	fmt.Printf("There are %d total Kbytes of physical memory.\n", statex.UllTotalPhys / 1024)
	fmt.Printf("There are %d free Kbytes of physical memory.\n", statex.UllAvailPhys / 1024)
	fmt.Printf("There are %d total Kbytes of paging file.\n", statex.UllTotalPageFile / 1024)
	fmt.Printf("There are %d free Kbytes of paging file.\n", statex.UllAvailPageFile / 1024)
	fmt.Printf("There are %d total Kbytes of virtual memory.\n", statex.UllTotalVirtual / 1024)
	fmt.Printf("There are %d free Kbytes of virtual memory.\n", statex.UllAvailVirtual / 1024)
	fmt.Printf("There are %d free Kbytes of extended memory.\n", statex.UllAvailExtendedVirtual / 1024)
}

// test disk
func TestGetDiskFreeSpaceExA(t *testing.T) {
	var qwFreeBytesToCaller, qwTotalBytes, qwFreeBytes uint64
	device := []byte("C:")
	c := &device[0]

	if ok,err := GetDiskFreeSpaceExA(c,
		PULARGE_INTEGER(unsafe.Pointer(&qwFreeBytesToCaller)),
		PULARGE_INTEGER(unsafe.Pointer(&qwTotalBytes)),
		PULARGE_INTEGER(unsafe.Pointer(&qwFreeBytes))); !ok {
		panic(err)
	}

	fmt.Printf("可获得的空闲空间（字节）: \t%v\n", qwFreeBytesToCaller)
	fmt.Printf("空闲空间（字节）: \t\t%v\n", qwFreeBytes)
	fmt.Printf("磁盘总容量（字节）: \t\t%v\n", qwTotalBytes)
}

func strlen(buf *[]byte) int {
	len := 0
	for (*buf)[len] != 0 {
		len++
	}

	return len
}

func TestGetLogicalDriveStringsA(t *testing.T) {
	const MAX_PATH = 260
	buf := make([]byte, MAX_PATH)
	if ret,err := GetLogicalDriveStringsA(MAX_PATH - 1, &buf[0]); ret == NULL {
		panic(err)
	}
	var dev []string
	begin := 0
	for begin < MAX_PATH {
		b := buf[begin:]
		l := strlen(&b)
		if l == 0 {
			break
		}
		dev = append(dev, string(b[:l]))
		begin += l + 1
	}

	fmt.Println(dev)
}

func TestGetVolumeInformationA(t *testing.T) {
	const MAX_PATH = 260
	dwPath := make([]byte, MAX_PATH)
	if ret,err := GetLogicalDriveStringsA(MAX_PATH - 1, &dwPath[0]); ret == NULL {
		panic(err)
	}
	s := strlen(&dwPath)

	//fmt.Println(bytes.Count(buf, []byte{0}))
	//fmt.Println(string(buf))

	p := 0

	for dwPath[p] != 0 {
		volumeNmaeBuff := make([]byte, MAX_PATH)
		fileSystemName := make([]byte, MAX_PATH)
		var volumeSerialNumber, maximumComponentLength, fileSystemFlag uint32

		if ok,err := GetVolumeInformationA(&dwPath[p],
			&volumeNmaeBuff[0],
			MAX_PATH,
			&volumeSerialNumber,
			&maximumComponentLength,
			&fileSystemFlag,
			&fileSystemName[0],
			MAX_PATH); !ok {
			panic(err)
		}

		fmt.Println("=======================================================")
		fmt.Println(string(dwPath[p:p + s]))
		fmt.Println("驱动器卷标:", string(volumeNmaeBuff))
		fmt.Println("最大文件名长度:", volumeSerialNumber)
		fmt.Println("文件系统标识:", maximumComponentLength)
		fmt.Println("文件系统类型:", string(fileSystemName))

		p += s + 1
	}

}

// test cpu
func CompareFileTime2(time1, time2 FILETIME) uint32 {
	a := time1.DwHighDateTime << 32 | time1.DwLowDateTime
	b := time2.DwHighDateTime << 32 | time2.DwLowDateTime

	return b - a
}

func TestGetSystemTimes(t *testing.T) {
	var m_preidleTime		FILETIME
	var m_prekernelTime		FILETIME
	var m_preuserTime		FILETIME

	var	idleTime	FILETIME
	var	kernelTime	FILETIME
	var	userTime	FILETIME

	GetSystemTimes(&m_preidleTime, &m_prekernelTime, &m_preuserTime)

	time.Sleep(1 * time.Second)

	if ok,err := GetSystemTimes(&idleTime, &kernelTime, &userTime); !ok {
		panic(err)
	}

	idle := CompareFileTime2(m_preidleTime, idleTime)
	kernel := CompareFileTime2(m_prekernelTime, kernelTime)
	user := CompareFileTime2(m_preuserTime, userTime)

	if kernel + user == 0 {
		fmt.Println("0.0")
	} else {
		//（总的时间 - 空闲时间）/ 总的时间 = 占用cpu的时间就是使用率
		cpu := (kernel + user - idle) * 100 / (kernel + user)
		fmt.Println(cpu)
	}
}

func TestGetSystemTimes2(t *testing.T) {
	for {
		TestGetSystemTimes(t)
	}
}

func TestExitProcess(t *testing.T) {
	err := ExitProcess(0)
	// program is exited, don't print.
	fmt.Println(err)
}

func Uint16ToBytes(n uint16) []byte {
	return []byte{
		byte(n),
		byte(n >> 8),
	}
}

func BytesToUint16(array []byte) uint16 {
	var data uint16 =0
	for i:=0;i< len(array);i++  {
		data = data+uint16(uint(array[i])<<uint(8*i))
	}

	return data
}

func Uint32ToBytes(n uint32) []byte {
	return []byte{
		byte(n),
		byte(n >> 8),
		byte(n >> 16),
		byte(n >> 24),
	}
}

func Uint64ToBytes(n uint64) []byte {
	return []byte{
		byte(n),
		byte(n >> 8),
		byte(n >> 16),
		byte(n >> 24),
		byte(n >> 32),
		byte(n >> 40),
		byte(n >> 48),
		byte(n >> 56),
	}
}

// TODO have some bug.
func TestFormatMessageW(t *testing.T) {
	buf := make([]uint16, 1024)
	ret,err := FormatMessageW(uint32(FORMAT_MESSAGE_ALLOCATE_BUFFER) | uint32(FORMAT_MESSAGE_FROM_SYSTEM),
		NULL,
		6,
		uint32((((WORD  )(0x00)) << 10) | (WORD  )(0x01)),
		&buf[0],
		0,
		nil)
	if ret == FALSE {
		fmt.Println(ret, err)
	} else {
		var b []byte
		for _,v := range buf {
			b = append(b, Uint16ToBytes(v)...)
		}
		fmt.Println(string(b))
	}
}