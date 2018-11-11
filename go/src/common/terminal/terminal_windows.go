// +build windows

package terminal

import (
	"win32"
	"fmt"
	"io"
)

var colors = map[int]int{
	Black                    : 0x00,
	Blue                     : 0x01,
	Green                    : 0x02,
	Cyan                     : 0x03,
	Red                      : 0x04,
	Purple                   : 0x05,
	Yellow                   : 0x06,
	White                    : 0x07,

	Gray                      : 0x08,
	LightBlue                : 0x09,
	LightGreen               : 0x0A,
	LightCyan                : 0x0B,
	LightRed                 : 0x0C,
	LightPurple              : 0x0D,
	LightYellow              : 0x0E,
	LightWhite               : 0x0F,
}

func ColorWrite(writer io.Writer, color int, format string, v... interface{}) {
	c,ok := colors[color]
	if !ok {
		writer.Write([]byte(fmt.Sprintf(format, v...)))
		return
	}

	hStdOut, err := win32.GetStdHandle(win32.STD_OUTPUT_HANDLE)
	if err == nil {
		var csbi win32.CONSOLE_SCREEN_BUFFER_INFO
		win32.GetConsoleScreenBufferInfo(hStdOut, &csbi)

		wColor := (csbi.WAttributes & uint16(0xF0)) + (uint16(c) & uint16(0x0F))
		win32.SetConsoleTextAttribute(hStdOut, wColor)

		writer.Write([]byte(fmt.Sprintf(format, v...)))

		win32.SetConsoleTextAttribute(hStdOut, csbi.WAttributes)
	} else {
		writer.Write([]byte(fmt.Sprintf(format, v...)))
	}
}