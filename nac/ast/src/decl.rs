use range::Range;

use crate::{Block, Identifier, Type};

pub enum DeclKind {
    Fn(Box<FnDecl>),
}

pub struct Decl {
    pub kind: DeclKind,
    pub range: Range,
}

impl Decl {
    pub fn new(kind: DeclKind, range: Range) -> Self {
        Self { kind, range }
    }
}

pub struct FnDecl {
    pub ident: Identifier,
    pub ret_type: Box<Type>,
    pub body: Box<Block>,
    pub range: Range,
}

impl FnDecl {
    pub fn new(ident: Identifier, ret_type: Box<Type>, body: Box<Block>, range: Range) -> Self {
        Self {
            ident,
            ret_type,
            body,
            range,
        }
    }
}