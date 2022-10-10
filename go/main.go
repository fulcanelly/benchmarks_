package main

import (
	"fmt"
	"math/rand"
	"time"
)

func getRand() *rand.Rand {
	source := rand.NewSource(time.Now().UnixNano())
	return rand.New(source)
}

func main() {
	var list []byte
	const size = 1000 * 1000 * 1000
	n := 0

	var lib [4]byte
	lib[0] = 44
	lib[1] = 12
	lib[2] = 112
	lib[3] = 224

	var rand_ = getRand()
	fmt.Println("Generating list..")

	for n < size {
		n += 1
		list = append(
			list, lib[rand_.Intn(4)],
		)
	}


	var stats map[byte]int;
	
	stats = make(map[byte]int);
	fmt.Println("Counting...")

	for _, item := range list {
		stats[item]++
	} 

	fmt.Println("Result:")

	for k, v := range stats {
		fmt.Printf(
			"Got %d: %d times\n", k, v)

	}

}
