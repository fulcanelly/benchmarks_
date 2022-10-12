package main

import (
	"fmt"
	"math/rand"
	"runtime"
	"time"
)

var lib [4]byte = [4]byte{44, 12, 112, 224}


func getNumber_(x *rand.Rand) byte {
	return lib[x.Intn(4)]
}


const size = 1_000_000_000

func getRand() *rand.Rand {
	source := rand.NewSource(time.Now().UnixNano())
	return rand.New(source)
}


type Pair[T interface {}] struct {
	start T
	end   T
}

func GenerateChunk(
	rands *rand.Rand,
	input []byte,
	_range Pair[int],
) {
	fmt.Println("Generating chunk ", _range)
	var i = _range.start 

	for i < _range.end {
		var num = getNumber_(rands)
		input[i] = num
		i++
	}
	fmt.Println("Done chunk ", _range)

}

func CountOccursInRage(
	input []byte,
	_range Pair[int],
	results chan map[byte]int,
) {
	fmt.Println("Counting chunk ", _range)

	var i = _range.start 
	var result map[byte]int
	result = make(map[byte]int)

	for i < _range.end {
		result[input[i]]++		
		i++ 
	}

	fmt.Println("Ending chunk ", _range)
	results <- result
}

func MergeCountMaps(
	first map[byte]int, 
	second map[byte]int,
) map[byte]int {
	var result map[byte]int = first

	for key, val := range second {
		result[key] += val
	}
	
	return result
}

func GenAndCountChunk(
	input []byte,
	_range Pair[int],
	results chan map[byte]int,
) {
	GenerateChunk(getRand(), input, _range)
	CountOccursInRage(input, _range, results)
}


func GenNCountParallel(
	input []byte,
	chunk_size int, 
	max int,
) map[byte]int {
	var start = 0

	var chans []chan map[byte]int
	
	for start < max {
		var recv = make(chan map[byte]int)
		chans = append(chans, recv);

		var _range =  Pair[int]{
			start, start + chunk_size,
		}

		go GenAndCountChunk(input, _range, recv);
		
		start += chunk_size

	}

 	var res map[byte]int 

	for _, ch := range chans {
		res = MergeCountMaps(<- ch, res)
	}

	return res
}


func main() {
	var cpus = runtime.NumCPU()

	var list []byte 

	var chunk_size = size / cpus
	
	list = make([]byte, size)

	var stats map[byte]int = GenNCountParallel(list, chunk_size, size)


	fmt.Println("Result:")

	for k, v := range stats {
		fmt.Printf(
			"Got %d: %d times\n", k, v)

	}

}
