use rand::Rng;
use std::collections::HashMap;


fn main() {
    let mut rng = rand::thread_rng();
    let mut res = Vec::new();
        
    let lib: [u8; 4] = [41, 21, 1, 4];

    println!("gen array... ");

    for _ in 0..1_000_000_000 {
        let i: usize = rng.gen();
        res.push(lib[i % 4]);
    }

    println!("counting stats... ");

    let mut stats: HashMap<u8, u32> = HashMap::new();

    for val in res.iter() {
        stats.entry(*val)
            .and_modify(|count| *count += 1)
            .or_insert(1);
    }

    for (key, value) in stats {
        println!("item {} occurs {} times", key, value);
    }


    return ()

}