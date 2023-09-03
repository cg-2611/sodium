use crate::ir::{
    BinaryExpr, BinaryOperator, Block, Decl, DeclKind, Expr, ExprKind, FnDecl, Literal,
    LiteralKind, RetExpr, SourceFile, Stmt, StmtKind, UnaryExpr, UnaryOperator, IR,
};
use session::Session;
use symbol::Ident;

pub struct IRPrinter<'a> {
    sess: &'a Session,
    out: String,
    indent_size: usize,
    indent: usize,
}

impl<'a, 'ir> IRPrinter<'a> {
    pub fn print_ir(sess: &'a Session, ir: &'ir IR) {
        let mut ir_printer = Self {
            sess,
            out: String::default(),
            indent_size: 4,
            indent: 0,
        };

        ir_printer.print_source_file(ir.source_file());
        println!("{}", ir_printer.out);
    }

    pub fn print_source_file(&mut self, source_file: &'ir SourceFile) {
        self.writeln(format!("source file ({}):", source_file.range).as_str());
        self.indent();

        for decl in &source_file.decls {
            self.print_decl(decl);
        }

        self.dedent();
    }

    pub fn print_decl(&mut self, decl: &'ir Decl) {
        self.write_indentation();
        self.writeln(format!("decl <{}> ({}):", decl.ty, decl.range).as_str());
        self.indent();

        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.print_fn_decl(fn_decl),
        }

        self.dedent();
    }

    pub fn print_fn_decl(&mut self, fn_decl: &'ir FnDecl) {
        self.write_indentation();
        self.writeln(format!("fn decl <{}> ({}):", fn_decl.ty, fn_decl.range).as_str());

        self.indent();

        self.print_ident(&fn_decl.ident);
        self.print_block(&fn_decl.body);

        self.dedent();
    }

    pub fn print_ident(&mut self, ident: &Ident) {
        self.write_indentation();
        let ident_string = self.sess.symbol_interner().get_string(&ident.symbol);
        self.writeln(format!("ident ({}): {}", ident.range, ident_string).as_str());
    }

    pub fn print_block(&mut self, block: &'ir Block) {
        self.write_indentation();
        self.writeln(format!("block <{}> ({}):", block.ty, block.range).as_str());
        self.indent();

        for stmt in &block.stmts {
            self.print_stmt(stmt);
        }

        self.dedent();
    }

    pub fn print_stmt(&mut self, stmt: &'ir Stmt) {
        self.write_indentation();
        self.writeln(format!("stmt <{}> ({}):", stmt.ty, stmt.range).as_str());
        self.indent();

        match &stmt.kind {
            StmtKind::ExprStmt(expr) => self.print_expr(expr),
        }

        self.dedent();
    }

    pub fn print_expr(&mut self, expr: &'ir Expr) {
        self.write_indentation();
        self.writeln(format!("expr <{}> ({}):", expr.ty, expr.range).as_str());
        self.indent();

        match &expr.kind {
            ExprKind::Block(block) => self.print_block(block),
            ExprKind::Literal(literal) => self.print_literal(literal),
            ExprKind::Binary(binary_expr) => self.print_binary_expr(binary_expr),
            ExprKind::Unary(unary_expr) => self.print_unary_expr(unary_expr),
            ExprKind::Ret(ret_expr) => self.print_ret_expr(ret_expr),
        }

        self.dedent();
    }

    pub fn print_literal(&mut self, literal: &'ir Literal) {
        self.write_indentation();
        self.writeln(format!("literal <{}> ({}):", literal.ty, literal.range).as_str());
        self.indent();

        match &literal.kind {
            LiteralKind::Integer(x) => {
                self.write_indentation();
                self.writeln(format!("integer literal: {}", x).as_str());
            }
        }

        self.dedent();
    }

    pub fn print_unary_expr(&mut self, unary_expr: &'ir UnaryExpr) {
        self.write_indentation();
        self.writeln(format!("unary expr <{}> ({}):", unary_expr.ty, unary_expr.range).as_str());

        self.indent();

        self.write_indentation();

        // TODO: extract to function
        match unary_expr.operator {
            UnaryOperator::Negate => self.writeln("operator: -"),
        }

        self.print_expr(&unary_expr.expr);

        self.dedent();
    }

    pub fn print_binary_expr(&mut self, binary_expr: &'ir BinaryExpr) {
        self.write_indentation();
        self.writeln(format!("binary expr <{}> ({}):", binary_expr.ty, binary_expr.range).as_str());

        self.indent();

        self.print_expr(&binary_expr.lhs);

        self.write_indentation();

        // TODO: extract to function
        match binary_expr.operator {
            BinaryOperator::Add => self.writeln("operator: +"),
            BinaryOperator::Subtract => self.writeln("operator: -"),
            BinaryOperator::Multiply => self.writeln("operator: *"),
            BinaryOperator::Divide => self.writeln("operator: /"),
        }

        self.print_expr(&binary_expr.rhs);

        self.dedent();
    }

    pub fn print_ret_expr(&mut self, ret_expr: &'ir RetExpr) {
        self.write_indentation();
        self.writeln(format!("ret expr <{}> ({}):", ret_expr.ty, ret_expr.range).as_str());
        self.indent();

        self.print_expr(&ret_expr.expr);

        self.dedent();
    }

    fn write(&mut self, text: &str) {
        self.out.push_str(text);
    }

    fn writeln(&mut self, text: &str) {
        self.write(text);
        self.out.push('\n');
    }

    fn write_indentation(&mut self) {
        self.out
            .push_str(" ".repeat(self.indent * self.indent_size).as_str())
    }

    fn indent(&mut self) {
        self.indent += 1;
    }

    fn dedent(&mut self) {
        self.indent -= 1;
    }
}
