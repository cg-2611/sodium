use crate::Identifier;

pub struct Type {
    pub ident: Identifier,
}

impl Type {
    pub fn new(ident: Identifier) -> Self {
        Self { ident }
    }
}
