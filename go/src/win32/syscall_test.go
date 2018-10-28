package win32

import (
	"testing"
	"fmt"
	)

func TestNewLazyDLL(t *testing.T) {
	kernel := NewLazyDLL("kernel32.dll")
	fmt.Println(kernel)
}

func TestLazyDLL_NewProcedure(t *testing.T) {
	kernel := NewLazyDLL("kernel32.dll")
	fmt.Println(kernel)
	fptr := kernel.NewProcedure("GetVersion")
	fmt.Println(fptr)
}
