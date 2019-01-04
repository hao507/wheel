package main

import (
	"encoding/json"
	"fmt"
)

type test struct {
	Arr  []int   `json:"arr"`
}

func main() {
	var arr test
	arr.Arr = make([]int, 0)
	data,err := json.Marshal(arr)
	if err != nil {
		panic(err)
	}
	fmt.Println(string(data))
}
