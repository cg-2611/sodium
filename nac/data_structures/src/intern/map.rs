use std::cell::RefCell;
use std::hash::{BuildHasher, Hash, Hasher};

use hashbrown::hash_map::RawEntryMut;
use hashbrown::HashMap;

use crate::intern::{Interned, Interner};

#[derive(Default)]
pub struct InternMap<'a, T>(RefCell<HashMap<Interned<'a, T>, ()>>);

impl<'a, T> InternMap<'a, T> {
    pub fn new() -> Self {
        Self(RefCell::new(HashMap::new()))
    }
}

impl<'a, T: Hash + Eq> InternMap<'a, T> {
    pub fn compute_hash(&self, value: &T) -> u64 {
        let mut hasher = self.0.borrow().hasher().build_hasher();
        value.hash(&mut hasher);
        hasher.finish()
    }
}

impl<'a, T: Hash + Eq> Interner<'a, T> for InternMap<'a, T> {
    fn intern(&self, value: T, alloc: impl FnOnce(T) -> &'a T) -> Interned<'a, T> {
        let hash = self.compute_hash(&value);

        let mut map = self.0.borrow_mut();
        let entry = map.raw_entry_mut().from_key_hashed_nocheck(hash, &value);

        match entry {
            RawEntryMut::Occupied(entry) => *entry.key(),
            RawEntryMut::Vacant(entry) => {
                let interned_value = Interned::new(alloc(value));
                entry.insert_hashed_nocheck(hash, interned_value, ());
                interned_value
            }
        }
    }
}
