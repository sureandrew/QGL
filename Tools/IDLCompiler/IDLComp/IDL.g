// MyRPC IDL grammar definition
//
// Copyright 2004 Bae,Hyun-jik.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

options {
	language="Cpp";
}

/* A Simple IDL Recognizer authored by Bae,Hyun-jik,imays@hitel.net. */

class IDLParser extends Parser;
options {
	defaultErrorHandler=false;
	genHashLines=true;
	exportVocab=IDL;
}

//********* This is the first plce to parse for IDL file
specification returns [ RefCount<CSpecification> spec ]
	{
		spec=RefCount<CSpecification>(new CSpecification);
		RefCount<CConfigurations> configs;
		RefCount<CClass> clas;
		RefCount<CFunction> func;
	}
	:	configs=configurations { spec->SetConfigs(configs); spec->SetPublic(true); spec->SetBlock(true); }
		(
			prep:PREPROC_DIRECTIVE { spec->AddPreprocess(prep->getText()); }	|
			tdef:TYPEDEF { spec->AddTypeDef(tdef->getText()); }					|
			tstruct:STRUCT { spec->AddStruct(tstruct->getText()); }
		)*
		(	
			clas=classDefinition { spec->SetClass(clas); }
			|
			(
				("public"
				COLON			   { spec->SetPublic(true); }
				|
				"private"
				COLON			   { spec->SetPublic(false); }
				|
				"block"
				COLON			   { spec->SetBlock(true); }
				|
				"nonblock"
				COLON			   { spec->SetBlock(false); }
				)*
				func=functionDefinition { spec->AddFunction(func); }
			)*
		)
		EOF!
	;

// RPC configurations to be applied to every RPC function
configurations returns [ RefCount<CConfigurations> configs ]
	{
		configs=RefCount<CConfigurations>(new CConfigurations);
		RefCount<CConfiguration> config;
	}
	:
	"config"
	LCURLY
	(config=configuration { configs->AddConfig(config); } )*
	RCURLY
	;
	
// configurations defined in IDL files
configuration returns [ RefCount<CConfiguration> config ]
	{
		config=RefCount<CConfiguration>(new CConfiguration);
	}
	:
	configName:IDENT { config->SetName(configName->getText()); }
	ASSIGN
	(configValue:INT { config->SetIntValue(configValue->getText()); } |
	 configString:STRING_LITERAL { config->SetStrValue(configString->getText()); })
	SEMI
	;

// an RPC class definition
classDefinition returns [ RefCount<CClass> clas ]
	{
		clas=RefCount<CClass>(new CClass);
		RefCount<CAttribute> attr;
		RefCount<CFunction> func;
	}
	:
	"class"
	className:IDENT { clas->SetName(className->getText()); }
	(COLON
	parentName:IDENT { clas->SetParent(parentName->getText()); })?
	LCURLY
	(
		(attrDefinition) => (attr=attrDefinition { clas->AddAttribute(attr); }) |
		(
			("public"
			COLON			   { clas->SetPublic(true); }
			|
			"private"
			COLON			   { clas->SetPublic(false); }
			|
			"block"
			COLON			   { clas->SetBlock(true); }
			|
			"nonblock"
			COLON			   { clas->SetBlock(false); }
			)*
			func=functionDefinition { clas->AddMethod(func); }
		)
	)*
	RCURLY
	;
	
// an RPC function definition
functionDefinition returns [ RefCount<CFunction> func ]
	{
		func=RefCount<CFunction>(new CFunction);
		RefCount<CParameter> param;
	}
	:
	functionName:IDENT { func->SetName(functionName->getText()); }
	LPAREN
	(
		param=parameterDefinition { func->AddParameter(param); } 
		( COMMA! param=parameterDefinition { func->AddParameter(param); } )* 
	)?
	RPAREN
	SEMI
	;

// an RPC class attribute definition
attrDefinition returns [ RefCount<CAttribute> attr ]
	{
		attr=RefCount<CAttribute>(new CAttribute);
		RefCount<CParameter> param;
		RefCount<CAttribute> child;
	}
	:
	attrName:IDENT { attr->SetName(attrName->getText()); }
	LCURLY
	(
		(parameterDefinition) => (param=parameterDefinition { attr->AddField(param); } SEMI) |
		(child=attrDefinition { attr->AddAttribute(child); }))*
	RCURLY
	SEMI
	;

// an RPC parameter
parameterDefinition returns [ RefCount<CParameter> param ]
	{
		param=RefCount<CParameter>(new CParameter);
	}
	
	:
	
	(ptype:TYPEP { param->SetType(ptype->getText()); })*
	type:IDENT { param->SetType(type->getText()); }
	(pointer:STAR { param->SetType(pointer->getText()); param->SetPointer(true); }
	 |
	 AND { param->SetReference(true); }) ?
	(LT_
		(
			(INT) =>
			intSt:INT
			COMMA
			intEd:INT {param->SetRange(intSt->getText(), intEd->getText()); }
			|
			floatSt:FLOAT
			COMMA
			floatEd:FLOAT {param->SetRange(floatSt->getText(), floatEd->getText()); }
			(
				COMMA
				digits:INT {param->SetDigits(digits->getText()); }
			)?
		)
	GT)?
	name:IDENT { param->SetName(name->getText()); }
	(LBRACK
		count:INT {param->SetCount(count->getText()); }
	RBRACK)?
	;

/********* This is the end of my simple RPC grammar. 
The other grammar definitions below are original IDL grammar. I let them remain because they
may assist you to add more functionality to your own IDL compiler. */

/* IDL LEXICAL RULES  */
class IDLLexer extends Lexer;
options {
	defaultErrorHandler=false;
	genHashLines=true;
	exportVocab=IDL;
	k=4;
}

SEMI
options {
  paraphrase = ";";
}
	:	';'
	;

QUESTION
options {
  paraphrase = "?";
}
	:	'?'
	;

LPAREN
options {
  paraphrase = "(";
}
	:	'('
	;

RPAREN
options {
  paraphrase = ")";
}
	:	')'
	;

LBRACK
options {
  paraphrase = "[";
}
	:	'['
	;

RBRACK
options {
  paraphrase = "]";
}
	:	']'
	;

LCURLY
options {
  paraphrase = "{";
}
	:	'{'
	;

RCURLY
options {
  paraphrase = "}";
}
	:	'}'
	;

OR
options {
  paraphrase = "|";
}
	:	'|'
	;

XOR
options {
  paraphrase = "^";
}
	:	'^'
	;

AND
options {
  paraphrase = "&";
}
	:	'&'
	;

COLON
options {
  paraphrase = ":";
}
	:	':'
	;

COMMA
options {
  paraphrase = ",";
}
	:	','
	;

DOT
options {
  paraphrase = ".";
}
	:	'.'
	;

ASSIGN
options {
  paraphrase = "=";
}
	:	'='
	;

NOT
options {
  paraphrase = "!";
}
	:	'!'
	;

LT_
options {
  paraphrase = "<";
}
	:	'<'
	;

LSHIFT
options {
  paraphrase = "<<";
}
	: "<<"
	;

GT
options {
  paraphrase = ">";
}
	:	'>'
	;

RSHIFT
options {
  paraphrase = ">>";
}
	: ">>"
	;

DIV
options {
  paraphrase = "/";
}
	:	'/'
	;

PLUS
options {
  paraphrase = "+";
}
	:	'+'
	;

MINUS
options {
  paraphrase = "-";
}
	:	'-'
	;

TILDE
options {
  paraphrase = "~";
}
	:	'~'
	;

STAR
options {
  paraphrase = "*";
}
	:	'*'
	;

MOD
options {
  paraphrase = "%";
}
	:	'%'
	;

SCOPEOP
options {
  paraphrase = "::";
}
	:  	"::"
	;

WS_
options {
  paraphrase = "white space";
}
	:	(' '
	|	'\t'
	|	'\n'  { newline(); }
	|	'\r')
		{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); }
	;

STRUCT
options {
  paraphrase = "a struct directive";
}
	:
	"struct"
	(~'{')*	'{'
	(~'}')* '}'
	';'
	(~'\n')* '\n'
	;

TYPEDEF
options {
  paraphrase = "a typedef directive";
}
	:
	"typedef"
	(~';')* ';'
	(~'\n')* '\n'
	;

PREPROC_DIRECTIVE
options {
  paraphrase = "a preprocessor directive";
}
	:
	'#'
	(~'\n')* '\n'
	/*{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); }*/
	;

SL_COMMENT
options {
  paraphrase = "a comment";
}

	:
	"//"
	(~'\n')* '\n'
	{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP); newline(); }
	;

ML_COMMENT
options {
  paraphrase = "a comment";
}
	:
	"/*"
	(
			STRING_LITERAL
		|	CHAR_LITERAL
		|	'\n' { newline(); }
		|	'*' ~'/'
		|	~'*'
	)*
	"*/"
	{ $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP);  }
	;

CHAR_LITERAL
options {
  paraphrase = "a character literal";
}
	:
	'\''
	( ESC | ~'\'' )
	'\''
	;

STRING_LITERAL
options {
  paraphrase = "a string literal";
}
	:
	'"'
	(ESC|~'"')*
	'"'
	;

protected
ESC
options {
  paraphrase = "an escape sequence";
}
	:	'\\'
		(	'n'
		|	't'
		|	'v'
		|	'b'
		|	'r'
		|	'f'
		|	'a'
		|	'\\'
		|	'?'
		|	'\''
		|	'"'
		|	('0' | '1' | '2' | '3')
			(
				/* Since a digit can occur in a string literal,
				 * which can follow an ESC reference, ANTLR
				 * does not know if you want to match the digit
				 * here (greedy) or in string literal.
				 * The same applies for the next two decisions
				 * with the warnWhenFollowAmbig option.
				 */
				options {
					warnWhenFollowAmbig = false;
				}
			:	OCTDIGIT
				(
					options {
						warnWhenFollowAmbig = false;
					}
				:	OCTDIGIT
				)?
			)?
		|   'x' HEXDIGIT
			(
				options {
					warnWhenFollowAmbig = false;
				}
			:	HEXDIGIT
			)?
		)
	;

protected
VOCAB
options {
  paraphrase = "an escaped character value";
}
	:	'\3'..'\377'
	;

protected
DIGIT
options {
  paraphrase = "a digit";
}
	:	'0'..'9'
	;

protected
OCTDIGIT
options {
  paraphrase = "an octal digit";
}
	:	'0'..'7'
	;

protected
HEXDIGIT
options {
  paraphrase = "a hexadecimal digit";
}
	:	('0'..'9' | 'a'..'f' | 'A'..'F')
	;

/* octal literals are detected by checkOctal */

HEX
options {
  paraphrase = "a hexadecimal value value";
}

	:    ("0x" | "0X") (HEXDIGIT)+
	;

INT
options {
  paraphrase = "an integer value";
}
	:    (DIGIT)+                  // base-10
             (  '.' (DIGIT)*                      	{$setType(FLOAT);}
	         (('e' | 'E') ('+' | '-')? (DIGIT)+)?
	     |   ('e' | 'E') ('+' | '-')? (DIGIT)+   	{$setType(FLOAT);}
             )?
	;

FLOAT
options {
  paraphrase = "an floating point value";
}

	:	'.' (DIGIT)+ (('e' | 'E') ('+' | '-')? (DIGIT)+)?
     	;

TYPEP
options {
  paraphrase = "a type prefix";
}
	:	"unsigned"
	;

IDENT
options {
  testLiterals = true;
  paraphrase = "an identifer";
}

	:	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')*
	;
