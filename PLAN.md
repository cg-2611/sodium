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


SodiumFile = Declaration EOF ;


Declaration = FunctionDeclaration ;


FunctionDeclaration = **func** FunctionName FunctionParameters FunctionSignature FunctionBody ;

FunctionName = IDENTIFIER ;

FunctionParameters = **(** **)** ;

FunctionSignature = **->** TYPE ;

FunctionBody = Block ;


Block = Block = **{** Statement* **}** ;


Statement = ReturnStatement  EOL;

ReturnStatement = **return** Expression ;


Expression = LiteralExpression ;

LiteralExpression = NUMERIC_LITERAL ;


TYPE = IDENTIFIER


IDENTIFIER = IDENTIFIER_HEAD IDENTIFIER_CHARACTER* ;

IDENTIFIER_HEAD = "_" | LETTER ;

IDENTIFIER_CHARACTER = IDENTIFIER_HEAD | DIGIT ;


NUMERIC_LITERAL = DIGIT* ;


LETTER = [a-zA-Z] ;

DIGIT = [0-9] ;
