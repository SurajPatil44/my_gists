#![allow(non_snake_case)]

use std::sync::mpsc::{Receiver, Sender};
use std::sync::{
    atomic::{AtomicBool, Ordering},
    mpsc, Arc, Mutex,
};
use std::{thread, time};

#[derive(Debug, Eq, PartialEq, Clone, Copy)]
enum State {
    Started,
    Doing,
    Done,
}

fn StateHandler(arc_state: Arc<Mutex<State>>, sender: Sender<State>) {
    let mut state = arc_state.lock().unwrap();
    *state = State::Doing;
    sender.send(*state).expect("first send");
    thread::sleep(time::Duration::from_secs(3));
    *state = State::Done;
    sender.send(*state).expect("failed in second send");
}

fn StateRunner(reciver: Receiver<State>, done: Arc<AtomicBool>) {
    for state in reciver.iter() {
        if state == State::Done {
            done.store(true, Ordering::SeqCst);
        }
    }
}

fn main() {
    let state = Arc::new(Mutex::new(State::Started));
    let stateclone = Arc::clone(&state);
    let (tx, rx) = mpsc::channel();
    let done = Arc::new(AtomicBool::new(false));
    let doneclone = Arc::clone(&done);
    let handler = thread::spawn(move || StateHandler(stateclone, tx.clone()));
    let handler2 = thread::spawn(move || StateRunner(rx, doneclone));
    handler.join().unwrap();
    handler2.join().unwrap();
    while !done.load(Ordering::SeqCst) {};
    println!("{:#?}", *state);
}
