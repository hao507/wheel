package log

import (
	"testing"
	"fmt"
)

func TestGetDebugInfo(t *testing.T) {
	info := GetDebugInfo(3)
	fmt.Println("filepath:", info.FilePath)
	fmt.Println("filename:", info.FileName)
	fmt.Println("package:", info.PackagePath)
	fmt.Println("pkgname:", info.PackageName)
	fmt.Println("function:", info.FunctionName)
	fmt.Println("line:", info.LineNumber)
}
