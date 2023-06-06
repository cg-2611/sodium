use crate::ast::Identifier;

pub struct Type {
    value: Identifier,
}

impl Type {
    pub fn new(value: Identifier) -> Self {
        Self { value }
    }

    pub fn value(&self) -> &Identifier {
        &self.value
    }
}
