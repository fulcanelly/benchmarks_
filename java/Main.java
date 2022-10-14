import java.util.HashMap;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadLocalRandom;

public class Main {
    static byte[] lib = new byte[] {112,31,3,7};

    static int cores = Runtime.getRuntime().availableProcessors();

    static int size = 1000 * 1000 * 1000;

    void generateChunk(int from, int to, byte[] arr) {
        var random = ThreadLocalRandom.current();
        System.out.printf("Generating chunk %d:%d\n", from, to);
        for (int i = from; i < to; i++) {
            arr[i] = lib[random.nextInt(4)];
        }
        System.out.printf("End generating chunk %d:%d\n", from, to);
    }

    HashMap<Byte, Integer> countChunk(int from, int to, byte[] arr) {
        var hash = new HashMap<Byte, Integer>();
        System.out.printf("Counting chunk %d:%d\n", from, to);

        for (int i = from; i < to; i++) {
            var elem = arr[i];

            var value = hash.get(elem);
            if (value == null) {
                hash.put(elem, 1);
            } else {
                hash.put(elem, value + 1);
            }
        }
        System.out.printf("End counting chunk %d:%d\n", from, to);

        return hash;
    }

    BlockingQueue<HashMap<Byte, Integer>> result = new LinkedBlockingQueue<>();

    void chunkRun(int from, int to, byte[] arr) {
        generateChunk(from, to, arr);
        result.add(countChunk(from, to, arr));
    }

    HashMap<Byte, Integer> mergeMaps(HashMap<Byte, Integer> a, HashMap<Byte, Integer> b) {
        HashMap<Byte, Integer> result = (HashMap<Byte, Integer>)a.clone();

        for (var elem : b.entrySet()) {
            var update = result.getOrDefault(elem.getKey(), 0) + elem.getValue();
            result.put(elem.getKey(), update);
        }

        return result;
    }

    HashMap<Byte, Integer> getElem() {
        try {
            return result.take();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }    

    void parallelTest() {
        byte[] arr = new byte[size];

        var chunkSize = size / cores;

        for (int i = 0; i < size; i += chunkSize) {
            var from = i;
            var to = Math.min(i + chunkSize, size);
            new Thread(() -> chunkRun(from, to, arr)).start();;
        }

        var result = new HashMap<Byte, Integer>();

        for (int i = 0; i < cores; i++) {
            System.out.println("getting " + i);
            var el = getElem();
            System.out.println(el);
            result = mergeMaps(el, result);
        }

        System.out.println(result);

    }


    public static void main(String[] args) {
        new Main().parallelTest();
    }
}