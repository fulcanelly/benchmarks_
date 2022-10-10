use rand::Rng;
use std::collections::HashMap;


fn main() {
    let mut rng = rand::thread_rng();
    let mut res = String::new();
        
    let symbols: [char; 4] = ['1','2','3','4'];

    println!("gen line... ");

    for _ in 0..1_000_000_000 {
        let i: usize = rng.gen();
        res.push(symbols[i % 4]);
    }

    println!("counting stats... ");

    let mut stats: HashMap<char, u32> = HashMap::new();

    for sym in res.chars() {
        stats.entry(sym)
            .and_modify(|count| *count += 1)
            .or_insert(1);
    }

    for (key, value) in stats {
        println!("char: {} occurs {} times", key, value);
    }


    return ()

}