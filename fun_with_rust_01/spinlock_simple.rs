#![allow(dead_code)]
#![allow(unused_imports)]
#![allow(unused_variables)]

use std::sync::{Arc,Mutex};
use std::{thread,time};

#[derive(Debug,Eq,PartialEq,Clone,Copy)]
enum State {
    Started,
    Done
}

fn StateHandler(mut arc_state : Arc<Mutex<State>>){
    let mut state = arc_state.lock().unwrap();
    thread::sleep(time::Duration::from_secs(3));
    *state = State::Done;
}


fn main() {
    let state = Arc::new(Mutex::new(State::Started));
    let stateclone = Arc::clone(&state);
    let handler = thread::spawn(move ||StateHandler(stateclone));
    loop {
        let state = state.lock().unwrap();
        println!("{:#?}",state);
        if *state == State::Done {
            //println!("{:#?}",state);
            break;
        }
    }
}