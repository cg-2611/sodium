use crate::ty::context::TypeContext;
use crate::ty::Type;
use crate::SemaResult;
use std::ops::Deref;

pub struct TypeInference;

impl TypeInference {
    pub fn ast_type_to_type<'cx>(
        tcx: TypeContext<'_, 'cx>,
        ast_type: &ast::Type,
    ) -> SemaResult<'cx, Type<'cx>> {
        match ast_type
            .ident
            .symbol
            .as_str(tcx.session().symbol_interner())
            .deref()
        {
            "i32" => Ok(Type::i32(tcx)),
            _ => {
                let message = format!("unknown type `{}`", ast_type.ident);
                Err(tcx
                    .session()
                    .create_ranged_error(message, ast_type.ident.range))
            }
        }
    }
}
