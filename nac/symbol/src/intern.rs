use std::borrow::Borrow;
use std::cell::RefCell;
use std::collections::HashMap;
use std::ops::Deref;
use std::rc::Rc;

use crate::Symbol;

#[derive(Clone, Hash, Eq, PartialEq)]
pub struct InternedString(Rc<str>);

impl InternedString {
    pub fn new(string: &str) -> Self {
        Self(Rc::from(string))
    }

    pub fn as_str(&self) -> &str {
        self.0.borrow()
    }
}

impl Borrow<str> for InternedString {
    fn borrow(&self) -> &str {
        &self.0
    }
}

impl Deref for InternedString {
    type Target = str;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

#[derive(Default)]
pub struct SymbolInterner {
    map: RefCell<HashMap<InternedString, Symbol>>,
    strings: RefCell<Vec<InternedString>>,
}

impl SymbolInterner {
    pub fn new() -> Self {
        Self {
            map: RefCell::new(HashMap::new()),
            strings: RefCell::new(Vec::new()),
        }
    }

    pub fn intern(&self, string: &str) -> Symbol {
        if let Some(&symbol) = self.map.borrow().get(string) {
            return symbol;
        }

        let symbol = Symbol::new(self.strings.borrow().len() as u32);
        let string = InternedString::new(string);

        self.map.borrow_mut().insert(string.clone(), symbol);
        self.strings.borrow_mut().push(string);

        symbol
    }

    pub fn get_interned(&self, symbol: &Symbol) -> InternedString {
        (*self.strings.borrow())[symbol.0 as usize].clone()
    }

    pub fn get_string(&self, symbol: &Symbol) -> String {
        self.get_interned(symbol).to_string()
    }
}
