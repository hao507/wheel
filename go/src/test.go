package main

/*
#include <windows.h>
#include <stdio.h>
#include <conio.h>

double FileTimeToDouble(FILETIME* pFiletime)
{
	return (double)((*pFiletime).dwHighDateTime * 4.294967296E9) + (double)(*pFiletime).dwLowDateTime;
}

double m_fOldCPUIdleTime;
double m_fOldCPUKernelTime;
double m_fOldCPUUserTime;


BOOL Initialize()
{
	FILETIME ftIdle, ftKernel, ftUser;
	BOOL flag = FALSE;
	if (flag = GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
	{
		m_fOldCPUIdleTime = FileTimeToDouble(&ftIdle);
		m_fOldCPUKernelTime = FileTimeToDouble(&ftKernel);
		m_fOldCPUUserTime = FileTimeToDouble(&ftUser);

	}
	return flag;
}

int GetCPUUseRate()
{
	int nCPUUseRate = -1;
	FILETIME ftIdle, ftKernel, ftUser;
	if (GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
	{
		double fCPUIdleTime = FileTimeToDouble(&ftIdle);
		double fCPUKernelTime = FileTimeToDouble(&ftKernel);
		double fCPUUserTime = FileTimeToDouble(&ftUser);
		nCPUUseRate= (int)(100.0 - (fCPUIdleTime - m_fOldCPUIdleTime) / (fCPUKernelTime - m_fOldCPUKernelTime + fCPUUserTime - m_fOldCPUUserTime)*100.0);
		m_fOldCPUIdleTime = fCPUIdleTime;
		m_fOldCPUKernelTime = fCPUKernelTime;
		m_fOldCPUUserTime = fCPUUserTime;
	}
	return nCPUUseRate;
}
int cpu()
{
	if (!Initialize())
	{
		getch();
		return -1;
	}
	else
	{
		Sleep(1000);
		return GetCPUUseRate();
	}
	return -1;
}

*/
import "C"

import (
	"fmt"
	"time"
	"github.com/shirou/gopsutil/mem"
	"github.com/shirou/gopsutil/disk"
	"github.com/shirou/gopsutil/cpu"
	"syscall"
	"unsafe"
	"strings"
	"strconv"
	"bytes"
	"encoding/binary"
)

func main0()  {



	str := "asfsdf123"
	b := []byte(str)
	var begin, end = 0, 0
	for i := 0; i < len(b); i++ {
		if b[i] >= '0' && b[i] <= '9' {
			begin = i
			for j := i + 1; j <= len(b); j++ {
				if j == len(b) || b[j] < '0' || b[j] > '9'{
					end = j
					break
				}
			}
			break
		}
	}

	if begin >= end {
		fmt.Println("error : begin >= end")
		fmt.Println(begin)
		fmt.Println(end)
	} else {
		bb := b[begin:end]
		fmt.Println(string(bb))
	}


	var s string
	fmt.Sscanf(str, "%S", &s)
	fmt.Println(s)
	return

	//for {
	//	cpu := C.cpu()
	//	fmt.Printf("\r%d%%", cpu)
	//}


	h := syscall.MustLoadDLL("kernel32.dll")
	c := h.MustFindProc("GetDiskFreeSpaceExW")
	lpFreeBytesAvailable := int64(0)
	lpTotalNumberOfBytes := int64(0)
	lpTotalNumberOfFreeBytes := int64(0)
	_, _, err := c.Call(uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr("F:"))),
		uintptr(unsafe.Pointer(&lpFreeBytesAvailable)),
		uintptr(unsafe.Pointer(&lpTotalNumberOfBytes)),
		uintptr(unsafe.Pointer(&lpTotalNumberOfFreeBytes)))

	if err != nil {
		fmt.Println(lpFreeBytesAvailable)
		fmt.Println(lpTotalNumberOfBytes)
		fmt.Println(lpTotalNumberOfFreeBytes)
	} else {
		fmt.Println(err)
	}



	return

	info,_ := cpu.Percent(1 * time.Second, false)
	fmt.Printf("cpu:%v%%\n", info[0])
	v, _ := mem.VirtualMemory()
	// almost every return value is a struct
	fmt.Printf("mem:Total: %v, Used:%v, Free:%v, UsedPercent:%f%%\n", v.Total, v.Used, v.Free, v.UsedPercent)



	p,_ := disk.Partitions(false)
	for _,info := range p {

		uu,_ := disk.Usage(info.Device)
		fmt.Println(uu.UsedPercent)

		//io,_ := disk.IOCounters(info.Device)
		//for k,v := range io {
		//	fmt.Println("k=", k, "v=", v)
		//}
		//disk.Usage()
		//fmt.Println(info.Device, info.Opts, info.Mountpoint)
		//fmt.Println(info.String())
	}

	return

	// convert to JSON. String() is also implemented
	fmt.Println(v)

	for i := 0; i < 5; i++ {
	}

	//ps,_ := disk.Partitions(false)
	u,_ := disk.Usage("/")
	fmt.Println("disk read:", u.UsedPercent)




	io,err := disk.IOCounters("/")
	if err != nil {
		fmt.Println(err)
		return
	}

	fmt.Println(io["/"].ReadBytes)

	return


	var queue chan string = make(chan string, 1024)
	go func(){
		for {
			select {
			case entry := <- queue :
				fmt.Println("read: " + entry)
			}
		}
	}()

	for i := 0; i < 10; i++ {
		queue <- fmt.Sprintf("%d", i + 1)
		time.Sleep(1 * time.Second)
	}
	close(queue)
	fmt.Println("is close")
	time.Sleep(10 * time.Second)
}

func main1() {
	str := "/dev/vda1       51604604 41459692   7523564  85% /"
	f := strings.Split(str, " *")
	fmt.Println(len(f))
}

func byteToInt(results []byte) int32 {
	bBuf := bytes.NewBuffer(results)
	var x int32
	binary.Read(bBuf, binary.BigEndian, &x)
	return x
}

func conv(num string) {

	d := make(map[string]interface{})
	var first,second byte
	fmt.Sscanf(num, "%2x%2x", &first, &second)
	firstInt := byteToInt([]uint8{0, 0, 0, first})
	secondInt := byteToInt([]uint8{0, 0, 0, second})
	temperature := float64((firstInt * 256) + secondInt) / 10
	value := strconv.FormatFloat(temperature, 'f', 2, 64)
	d[fmt.Sprintf("index%d",0)] = value

	fmt.Println(d)
}

func main() {
	conv("0161")
	conv("00e5")
}