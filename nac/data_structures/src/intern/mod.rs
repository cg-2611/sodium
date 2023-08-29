use std::borrow::Borrow;
use std::hash::Hash;
use std::ops::Deref;

pub use map::InternMap;

pub mod map;

pub trait Interner<'a, T: Hash + Eq> {
    fn intern(&self, value: T, alloc: impl FnOnce(T) -> &'a T) -> Interned<'a, T>;
}

#[derive(Debug, Hash, Eq, PartialEq)]
pub struct Interned<'a, T>(&'a T);

impl<'a, T> Interned<'a, T> {
    pub fn new(value: &'a T) -> Self {
        Self(value)
    }
}

impl<T> Copy for Interned<'_, T> {}
impl<T> Clone for Interned<'_, T> {
    fn clone(&self) -> Self {
        *self
    }
}

impl<T> Deref for Interned<'_, T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        self.0
    }
}

impl<T> Borrow<T> for Interned<'_, T> {
    fn borrow(&self) -> &T {
        self.0
    }
}
