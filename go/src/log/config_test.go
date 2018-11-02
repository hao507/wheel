package log

import (
	"testing"
	"fmt"
)

func TestNewConfig(t *testing.T) {
	fmt.Println("------------------ config test ------------------")
	conf := NewConfig(true, "info", true)
	oldConf := NewConfigFromLogger(std)
	conf.ToLogger(std)
	Trace("Trace")
	Debug("Debug")
	Info("Info")
	Warning("Warning")
	Error("Error")
	Fatal("Fatal")
	oldConf.ToLogger(std)
}

func TestNewConfigFromJson(t *testing.T) {
	fmt.Println("------------- config from json -------------")
	js := `{
		"color":true,
		"level":"INFO",
		"terminal":true
	}`

	c,err := NewConfigFromJson(js)
	if err != nil {
		panic(err)
	}

	bak := NewConfigFromLogger(std)
	Info("test info begin")
	c.ToLogger(std)
	Info("test info current")
	bak.ToLogger(std)
	Info("test info after")
}

func TestConfig_ToFile(t *testing.T) {
	conf := NewConfigFromLogger(std)
	if err := conf.ToFile("./log.conf"); err != nil {
		panic(err)
	}
}