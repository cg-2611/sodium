use range::Range;

use crate::type_check::TypeChecker;
use crate::SemaError;

impl<'cx> TypeChecker<'_, 'cx> {
    pub fn type_error(&self, message: String, range: Range) -> SemaError<'cx> {
        self.tcx.session().create_ranged_error(message, range)
    }
}
