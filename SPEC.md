## Grammar
Terminal symbol names are in all UPPERCASE.
Non-terminal symbol names are in CamelCase.

### Lexical Structure
The lexical structure describes the sequence of characters that form valid tokens of Sodium.
A token consists of a keyword, literal, or symbol.
```
Keyword ::= "fn" | "ret" ;
```
```
Symbol ::= "->" | "{" | "}" | "(" | ")" ;
```
```
Identifier ::= IdentifierHead IdentifierCharacter* ;
IdentifierHead ::= "_" | LETTER ;
IdentifierCharacter ::= IdentifierHead | DIGIT ;

IntegerLiteral ::= DIGIT* ;
```
```
LETTER ::= [a-zA-Z] ;

DIGIT ::= [0-9] ;
```

### Syntactic Structure
The syntactic structure describes the sequence of valid tokens that form valid syntax of Sodium.
```
SourceFile ::= Declaration* EOF ;
```
```
Statement ::= Declaration | Expression ";" ;
```
```
Declaration ::= FunctionDeclaration ;

FunctionDeclaration ::= "fn" Identifier "(" ")" FunctionReturnType Block ;

FunctionReturnType ::= "->" Type ;

Type ::= Identifier ;
```
```
Expression ::= 
      Block 
    | LiteralExpression
    | Unary Expression
    | BinaryExpression
    | ReturnExpression ;

Block ::= "{" Statement* "}"

LiteralExpression ::= IntegerLiteral ;

UnaryExpression ::= UnaryOperatorExpression ;
UnaryOperatorExpression ::= UnaryAritmeticOperatorExpression ;
UnaryAritmeticOperatorExpression:: = "-" Expression ;

BinaryExpression ::= BinaryOperatorExpression ;
BinaryOperatorExpression ::= BinaryArithmeticOperatorExpression ;
BinaryArithmeticOperatorExpression ::=
      Expression "+" Expression
    | Expression "-" Expression
    | Expression "*" Expression
    | Expression "/" Expression ;

ReturnExpression ::= "ret" Expression ;
```
