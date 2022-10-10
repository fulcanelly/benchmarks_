import std/random
import std/tables

var list: seq[byte] = @[]

var lib: array[4, byte] = [
  byte 121, 13, 3, 16
]

echo "Generating..."

for i in 0 ..< 1_000_000_000:
  list.add(
    lib[rand(3)]
  )


echo "Counting..."

var countByByte = initTable[byte, int]()


for i in list:
  if countByByte.hasKey(i):
    countByByte[i] += 1
  else:
    countByByte[i] = 1


echo countByByte