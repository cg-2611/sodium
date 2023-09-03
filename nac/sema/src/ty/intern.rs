use std::borrow::Borrow;
use std::cell::RefCell;
use std::collections::HashSet;
use std::ops::Deref;

use crate::ty::context::TypeArena;
use crate::ty::TypeKind;

#[derive(Debug, Copy, Clone, Hash, Eq, PartialEq)]
pub struct InternedType<'cx>(&'cx TypeKind<'cx>);

impl<'cx> Deref for InternedType<'cx> {
    type Target = TypeKind<'cx>;

    fn deref(&self) -> &Self::Target {
        self.0
    }
}

impl<'cx> Borrow<TypeKind<'cx>> for InternedType<'cx> {
    fn borrow(&self) -> &TypeKind<'cx> {
        self.0
    }
}

pub struct TypeInterner<'cx> {
    type_arena: &'cx TypeArena<'cx>,
    set: RefCell<HashSet<InternedType<'cx>>>,
}

impl<'cx> TypeInterner<'cx> {
    pub fn new(type_arena: &'cx TypeArena<'cx>) -> Self {
        Self {
            type_arena,
            set: RefCell::new(HashSet::new()),
        }
    }

    pub fn intern(&self, kind: TypeKind<'cx>) -> InternedType<'cx> {
        if let Some(&ty) = self.set.borrow().get(&kind) {
            return ty;
        }

        let ty = InternedType(self.type_arena.alloc(kind));
        self.set.borrow_mut().insert(ty);
        ty
    }
}
