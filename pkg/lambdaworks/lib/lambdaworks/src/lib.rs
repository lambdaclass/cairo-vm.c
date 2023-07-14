use lambdaworks_math::{
    field::element::FieldElement,
    field::fields::fft_friendly::stark_252_prime_field::Stark252PrimeField,
    unsigned_integer::element::UnsignedInteger,
};

type Felt = FieldElement<Stark252PrimeField>;
type Limbs = *mut u64;

fn felt_to_limbs(felt: Felt, limbs: Limbs) {
    let representative = felt.representative().limbs;
    for i in 0..4 {
        let u = i as usize;
        unsafe {
            *limbs.offset(i) = representative[u];
        }
    }
}

fn limbs_to_felt(limbs: Limbs) -> Felt {
    unsafe {
        let slice: &mut [u64] = std::slice::from_raw_parts_mut(limbs, 4);
        let array: [u64; 4] = slice.try_into().unwrap();
        let ui = UnsignedInteger::from_limbs(array);
        let felt = Felt::from(&ui);
        return felt;
    }
}

#[no_mangle]
pub extern "C" fn from(result: Limbs, value: u64) {
    felt_to_limbs(Felt::from(value), result);
}

#[no_mangle]
pub extern "C" fn zero(result: Limbs) {
    felt_to_limbs(Felt::zero(), result)
}

#[no_mangle]
pub extern "C" fn one(result: Limbs) {
    felt_to_limbs(Felt::one(), result)
}

#[no_mangle]
pub extern "C" fn add(a: Limbs, b: Limbs, result: Limbs) {
    felt_to_limbs(limbs_to_felt(a) + limbs_to_felt(b), result);
}

#[no_mangle]
pub extern "C" fn sub(a: Limbs, b: Limbs, result: Limbs) {
    felt_to_limbs(limbs_to_felt(a) - limbs_to_felt(b), result)
}

#[no_mangle]
pub extern "C" fn mul(a: Limbs, b: Limbs, result: Limbs) {
    felt_to_limbs(limbs_to_felt(a) * limbs_to_felt(b), result)
}

#[no_mangle]
pub extern "C" fn div(a: Limbs, b: Limbs, result: Limbs) {
    felt_to_limbs(limbs_to_felt(a) / limbs_to_felt(b), result)
}
