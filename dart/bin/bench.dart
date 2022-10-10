import 'dart:math';

final rand = Random();

void main() {
  final set = [22, 23, 312, 123]; //

  var arr = <int>[];

  print("Generating list..");
  for (var i = 0; i < 1000 * 1000 * 100; i++) {
    arr.add(set[rand.nextInt(4)]);
  }

  var countByChar = <int, int>{};

  print("Counting...");
  for (var item in arr) {
    var old = countByChar.putIfAbsent(item, () => 0);

    countByChar[item] = old + 1;
  }

  print("Result: ");
  for (var key in countByChar.keys) {
    print("Char '${key}' count is ${countByChar[key]} ");
  }
}
