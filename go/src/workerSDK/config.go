package workerSDK

import (
	"flag"
	"io/ioutil"
	"encoding/json"
)

var conf = flag.String("c", "worker.conf" , "Specify the configuration file")

type config struct {
	Custom	string		`json:"custom"`
	Report	host		`json:"report"`
	Worker	worker		`json:"worker"`
	Next	[]worker	`json:"next"`
}

func loadConfig() (*config, error) {
	data,err := ioutil.ReadFile(*conf)
	if err != nil {
		return nil, err
	}

	var c config
	if err = json.Unmarshal(data, &c); err != nil {
		return nil, err
	}

	return &c, err
}