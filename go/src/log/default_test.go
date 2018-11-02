package log

import (
	"testing"
	"fmt"
)

func TestTrace(t *testing.T) {
	fmt.Println("------------------ default test ------------------")
	Trace("Trace")
	Debug("Debug")
	Info("Info")
	Warning("Warning")
	Error("Error")
	Fatal("Fatal")

	T("T")
	D("D")
	I("I")
	W("W")
	E("E")
	F("F")
}