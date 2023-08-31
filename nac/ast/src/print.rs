use crate::{
    Block, Decl, DeclKind, Expr, ExprKind, FnDecl, Literal, LiteralKind, RetExpr, SourceFile, Stmt,
    StmtKind, Type, AST,
};
use session::Session;
use symbol::Ident;

pub struct ASTPrinter<'a> {
    sess: &'a Session,
    out: String,
    indent_size: usize,
    indent: usize,
}

impl<'a, 'ast> ASTPrinter<'a> {
    pub fn print_ast(sess: &'a Session, ast: &'ast AST) {
        let mut ast_printer = Self {
            sess,
            out: String::new(),
            indent_size: 4,
            indent: 0,
        };

        ast_printer.print_source_file(ast.root());
        println!("{}", ast_printer.out);
    }

    pub fn print_source_file(&mut self, source_file: &'ast SourceFile) {
        self.writeln(format!("source file ({}):", source_file.range).as_str());
        self.indent();

        for decl in &source_file.decls {
            self.print_decl(decl);
        }

        self.dedent();
    }

    pub fn print_decl(&mut self, decl: &'ast Decl) {
        self.write_indentation();
        self.writeln(format!("decl ({}):", decl.range).as_str());
        self.indent();

        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.print_fn_decl(fn_decl),
        }

        self.dedent();
    }

    pub fn print_fn_decl(&mut self, fn_decl: &'ast FnDecl) {
        self.write_indentation();
        self.writeln(format!("fn decl ({}):", fn_decl.range).as_str());

        self.indent();

        self.print_ident(&fn_decl.ident);
        self.print_type(&fn_decl.ret_type);
        self.print_block(&fn_decl.body);

        self.dedent();
    }

    pub fn print_ident(&mut self, ident: &Ident) {
        self.write_indentation();
        let ident_string = self.sess.symbol_interner().get_string(&ident.symbol);
        self.writeln(format!("ident  ({}): {}", ident.range, ident_string).as_str());
    }

    pub fn print_type(&mut self, ty: &'ast Type) {
        self.write_indentation();
        self.writeln(format!("type ({}):", ty.ident.range).as_str());
        self.indent();

        self.print_ident(&ty.ident);

        self.dedent();
    }

    pub fn print_block(&mut self, block: &'ast Block) {
        self.write_indentation();
        self.writeln(format!("block ({}):", block.range).as_str());
        self.indent();

        for stmt in &block.stmts {
            self.print_stmt(stmt);
        }

        self.dedent();
    }

    pub fn print_stmt(&mut self, stmt: &'ast Stmt) {
        self.write_indentation();
        self.writeln(format!("stmt ({}):", stmt.range).as_str());
        self.indent();

        match &stmt.kind {
            StmtKind::ExprStmt(expr) => self.print_expr(expr),
        }

        self.dedent();
    }

    pub fn print_expr(&mut self, expr: &'ast Expr) {
        self.write_indentation();
        self.writeln(format!("expr ({}):", expr.range).as_str());
        self.indent();

        match &expr.kind {
            ExprKind::Block(block) => self.print_block(block),
            ExprKind::Literal(literal) => self.print_literal(literal),
            ExprKind::Ret(ret_expr) => self.print_ret_expr(ret_expr),
        }

        self.dedent();
    }

    pub fn print_ret_expr(&mut self, ret_expr: &'ast RetExpr) {
        self.write_indentation();
        self.writeln(format!("ret expr ({}):", ret_expr.range).as_str());
        self.indent();

        self.print_expr(&ret_expr.expr);

        self.dedent();
    }

    pub fn print_literal(&mut self, literal: &'ast Literal) {
        self.write_indentation();
        self.writeln(format!("literal ({}):", literal.range).as_str());
        self.indent();

        match &literal.kind {
            LiteralKind::Integer(x) => {
                self.write_indentation();
                self.writeln(format!("integer literal: {}", x).as_str());
            }
        }

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
