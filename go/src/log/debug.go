package log

import (
	"runtime"
	"strings"
	"strconv"
)

type debugInfo struct {
	FilePath		string
	FileName		string
	PackagePath		string
	PackageName		string
	FunctionName	string
	LineNumber		string
}

func getCodeInfo(skip int) (funcname string, line int, filename string) {
	pc,filename,line,_ := runtime.Caller(skip)
	f := runtime.FuncForPC(pc)
	funcname = f.Name()

	return
}

func getDebugInfo(skip int) (di debugInfo) {
	defer func(){
		if err := recover(); err != nil {
			// todo some things
		}
	}()


	pathpkgfunc,linenum,fileurl := getCodeInfo(skip)

	// file path and name
	fileurl = strings.Replace(fileurl, "\\", "/", -1)
	str := strings.Split(fileurl, "/")
	length := len(str)
	filename := str[length - 1]
	filepath := strings.Split(fileurl, filename)[0]
	filepath = strings.TrimRight(filepath, `/\`)

	// package info
	str = strings.Split(pathpkgfunc, "/")
	length = len(str)

	// package path and function name
	pkgfunc := pathpkgfunc
	if length > 0 {
		pkgfunc = str[length - 1]
	}
	// package path
	pkgpath := strings.Split(pathpkgfunc, pkgfunc)[0]
	pkgpath = strings.Trim(pkgpath, `/\`)
	// package name and package function
	s := strings.Split(pkgfunc, ".")
	pkg := s[0]
	fun := s[1]
	line := strconv.Itoa(linenum)

	return debugInfo{FilePath:filepath, FileName:filename, PackagePath:pkgpath, PackageName:pkg, FunctionName:fun, LineNumber:line}
}

func GetDebugInfo(skip int) (debugInfo) {
	return getDebugInfo(skip)
}