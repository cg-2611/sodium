use range::Range;
use symbol::Ident;

use crate::ir::Block;
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
    pub ident: Ident,
    pub body: Box<Block<'cx>>,
    pub ty: Type<'cx>,
    pub range: Range,
}

impl<'cx> FnDecl<'cx> {
    pub fn new(ident: Ident, body: Box<Block<'cx>>, ty: Type<'cx>, range: Range) -> Self {
        Self {
            ident,
            body,
            ty,
            range,
        }
    }
}
