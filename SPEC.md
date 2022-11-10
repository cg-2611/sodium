## Grammar
Terminal symbol names are in all UPPERCASE.
Non-terminal symbol names are in CamelCase.

### Lexical Structure
---
The lexical structure describes the sequence of characters that form valid tokens of Sodium.
A token consists of an reserved words, literal, and symbols.
```
ReservedWord = Keyword | Type ;

Keyword = "func" | "return" ;
Type = "int" ;
```
```
Symbol = "->" | "{" | "}" | "(" | ")" | ";" ;
```
```
Identifier = IdentifierHead IdentifierCharacter* ;
IdentifierHead = "_" | LETTER ;
IdentifierCharacter = IdentifierHead | DIGIT ;

IntegerLiteral = DIGIT* ;
```
```
LETTER = [a-zA-Z] ;

DIGIT = [0-9] ;
```

### Syntactic Structure
---
The syntactic structure describes the sequence of valid tokens that form valid syntax of Sodium.
```
SourceFile = Declaration* EOF ;
```
```
Statement = Declaration | Expression ;
```
```
Declaration = FunctionDeclaration ;

FunctionDeclaration = "func" Identifier "(" ")" FunctionReturnType Block ;

FunctionReturnType = "->" Type ;
```
```
Expression = Block | IntegerLiteralExpression | ReturnExpression ";" ;

Block = "{" Statement* "}"

IntegerLiteralExpression = IntegerLiteral ;

ReturnExpression = "return" Expression ;
```
