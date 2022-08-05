# Sodium Plan

- v0.1.0:
  - implement file io for `nac`
  - implement a lexer for `nac`

- v0.2.0
  - implement a parser for `nac`
  - interface `nac` front-end with llvm back-end
  - compile examples/main.na

- v0.3.0:
  - implement variables
  - implement primitive integral data types
  - implement type checking

- v0.4.0:
  - implement control flow structures:
    - if statement
    - for loop
    - while loop

- v0.5.0:
  - implement functions
  - implement parameters

### Grammar
---

Terminal symbols names are in all UPPERCASE.
Non-terminal symbols are in CamelCase.

SodiumFile = Declaration* EOF ;

Declaration = FunctionDeclaration ;

FunctionDeclaration = **func** FunctionName Parameters FunctionSignature FunctionBody ;

FunctionName = Identifier ;

Parameters = **(** **)** ;

FunctionSignature = **->** Type ;

FunctionBody = Block ;

Block = **{** Statement* **}** ;

Statement = ControlTransferStatement EOL;

ControlTransferStatement = ReturnStatement ;

ReturnStatement = **return** Expression ;

Expression = LiteralExpression ;

Type = IDENTIFIER ;

IDENTIFIER = IdentifierHead IdentifierCharacter* ;

IdentifierHead = "_" | Letter ;

IdentifierCharacter = IdentifierHead | Digit ;

LiteralExpression = NUMERIC_LITERAL ;

NUMERIC_LITERAL = Digit+ ;

Letter =
    "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "x" | "t" | "u" | "v" | "w" | "x" | "y" | "z" | "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "X" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" ;

Digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" |" "7| "8" | "9" ;

