# Sodium Plan

- v0.1.0:
  - implement file io for `nac`
  - implement a lexer for `nac`
  - implement a parser for `nac`
  - implement type checking for `nac`
  - interface `nac` front-end with llvm back-end
  - compile examples/main.na

- v0.2.0:
  - implement variables
  - implement primitive integral data types
  - add comments to sodium

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

Keyword = "func" | "return" ;
Type = "int" ;

Punctuation = "->" | "{" | "}" | "(" | ")" ;

Identifier = IdentifierHead IdentifierCharacter* ;
IdentifierHead = "_" | LETTER ;
IdentifierCharacter = IdentifierHead | DIGIT ;

IntegerLiteral = DIGIT* ;

LETTER = [a-zA-Z] ;

DIGIT = [0-9] ;
```

#### Syntactic Structure
The syntactic structure describes the sequence of valid tokens that form valid syntax of Sodium.
```
SourceFile = Declaration* EOF ;
```
```
Declaration = FunctionDeclaration ;

FunctionDeclaration = FunctionSignature Block ;
FunctionSignature = "func" Identifier ParameterList ReturnType ;

ParameterList = "(" ")" ;

ReturnType = "->" Type ;
```
```
Statement = Block | ReturnStatement ;

Block = "{" Statement* "}" ;

ReturnStatement = "return" Expression ";" ;
```
```
Expression = LiteralExpression ;

LiteralExpression = IntegerLiteralExpression ;

IntegerLiteralExpression = IntegerLiteral ;
```
