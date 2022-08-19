# Sodium Plan

- v0.1.0:
  - implement file io for `nac`
  - implement a lexer for `nac`
  - implement a parser for `nac`
  - interface `nac` front-end with llvm back-end
  - compile examples/main.na

- v0.2.0:
  - implement variables
  - implement primitive integral data types
  - implement type checking

- v0.3.0:
  - implement control flow structures:
    - if statement
    - for loop
    - while loop

- v0.4.0:
  - implement functions
  - implement parameters

### Grammar
---
Terminal symbol names are in all UPPERCASE.
Non-terminal symbol names are in CamelCase.

#### Lexical Structure
The lexical structure describes the sequence of characters that form valid tokens of Sodium.
A token consists of an reserved words, punctuation, identifier or literal.
```
ReservedWord = Keyword | Type ;

Keyword = func | return ;
Type = int ;

Punctuation = -> | { | } | ( | ) ;

Identifier = IdentifierHead IdentifierCharacter* ;
IdentifierHead = "_" | LETTER ;
IdentifierCharacter = IdentifierHead | DIGIT ;

NumericLiteral = DIGIT* ;

LETTER = [a-zA-Z] ;

DIGIT = [0-9] ;

EOL = \n ;
```

#### Syntactic Structure
The syntactic structure describes the sequence of valid tokens that form valid syntax of Sodium.
```
SourceFile = Declaration* EOF ;
```
```
Declaration = FunctionDeclaration ;

FunctionDeclaration = func FunctionSignature Block ;
FunctionSignature = Identifier ParameterList ReturnType ;

ParameterList = () ;

ReturnType = -> Type ;
```
```
Statement = Block | ReturnStatement ;

Block = { Statement* } ;

ReturnStatement = return Expression StatementTerminator ;

StatementTerminator = EOL | } ;
```
```
Expression = LiteralExpression ;

LiteralExpression = NumericLiteralExpression ;

NumericLiteralExpression = NumericLiteral ;
```
