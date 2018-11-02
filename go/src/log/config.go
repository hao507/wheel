package log

import (
	"os"
	"encoding/json"
	"bytes"
	"fmt"
	"io/ioutil"
)

// default file config
const defaultFileSize = "1024mb"
const defaultFileCount = 2

type file struct {
	Name		string	`json:"name"`			// file name.
	Size		string	`json:"size"`			// single file size.
	Count		int		`json:"count"`		// files count.
}

type Config struct {
	Color		bool	`json:"color"`		// false: only level name with color, true: then all with color.
	Level		string	`json:"level"`		// logger level.
	Terminal	bool	`json:"terminal"`		// output to terminal.
	File		[]file	`json:"file"`			// output to file.
}

func NewConfig(color bool, level string, terminal bool) *Config {
	return &Config{Color:color, Level:level, Terminal:terminal, File:[]file{file{Name:"", Size:"", Count:0}}}
}

func NewConfigFromFile(filename string) (*Config, error) {
	b,err := ioutil.ReadFile(filename)
	if err != nil {
		return nil, err
	}

	var c Config

	err = json.Unmarshal(b, &c)
	if err != nil {
		return nil, err
	}

	return &c, nil
}

func NewConfigFromJson(js string) (*Config, error) {
	var c Config

	err := json.Unmarshal([]byte(js), &c)
	if err != nil {
		return nil, err
	}

	return &c, nil
}

func NewConfigFromLogger(log *Logger) *Config {
	color := log.color
	level := "INFO"
	terminal := false
	var files []file

	switch log.level {
	case TRACE:
		level = "TRACE"
	case DEBUG:
		level = "DEBUG"
	case INFO:
		level = "INFO"
	case WARNING:
		level = "WARNING"
	case ERROR:
		level = "ERROR"
	case FATAL:
		level = "FATAL"
	default:
		level = "INFO"
	}

	for _,w := range log.out {
		if w == os.Stderr || w == os.Stdout || w == os.Stdin {
			terminal = true
		} else {
			files = append(files, file{Name:w.Name(), Size:defaultFileSize, Count:defaultFileCount})
		}
	}

	return &Config{Color:color, Level:level, Terminal:terminal, File:files}
}

func (c *Config) ToLogger(log *Logger) {
	color := c.Color
	level := INFO
	var out []Writer

	switch {
	case c.Level == "trace" || c.Level == "TRACE" || c.Level == "Trace":
		level = TRACE
	case c.Level == "debug" || c.Level == "DEBUG" || c.Level == "Debug":
		level = DEBUG
	case c.Level == "info" || c.Level == "INFO" || c.Level == "Info":
		level = INFO
	case c.Level == "warning" || c.Level == "WARNING" || c.Level == "Warning":
		level = WARNING
	case c.Level == "error" || c.Level == "ERROR" || c.Level == "Error":
		level = ERROR
	case c.Level == "fatal" || c.Level == "FATAL" || c.Level == "Fatal":
		level = FATAL
	default:
		level = INFO
	}

	if c.Terminal {
		out = append(out, defaultWriter)
	}

	for _,f := range c.File {
		if f.Name != "" && f.Size != "" && f.Count != 0 {
			var size int
			var unit string
			fmt.Sscanf(f.Size, "%d%s", &size, &unit)

			switch {
			case unit == "b" || unit == "B" || unit == "":
			case unit == "k" || unit == "K" || unit == "kb" || unit == "KB":
				size *= 1024
			case unit == "m" || unit == "M" || unit == "mb" || unit == "MB":
				size *= 1024 * 1024
			case unit == "g" || unit == "G" || unit == "gb" || unit == "GB":
				size *= 1024 * 1024 * 1024
			}

			fh,err := NewRotatingFileHandler(f.Name, size, f.Count)
			if err == nil {
				out = append(out, fh)
			}
		}
	}

	log.mutex.Lock()
	defer log.mutex.Unlock()

	if log.lock != nil {
		log.lock.Lock()
		defer log.lock.Unlock()
	}

	for _,out := range log.out {
		if out != os.Stdout && out != os.Stderr && out != os.Stdin {
			out.Close()
		}
	}
	log.out = out
	log.color = color
	log.level = level
}

func (c *Config) ToJson() ([]byte, error) {
	js,err := json.Marshal(c)
	if err != nil {
		return nil, err
	}

	var buf bytes.Buffer
	err = json.Indent(&buf, js, "", "\t")
	if err != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}

func (c *Config) ToFile(filename string) error {
	f,err := os.OpenFile(filename, os.O_CREATE|os.O_RDWR|os.O_TRUNC, 0666)
	if err != nil {
		return err
	}
	defer f.Close()

	js,err := c.ToJson()
	if err != nil {
		return err
	}

	_,err = f.Write(js)
	if err != nil {
		return err
	}

	return nil
}