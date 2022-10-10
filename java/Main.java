import java.util.HashMap;
import java.util.Random;

public class Main {

    static void countSymbols(String str) {
        var chars = new char[str.length()];
        str.getChars(0, str.length(), chars, 0);
        
        var map = new HashMap<Character, Integer>();
        
        for (var sym : chars) {
            var last = map.get(sym);
            if (last == null) {
                map.put(sym, 0);
                last = 0;
            }
            
            map.put(sym, last + 1);
        }

        System.out.println(map);
    }

    public static void main(String[] args) {
        var builder = new StringBuilder();
        char[] chars = {'1','2','3','4'};
        var random = new Random();

        for (int i = 0; i < 1000 * 1000 * 1000; i++) {
            var sym = chars[random.nextInt(4)];
            builder.append(sym);
        }
        
        var str = builder.toString();
        countSymbols(str);


    }
}