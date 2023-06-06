use crate::ast::stmt::Stmt;
use crate::source::Range;

pub enum ExprKind {
    Block(Block),
    Literal(Literal),
}

pub struct Expr {
    kind: ExprKind,
    range: Range,
}

impl Expr {
    pub fn new(kind: ExprKind, range: Range) -> Self {
        Self { kind, range }
    }

    pub fn kind(&self) -> &ExprKind {
        &self.kind
    }

    pub fn range(&self) -> Range {
        self.range
    }
}

pub struct Block {
    stmts: Vec<Stmt>,
    range: Range,
}

impl Block {
    pub fn new(stmts: Vec<Stmt>, range: Range) -> Self {
        Self { stmts, range }
    }

    pub fn stmts(&self) -> &Vec<Stmt> {
        &self.stmts
    }

    pub fn range(&self) -> Range {
        self.range
    }
}

pub struct RetExpr {
    expr: Expr,
    range: Range,
}

impl RetExpr {
    pub fn new(expr: Expr, range: Range) -> Self {
        Self { expr, range }
    }

    pub fn expr(&self) -> &Expr {
        &self.expr
    }

    pub fn range(&self) -> Range {
        self.range
    }
}

pub enum LiteralKind {
    Integer(i32),
}

pub struct Literal {
    kind: LiteralKind,
    range: Range,
}

impl Literal {
    pub fn new(kind: LiteralKind, range: Range) -> Self {
        Self { kind, range }
    }

    pub fn kind(&self) -> &LiteralKind {
        &self.kind
    }

    pub fn range(&self) -> Range {
        self.range
    }
}
