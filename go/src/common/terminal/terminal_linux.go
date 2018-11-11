// +build linux

package terminal

import (
	"fmt"
	"io"
)

var colors = map[int]string{
	Black                    : "30m",
	Blue                     : "34m",
	Green                    : "32m",
	Cyan                     : "36m",
	Red                      : "31m",
	Purple                   : "35m",
	Yellow                   : "33m",
	White                    : "37m",

	Gray                      : "1;30m",
	LightBlue                : "1;34m",
	LightGreen               : "1;32m",
	LightCyan                : "1;36m",
	LightRed                 : "1;31m",
	LightPurple              : "1;35m",
	LightYellow              : "1;33m",
	LightWhite               : "1;37m",
}

func ColorWrite(writer io.Writer, color int, format string, v... interface{}) {
	c,ok := colors[color]
	if !ok {
		writer.Write([]byte(fmt.Sprintf(format, v...)))
		return
	}

	writer.Write([]byte("\033[" + c + fmt.Sprintf(format, v...) + "\033[0m"))
}
