use crate::ast::decl::{Decl, DeclKind, FnDecl};
use crate::ast::expr::{Block, Expr, ExprKind, Literal, LiteralKind, RetExpr};
use crate::ast::stmt::{Stmt, StmtKind};
use crate::ast::ty::Type;
use crate::ast::visitor::MutVisitor;
use crate::ast::{Identifier, SourceFile, AST};

pub struct ASTPrinter {
    out: String,
    indent_size: usize,
    indent: usize,
}

impl<'ast> MutVisitor<'ast> for ASTPrinter {
    fn visit_source_file(&mut self, source_file: &'ast SourceFile) {
        self.walk_source_file(source_file);
    }

    fn visit_decl(&mut self, decl: &'ast Decl) {
        self.walk_decl(decl);
    }

    fn visit_fn_decl(&mut self, fn_decl: &'ast FnDecl) {
        self.walk_fn_decl(fn_decl);
    }

    fn visit_ident(&mut self, ident: &'ast Identifier) {
        self.walk_ident(ident);
    }

    fn visit_type(&mut self, ty: &'ast Type) {
        self.walk_type(ty);
    }

    fn visit_block(&mut self, block: &'ast Block) {
        self.walk_block(block);
    }

    fn visit_stmt(&mut self, stmt: &'ast Stmt) {
        self.walk_stmt(stmt);
    }

    fn visit_expr(&mut self, expr: &'ast Expr) {
        self.walk_expr(expr);
    }

    fn visit_ret_expr(&mut self, ret_expr: &'ast RetExpr) {
        self.walk_ret_expr(ret_expr);
    }

    fn visit_literal(&mut self, literal: &'ast Literal) {
        self.walk_literal(literal);
    }
}

impl<'ast> ASTPrinter {
    pub fn print_ast(ast: &'ast AST) {
        let mut ast_printer = Self {
            out: String::default(),
            indent_size: 4,
            indent: 0,
        };

        ast_printer.visit_source_file(ast.root());
        println!("{}", ast_printer.out);
    }

    pub fn walk_source_file(&mut self, source_file: &'ast SourceFile) {
        self.writeln(format!("source file ({}):", source_file.range).as_str());
        self.indent();

        for decl in &source_file.decls {
            self.visit_decl(decl);
        }

        self.dedent();
    }

    pub fn walk_decl(&mut self, decl: &'ast Decl) {
        self.write_indentation();
        self.writeln(format!("decl ({}):", decl.range).as_str());
        self.indent();

        match &decl.kind {
            DeclKind::Fn(fn_decl) => self.visit_fn_decl(fn_decl),
        }

        self.dedent();
    }

    pub fn walk_fn_decl(&mut self, fn_decl: &'ast FnDecl) {
        self.write_indentation();
        self.writeln(format!("fn decl ({}):", fn_decl.range).as_str());

        self.indent();

        self.visit_ident(&fn_decl.ident);
        self.visit_type(&fn_decl.ret_type);
        self.visit_block(&fn_decl.body);

        self.dedent();
    }

    pub fn walk_ident(&mut self, ident: &'ast Identifier) {
        self.write_indentation();
        self.writeln(format!("ident: {} ({})", ident.value, ident.range).as_str());
    }

    pub fn walk_type(&mut self, ty: &'ast Type) {
        self.write_indentation();
        self.writeln(format!("type ({}):", ty.ident.range).as_str());
        self.indent();

        self.visit_ident(&ty.ident);

        self.dedent();
    }

    pub fn walk_block(&mut self, block: &'ast Block) {
        self.write_indentation();
        self.writeln(format!("block: ({})", block.range).as_str());
        self.indent();

        for stmt in &block.stmts {
            self.visit_stmt(stmt);
        }

        self.dedent();
    }

    pub fn walk_stmt(&mut self, stmt: &'ast Stmt) {
        self.write_indentation();
        self.writeln(format!("stmt: ({})", stmt.range).as_str());
        self.indent();

        match &stmt.kind {
            StmtKind::ExprStmt(expr) => self.visit_expr(expr),
        }

        self.dedent();
    }

    pub fn walk_expr(&mut self, expr: &'ast Expr) {
        self.write_indentation();
        self.writeln(format!("expr: ({})", expr.range).as_str());
        self.indent();

        match &expr.kind {
            ExprKind::Block(block) => self.visit_block(block),
            ExprKind::Literal(literal) => self.visit_literal(literal),
            ExprKind::Ret(ret_expr) => self.visit_ret_expr(ret_expr),
        }

        self.dedent();
    }

    pub fn walk_ret_expr(&mut self, ret_expr: &'ast RetExpr) {
        self.write_indentation();
        self.writeln(format!("ret expr ({}):", ret_expr.range).as_str());
        self.indent();

        self.visit_expr(&ret_expr.expr);

        self.dedent();
    }

    pub fn walk_literal(&mut self, literal: &'ast Literal) {
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
