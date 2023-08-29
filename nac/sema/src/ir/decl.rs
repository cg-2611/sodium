use range::Range;

use crate::ir::{Block, Identifier};
use crate::ty::Type;

pub enum DeclKind<'cx> {
    Fn(FnDecl<'cx>),
}

pub struct Decl<'cx> {
    pub kind: DeclKind<'cx>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> Decl<'cx> {
    pub fn new(kind: DeclKind<'cx>, ty: Type<'cx>, range: Range) -> Self {
        Self { kind, ty, range }
    }
}

pub struct FnDecl<'cx> {
    pub ident: Identifier,
    pub body: Box<Block<'cx>>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> FnDecl<'cx> {
    pub fn new(ident: Identifier, body: Box<Block<'cx>>, ty: Type<'cx>, range: Range) -> Self {
        Self {
            ident,
            body,
            ty,
            range,
        }
    }
}
