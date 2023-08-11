use range::Range;
use ty::Type;

use crate::{Block, Identifier};

pub enum DeclKind {
    Fn(Box<FnDecl>),
}

pub struct Decl {
    pub kind: DeclKind,
    pub ty: Type,
    pub range: Range,
}

impl Decl {
    pub fn new(kind: DeclKind, ty: Type, range: Range) -> Self {
        Self { kind, ty, range }
    }
}

pub struct FnDecl {
    pub ident: Identifier,
    pub body: Box<Block>,
    pub ty: Type,
    pub range: Range,
}

impl FnDecl {
    pub fn new(ident: Identifier, body: Box<Block>, ty: Type, range: Range) -> Self {
        Self {
            ident,
            body,
            ty,
            range,
        }
    }
}
