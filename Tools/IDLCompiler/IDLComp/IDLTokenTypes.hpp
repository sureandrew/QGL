#ifndef INC_IDLTokenTypes_hpp_
#define INC_IDLTokenTypes_hpp_

/* $ANTLR 2.7.4: "IDL.g" -> "IDLTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API IDLTokenTypes {
#endif
	enum {
		EOF_ = 1,
		PREPROC_DIRECTIVE = 4,
		TYPEDEF = 5,
		STRUCT = 6,
		LITERAL_public = 7,
		COLON = 8,
		LITERAL_private = 9,
		LITERAL_block = 10,
		LITERAL_nonblock = 11,
		LITERAL_config = 12,
		LCURLY = 13,
		RCURLY = 14,
		IDENT = 15,
		ASSIGN = 16,
		INT = 17,
		STRING_LITERAL = 18,
		SEMI = 19,
		LITERAL_class = 20,
		LPAREN = 21,
		COMMA = 22,
		RPAREN = 23,
		TYPEP = 24,
		STAR = 25,
		AND = 26,
		LT_ = 27,
		FLOAT = 28,
		GT = 29,
		LBRACK = 30,
		RBRACK = 31,
		QUESTION = 32,
		OR = 33,
		XOR = 34,
		DOT = 35,
		NOT = 36,
		LSHIFT = 37,
		RSHIFT = 38,
		DIV = 39,
		PLUS = 40,
		MINUS = 41,
		TILDE = 42,
		MOD = 43,
		SCOPEOP = 44,
		WS_ = 45,
		SL_COMMENT = 46,
		ML_COMMENT = 47,
		CHAR_LITERAL = 48,
		ESC = 49,
		VOCAB = 50,
		DIGIT = 51,
		OCTDIGIT = 52,
		HEXDIGIT = 53,
		HEX = 54,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
#endif /*INC_IDLTokenTypes_hpp_*/
