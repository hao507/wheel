// +build linux

package process

import (
	"os"
	"fmt"
	"io/ioutil"
	"regexp"
	"errors"
	"strings"
	"strconv"
	)

// process info
type processInfo struct {
	pid			uint32
	ppid		uint32
	pgid		uint32
	sid			uint32
	name		string
	state		string
}

// get process pid list
func getProcessList() (processList []string, err error) {
	fileinfo,err := ioutil.ReadDir("/proc")
	if err != nil {
		return processList, err
	}

	for _,fi := range fileinfo {
		if fi.IsDir() {
			pattern := `^[\d]*$`
			reg := regexp.MustCompile(pattern)
			ret := reg.FindAllString(fi.Name(), -1)
			if len(ret) > 0{
				processList = append(processList, fi.Name())
			}
		}
	}

	return processList, err
}

// get process info
func getProcessInfo(pid uint32) (p *processInfo, err error) {
	if !FindProcess(pid) {
		return p, errors.New("process not found")
	}

	path := fmt.Sprintf("/proc/%d/stat", pid)

	file,err := os.Open(path)
	if err != nil {
		return p, err
	}
	defer file.Close()

	var buf = make([]byte, 1024)

	n,err := file.Read(buf)
	if err != nil {
		return p, err
	}

	var pinfo processInfo

	info := strings.Split(string(buf[:n]), " ")
	if len(info) < 5 {
		return p, errors.New("process incomplete information")
	}

	// pid
	if i,err := strconv.Atoi(info[0]); err != nil {
		return p, err
	} else {
		pinfo.pid = uint32(i)
	}

	// name
	pinfo.name = strings.Trim(info[1], "()")

	// state
	pinfo.state = info[2]

	// ppid
	if i,err := strconv.Atoi(info[3]); err != nil {
		return p, err
	} else {
		pinfo.ppid = uint32(i)
	}

	// pgid
	if i,err := strconv.Atoi(info[4]); err != nil {
		return p, err
	} else {
		pinfo.pgid = uint32(i)
	}

	// sid
	if i,err := strconv.Atoi(info[5]); err != nil {
		return p, err
	} else {
		pinfo.sid = uint32(i)
	}


	return &pinfo, err
}

// kill a process
func KillProcess(pid uint32) (bool) {
	p,err := os.FindProcess(int(pid))
	if err != nil {
		return false
	}

	err = p.Kill()
	if err != nil {
		return false
	}

	return true
}

// kill a process tree(current process except)
func KillProcessTree(pid uint32) (bool) {
	plist,err := getProcessList()
	if err != nil {
		return false
	}

	for _,p := range plist {
		ipid,err := strconv.Atoi(p)
		if err != nil {
			continue
		}

		cpid := uint32(ipid)

		cpinfo,err := getProcessInfo(cpid)
		if err != nil {
			continue
		}

		if cpinfo.ppid == pid && int(cpinfo.pid) != os.Getpid() {
			KillProcessTree(cpid)
			KillProcess(cpid)
		}
	}

	if FindProcess(pid) {
		KillProcess(pid)
	}

	return true
}

// find process
func FindProcess(pid uint32) (bool) {
	path := fmt.Sprintf("/proc/%d", pid)

	_, err := os.Stat(path)
	if err == nil {
		return true
	}

	if os.IsNotExist(err) {
		return false
	}

	return false
}
