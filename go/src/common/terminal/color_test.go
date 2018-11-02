package terminal

import (
	"testing"
	"os"
)

func TestColorPrintf(t *testing.T) {
	ColorWrite(os.Stdout, Black, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, Blue, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, Green, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, Cyan, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, Red, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, Purple, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, Yellow, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, White, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, Gray, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, LightBlue, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, LightGreen, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, LightCyan, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, LightRed, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, LightPurple, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, LightYellow, "test--------------------------------------------\n")
	ColorWrite(os.Stdout, LightWhite, "test--------------------------------------------\n")
}