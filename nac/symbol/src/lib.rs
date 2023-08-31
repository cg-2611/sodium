use std::fmt::{Debug, Display, Formatter};

use range::Range;

use crate::intern::InternedString;

pub use self::intern::SymbolInterner;

pub mod intern;

#[derive(Debug, Copy, Clone, Eq, PartialEq)]
pub struct Ident {
    pub symbol: Symbol,
    pub range: Range,
}

impl Display for Ident {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        Debug::fmt(self, f)
    }
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

    pub fn as_str(self, interner: &SymbolInterner) -> InternedString {
        interner.get(self)
    }
}
