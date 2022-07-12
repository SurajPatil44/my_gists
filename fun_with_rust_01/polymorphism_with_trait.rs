use core::marker::PhantomData;

trait Decoder<T>{
    fn decode(&mut self,string : &[T]) -> Vec<T>;
}

trait Encoder<T> {
    fn encode(&mut self,string : &[T]) -> Vec<T>;
}

struct Xor<T> {
    key : T
}
struct PlusMinus<u8> {
    _t : PhantomData<u8>
}

impl Encoder<u8> for Xor<u8> {
    fn encode(&mut self,string : &[u8]) -> Vec<u8> {
        string.iter().map(|x| x^self.key).collect::<Vec<u8>>()
    } 
}

impl Decoder<u8> for Xor<u8> {
    fn decode(&mut self,string : &[u8]) -> Vec<u8> {
        string.iter().map(|x| x^self.key).collect::<Vec<u8>>()
    } 
}

impl Encoder<u8> for PlusMinus<u8>{
    fn encode(&mut self,string : &[u8]) -> Vec<u8> {
        string.iter().map(|x| x+32u8).collect::<Vec<u8>>()
    } 
}

impl Decoder<u8> for PlusMinus<u8> {
    fn decode(&mut self,string : &[u8]) -> Vec<u8> {
        string.iter().map(|x| x-32u8).collect::<Vec<u8>>()
    } 
}



fn main() {
    let arr = b"HAVE A NICE DAY";
    let mut pm = PlusMinus::<u8> { _t : PhantomData };
    let mut xor : Xor<u8> = Xor{key : b'n'};
    let arr2 = pm.encode(&arr[..]);
    println!("{:?}",String::from_utf8_lossy(&arr2));
    let arr2 = xor.encode(&arr2[..]);
    println!("{:?}",String::from_utf8_lossy(&arr2));
    let arr3 = xor.decode(&arr2[..]);
    println!("{:?}",String::from_utf8_lossy(&arr3));
    let arr3 = pm.decode(&arr3[..]);
    //println!("{:?}",arr2);
    println!("{:?}",String::from_utf8_lossy(&arr3));
    println!("{:?}",arr3);
}