package log

import (
	"time"
	"fmt"
)

// default logger
var std = New(defaultFormatCB)

// default config
func stdFormatCB(level int, format string, v... interface{}) string {
	dInfo := GetDebugInfo(5)
	tag := ""
	ts := time.Now().Format(time.RFC3339)
	switch level {
	case TRACE:
		tag = dInfo.FilePath + "/" + dInfo.FileName + ":" + dInfo.LineNumber
	case DEBUG:
		tag = dInfo.PackagePath + "/" + dInfo.PackageName + ":" + dInfo.LineNumber
	case INFO:
		tag = ""
	case WARNING:
		tag = dInfo.PackageName
	case ERROR:
		tag = dInfo.PackageName + ":" + dInfo.FunctionName
	case FATAL:
		tag = dInfo.FilePath + "/" + dInfo.FileName + ":" + dInfo.LineNumber
	}

	return ts + " " + tag + " " + fmt.Sprintf(format, v...)
}


// user configure interface

func AddWriters(writers ...Writer) {
	std.AddWriters(writers...)
}

func RemoveWriters(writers ...Writer) {
	std.RemoveWriters(writers...)
}

func SetLevel(level int){
	std.SetLevel(level)
}

func GetLevel() int {
	return std.GetLevel()
}

func SetFlock(lock Lock) {
	std.SetLock(lock)
}

func GetFlock() Lock {
	return std.GetLock()
}

func SetColor(flag bool) {
	std.SetColor(flag)
}

func GetColor() bool {
	return std.GetColor()
}

func SetFormatCB (cb formatCB) {
	std.SetFormatCB(cb)
}

func GetFormatCB () formatCB {
	return std.GetFormatCB()
}


// level out log.

func BlankLine() {
	std.BlankLine()
}

func Trace(format string, v... interface{}){
	std.logOut(TRACE, format, v...)
}

func Debug(format string, v... interface{}){
	std.logOut(DEBUG, format, v...)
}

func Info(format string, v... interface{}){
	std.logOut(INFO, format, v...)
}

func Warning(format string, v... interface{}){
	std.logOut(WARNING, format, v...)
}

func Error(format string, v... interface{}){
	std.logOut(ERROR, format, v...)
}

func Fatal(format string, v... interface{}){
	std.logOut(FATAL, format, v...)
}

func B() {
	std.BlankLine()
}

func T(format string, v... interface{}){
	std.logOut(TRACE, format, v...)
}

func D(format string, v... interface{}){
	std.logOut(DEBUG, format, v...)
}

func I(format string, v... interface{}){
	std.logOut(INFO, format, v...)
}

func W(format string, v... interface{}){
	std.logOut(WARNING, format, v...)
}

func E(format string, v... interface{}){
	std.logOut(ERROR, format, v...)
}

func F(format string, v... interface{}){
	std.logOut(FATAL, format, v...)
}