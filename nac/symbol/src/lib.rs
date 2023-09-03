use std::fmt::Debug;

use range::Range;

pub use self::intern::SymbolInterner;

pub mod intern;

#[derive(Debug, Copy, Clone, Eq, PartialEq)]
pub struct Ident {
    pub symbol: Symbol,
    pub range: Range,
}

impl Ident {
    pub fn new(symbol: Symbol, range: Range) -> Self {
        Self { symbol, range }
    }
}

#[derive(Debug, Copy, Clone, Hash, Eq, PartialEq)]
pub struct Symbol(u32);

impl Symbol {
    pub fn new(index: u32) -> Self {
        Self(index)
    }

    pub fn intern(string: &str, interner: &SymbolInterner) -> Self {
        interner.intern(string)
    }

    pub fn string(&self, interner: &SymbolInterner) -> String {
        interner.get_string(self)
    }
}
