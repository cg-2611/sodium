use std::ops::Deref;

use arena::TypedArena;
use session::Session;

use crate::ty::intern::TypeInterner;
use crate::ty::{Type, TypeKind};

#[derive(Default)]
pub struct TypeArena<'cx>(TypedArena<TypeKind<'cx>>);

impl<'cx> TypeArena<'cx> {
    pub fn new() -> Self {
        Self(TypedArena::new())
    }
}

impl<'cx> Deref for TypeArena<'cx> {
    type Target = TypedArena<TypeKind<'cx>>;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

pub struct PrimitiveTypes<'cx> {
    pub i32: Type<'cx>,
}

impl<'cx> PrimitiveTypes<'cx> {
    pub fn new(interner: &TypeInterner<'cx>) -> Self {
        Self {
            i32: Type::new(interner.intern(TypeKind::I32)),
        }
    }
}

#[derive(Copy, Clone)]
pub struct TypeContext<'a, 'cx: 'a> {
    cx: &'a Context<'cx>,
}

impl<'a, 'cx> TypeContext<'a, 'cx> {
    pub fn new(cx: &'a Context<'cx>) -> Self {
        Self { cx }
    }

    pub fn create_and_enter<F, R>(sess: &'cx Session, type_arena: &'cx TypeArena<'cx>, f: F) -> R
    where
        for<'b> F: FnOnce(TypeContext<'b, 'cx>) -> R,
    {
        let cx = Context::new(sess, type_arena);
        cx.enter(f)
    }
}

impl<'a, 'cx> Deref for TypeContext<'a, 'cx> {
    type Target = &'a Context<'cx>;

    fn deref(&self) -> &Self::Target {
        &self.cx
    }
}

pub struct Context<'cx> {
    sess: &'cx Session,
    type_arena: &'cx TypeArena<'cx>,
    type_interner: TypeInterner<'cx>,
    types: PrimitiveTypes<'cx>,
}

impl<'cx> Context<'cx> {
    pub fn new(sess: &'cx Session, type_arena: &'cx TypeArena<'cx>) -> Self {
        let type_interner = TypeInterner::new(type_arena);
        let types = PrimitiveTypes::new(&type_interner);

        Self {
            sess,
            type_arena,
            type_interner,
            types,
        }
    }

    pub fn enter<F, R>(&self, f: F) -> R
    where
        F: for<'a> FnOnce(TypeContext<'a, 'cx>) -> R,
    {
        f(TypeContext::new(self))
    }

    pub fn session(&self) -> &'cx Session {
        self.sess
    }

    pub fn type_arena(&self) -> &'cx TypeArena<'cx> {
        self.type_arena
    }

    pub fn i32(&self) -> Type<'cx> {
        self.types.i32
    }

    pub fn fn_type(&self, ty: Type<'cx>) -> Type<'cx> {
        Type::new(self.type_interner.intern(TypeKind::Fn(ty)))
    }
}
