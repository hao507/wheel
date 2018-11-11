package log

import (
	"testing"
	"fmt"
	"time"
)

func TestNew(t *testing.T) {
	fmt.Println("------------------ logger test ------------------")
	log := New(nil)
	log.Trace("Trace")
	log.Debug("Debug")
	log.Info("Info")
	log.Warning("Warning")
	log.Error("Error")
	log.Fatal("Fatal")
}

func TestLogger_SetEnableMQ(t *testing.T) {
	fmt.Println("------------------ logger test ------------------")

	log := New(nil)
	log.Trace("Trace")
	log.Debug("Debug")
	log.Info("Info")
	log.Warning("Warning")
	log.Error("Error")
	log.Fatal("Fatal")

	log.SetEnableMQ(true)
	for i:=0; i<1000; i++ {
		if i == 100 {
			time.Sleep(2 * time.Second)
			//log.SetEnableMQ(false)
			//log.SetEnableMQ(true)
		}
		if i == 200 {
			//log.SetEnableMQ(true)
		}
		log.Info("info %d", i)
	}
}