use symbol::Ident;

pub struct Type {
    pub ident: Ident,
}

impl Type {
    pub fn new(ident: Ident) -> Self {
        Self { ident }
    }
}
