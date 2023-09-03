use crate::ty::context::TypeContext;
use crate::ty::Type;
use crate::SemaResult;

pub struct TypeInference;

impl TypeInference {
    pub fn ast_type_to_type<'cx>(
        tcx: TypeContext<'_, 'cx>,
        ast_type: &ast::Type,
    ) -> SemaResult<'cx, Type<'cx>> {
        let type_string = tcx
            .session()
            .symbol_interner()
            .get_string(&ast_type.ident.symbol);

        match type_string.as_str() {
            "i32" => Ok(Type::i32(tcx)),
            _ => {
                let message = format!("unknown type `{}`", type_string);
                Err(tcx
                    .session()
                    .create_ranged_error(message, ast_type.ident.range))
            }
        }
    }
}
