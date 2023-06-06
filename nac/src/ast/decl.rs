use crate::ast::expr::Block;
use crate::ast::ty::Type;
use crate::ast::Identifier;
use crate::source::Range;

pub enum DeclKind {
    Fn(FnDecl),
}

pub struct Decl {
    kind: DeclKind,
    range: Range,
}

impl Decl {
    pub fn new(kind: DeclKind, range: Range) -> Self {
        Self { kind, range }
    }

    pub fn kind(&self) -> &DeclKind {
        &self.kind
    }

    pub fn range(&self) -> Range {
        self.range
    }
}

pub struct FnDecl {
    name: Identifier,
    ret_type: Type,
    body: Block,
}

impl FnDecl {
    pub fn new(name: Identifier, ret_type: Type, body: Block) -> Self {
        Self {
            name,
            ret_type,
            body,
        }
    }

    pub fn name(&self) -> &Identifier {
        &self.name
    }

    pub fn ret_type(&self) -> &Type {
        &self.ret_type
    }

    pub fn body(&self) -> &Block {
        &self.body
    }
}
