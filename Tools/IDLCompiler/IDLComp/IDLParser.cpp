/* $ANTLR 2.7.4: "IDL.g" -> "IDLParser.cpp"$ */
#include "IDLParser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
#line 1 "IDL.g"
#line 8 "IDLParser.cpp"
IDLParser::IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

IDLParser::IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,1)
{
}

IDLParser::IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

IDLParser::IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,1)
{
}

IDLParser::IDLParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,1)
{
}

 RefCount<CSpecification>  IDLParser::specification() {
#line 22 "IDL.g"
	 RefCount<CSpecification> spec ;
#line 37 "IDLParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  prep = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  tdef = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  tstruct = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 22 "IDL.g"
	
			spec=RefCount<CSpecification>(new CSpecification);
			RefCount<CConfigurations> configs;
			RefCount<CClass> clas;
			RefCount<CFunction> func;
		
#line 48 "IDLParser.cpp"
	
	configs=configurations();
	if ( inputState->guessing==0 ) {
#line 29 "IDL.g"
		spec->SetConfigs(configs); spec->SetPublic(true); spec->SetBlock(true);
#line 54 "IDLParser.cpp"
	}
	{ // ( ... )*
	for (;;) {
		switch ( LA(1)) {
		case PREPROC_DIRECTIVE:
		{
			prep = LT(1);
			match(PREPROC_DIRECTIVE);
			if ( inputState->guessing==0 ) {
#line 31 "IDL.g"
				spec->AddPreprocess(prep->getText());
#line 66 "IDLParser.cpp"
			}
			break;
		}
		case TYPEDEF:
		{
			tdef = LT(1);
			match(TYPEDEF);
			if ( inputState->guessing==0 ) {
#line 32 "IDL.g"
				spec->AddTypeDef(tdef->getText());
#line 77 "IDLParser.cpp"
			}
			break;
		}
		case STRUCT:
		{
			tstruct = LT(1);
			match(STRUCT);
			if ( inputState->guessing==0 ) {
#line 33 "IDL.g"
				spec->AddStruct(tstruct->getText());
#line 88 "IDLParser.cpp"
			}
			break;
		}
		default:
		{
			goto _loop3;
		}
		}
	}
	_loop3:;
	} // ( ... )*
	{
	switch ( LA(1)) {
	case LITERAL_class:
	{
		clas=classDefinition();
		if ( inputState->guessing==0 ) {
#line 36 "IDL.g"
			spec->SetClass(clas);
#line 108 "IDLParser.cpp"
		}
		break;
	}
	case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
	case LITERAL_public:
	case LITERAL_private:
	case LITERAL_block:
	case LITERAL_nonblock:
	case IDENT:
	{
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				{ // ( ... )*
				for (;;) {
					switch ( LA(1)) {
					case LITERAL_public:
					{
						match(LITERAL_public);
						match(COLON);
						if ( inputState->guessing==0 ) {
#line 40 "IDL.g"
							spec->SetPublic(true);
#line 132 "IDLParser.cpp"
						}
						break;
					}
					case LITERAL_private:
					{
						match(LITERAL_private);
						match(COLON);
						if ( inputState->guessing==0 ) {
#line 43 "IDL.g"
							spec->SetPublic(false);
#line 143 "IDLParser.cpp"
						}
						break;
					}
					case LITERAL_block:
					{
						match(LITERAL_block);
						match(COLON);
						if ( inputState->guessing==0 ) {
#line 46 "IDL.g"
							spec->SetBlock(true);
#line 154 "IDLParser.cpp"
						}
						break;
					}
					case LITERAL_nonblock:
					{
						match(LITERAL_nonblock);
						match(COLON);
						if ( inputState->guessing==0 ) {
#line 49 "IDL.g"
							spec->SetBlock(false);
#line 165 "IDLParser.cpp"
						}
						break;
					}
					default:
					{
						goto _loop7;
					}
					}
				}
				_loop7:;
				} // ( ... )*
				func=functionDefinition();
				if ( inputState->guessing==0 ) {
#line 51 "IDL.g"
					spec->AddFunction(func);
#line 181 "IDLParser.cpp"
				}
			}
			else {
				goto _loop8;
			}
			
		}
		_loop8:;
		} // ( ... )*
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
	return spec ;
}

 RefCount<CConfigurations>  IDLParser::configurations() {
#line 58 "IDL.g"
	 RefCount<CConfigurations> configs ;
#line 206 "IDLParser.cpp"
#line 58 "IDL.g"
	
			configs=RefCount<CConfigurations>(new CConfigurations);
			RefCount<CConfiguration> config;
		
#line 212 "IDLParser.cpp"
	
	match(LITERAL_config);
	match(LCURLY);
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == IDENT)) {
			config=configuration();
			if ( inputState->guessing==0 ) {
#line 66 "IDL.g"
				configs->AddConfig(config);
#line 223 "IDLParser.cpp"
			}
		}
		else {
			goto _loop11;
		}
		
	}
	_loop11:;
	} // ( ... )*
	match(RCURLY);
	return configs ;
}

 RefCount<CClass>  IDLParser::classDefinition() {
#line 84 "IDL.g"
	 RefCount<CClass> clas ;
#line 240 "IDLParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  className = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  parentName = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 84 "IDL.g"
	
			clas=RefCount<CClass>(new CClass);
			RefCount<CAttribute> attr;
			RefCount<CFunction> func;
		
#line 249 "IDLParser.cpp"
	
	match(LITERAL_class);
	className = LT(1);
	match(IDENT);
	if ( inputState->guessing==0 ) {
#line 92 "IDL.g"
		clas->SetName(className->getText());
#line 257 "IDLParser.cpp"
	}
	{
	switch ( LA(1)) {
	case COLON:
	{
		match(COLON);
		parentName = LT(1);
		match(IDENT);
		if ( inputState->guessing==0 ) {
#line 94 "IDL.g"
			clas->SetParent(parentName->getText());
#line 269 "IDLParser.cpp"
		}
		break;
	}
	case LCURLY:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(LCURLY);
	{ // ( ... )*
	for (;;) {
		bool synPredMatched18 = false;
		if (((LA(1) == IDENT))) {
			int _m18 = mark();
			synPredMatched18 = true;
			inputState->guessing++;
			try {
				{
				attrDefinition();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& /*pe*/) {
				synPredMatched18 = false;
			}
			rewind(_m18);
			inputState->guessing--;
		}
		if ( synPredMatched18 ) {
			{
			attr=attrDefinition();
			if ( inputState->guessing==0 ) {
#line 97 "IDL.g"
				clas->AddAttribute(attr);
#line 308 "IDLParser.cpp"
			}
			}
		}
		else if ((_tokenSet_0.member(LA(1)))) {
			{
			{ // ( ... )*
			for (;;) {
				switch ( LA(1)) {
				case LITERAL_public:
				{
					match(LITERAL_public);
					match(COLON);
					if ( inputState->guessing==0 ) {
#line 100 "IDL.g"
						clas->SetPublic(true);
#line 324 "IDLParser.cpp"
					}
					break;
				}
				case LITERAL_private:
				{
					match(LITERAL_private);
					match(COLON);
					if ( inputState->guessing==0 ) {
#line 103 "IDL.g"
						clas->SetPublic(false);
#line 335 "IDLParser.cpp"
					}
					break;
				}
				case LITERAL_block:
				{
					match(LITERAL_block);
					match(COLON);
					if ( inputState->guessing==0 ) {
#line 106 "IDL.g"
						clas->SetBlock(true);
#line 346 "IDLParser.cpp"
					}
					break;
				}
				case LITERAL_nonblock:
				{
					match(LITERAL_nonblock);
					match(COLON);
					if ( inputState->guessing==0 ) {
#line 109 "IDL.g"
						clas->SetBlock(false);
#line 357 "IDLParser.cpp"
					}
					break;
				}
				default:
				{
					goto _loop22;
				}
				}
			}
			_loop22:;
			} // ( ... )*
			func=functionDefinition();
			if ( inputState->guessing==0 ) {
#line 111 "IDL.g"
				clas->AddMethod(func);
#line 373 "IDLParser.cpp"
			}
			}
		}
		else {
			goto _loop23;
		}
		
	}
	_loop23:;
	} // ( ... )*
	match(RCURLY);
	return clas ;
}

 RefCount<CFunction>  IDLParser::functionDefinition() {
#line 118 "IDL.g"
	 RefCount<CFunction> func ;
#line 391 "IDLParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  functionName = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 118 "IDL.g"
	
			func=RefCount<CFunction>(new CFunction);
			RefCount<CParameter> param;
		
#line 398 "IDLParser.cpp"
	
	functionName = LT(1);
	match(IDENT);
	if ( inputState->guessing==0 ) {
#line 124 "IDL.g"
		func->SetName(functionName->getText());
#line 405 "IDLParser.cpp"
	}
	match(LPAREN);
	{
	switch ( LA(1)) {
	case IDENT:
	case TYPEP:
	{
		param=parameterDefinition();
		if ( inputState->guessing==0 ) {
#line 127 "IDL.g"
			func->AddParameter(param);
#line 417 "IDLParser.cpp"
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				param=parameterDefinition();
				if ( inputState->guessing==0 ) {
#line 128 "IDL.g"
					func->AddParameter(param);
#line 427 "IDLParser.cpp"
				}
			}
			else {
				goto _loop27;
			}
			
		}
		_loop27:;
		} // ( ... )*
		break;
	}
	case RPAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(RPAREN);
	match(SEMI);
	return func ;
}

 RefCount<CConfiguration>  IDLParser::configuration() {
#line 71 "IDL.g"
	 RefCount<CConfiguration> config ;
#line 457 "IDLParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  configName = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  configValue = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  configString = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 71 "IDL.g"
	
			config=RefCount<CConfiguration>(new CConfiguration);
		
#line 465 "IDLParser.cpp"
	
	configName = LT(1);
	match(IDENT);
	if ( inputState->guessing==0 ) {
#line 76 "IDL.g"
		config->SetName(configName->getText());
#line 472 "IDLParser.cpp"
	}
	match(ASSIGN);
	{
	switch ( LA(1)) {
	case INT:
	{
		configValue = LT(1);
		match(INT);
		if ( inputState->guessing==0 ) {
#line 78 "IDL.g"
			config->SetIntValue(configValue->getText());
#line 484 "IDLParser.cpp"
		}
		break;
	}
	case STRING_LITERAL:
	{
		configString = LT(1);
		match(STRING_LITERAL);
		if ( inputState->guessing==0 ) {
#line 79 "IDL.g"
			config->SetStrValue(configString->getText());
#line 495 "IDLParser.cpp"
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	match(SEMI);
	return config ;
}

 RefCount<CAttribute>  IDLParser::attrDefinition() {
#line 135 "IDL.g"
	 RefCount<CAttribute> attr ;
#line 512 "IDLParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  attrName = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 135 "IDL.g"
	
			attr=RefCount<CAttribute>(new CAttribute);
			RefCount<CParameter> param;
			RefCount<CAttribute> child;
		
#line 520 "IDLParser.cpp"
	
	attrName = LT(1);
	match(IDENT);
	if ( inputState->guessing==0 ) {
#line 142 "IDL.g"
		attr->SetName(attrName->getText());
#line 527 "IDLParser.cpp"
	}
	match(LCURLY);
	{ // ( ... )*
	for (;;) {
		bool synPredMatched31 = false;
		if (((LA(1) == IDENT || LA(1) == TYPEP))) {
			int _m31 = mark();
			synPredMatched31 = true;
			inputState->guessing++;
			try {
				{
				parameterDefinition();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& /*pe*/) {
				synPredMatched31 = false;
			}
			rewind(_m31);
			inputState->guessing--;
		}
		if ( synPredMatched31 ) {
			{
			param=parameterDefinition();
			if ( inputState->guessing==0 ) {
#line 145 "IDL.g"
				attr->AddField(param);
#line 554 "IDLParser.cpp"
			}
			match(SEMI);
			}
		}
		else if ((LA(1) == IDENT)) {
			{
			child=attrDefinition();
			if ( inputState->guessing==0 ) {
#line 146 "IDL.g"
				attr->AddAttribute(child);
#line 565 "IDLParser.cpp"
			}
			}
		}
		else {
			goto _loop34;
		}
		
	}
	_loop34:;
	} // ( ... )*
	match(RCURLY);
	match(SEMI);
	return attr ;
}

 RefCount<CParameter>  IDLParser::parameterDefinition() {
#line 152 "IDL.g"
	 RefCount<CParameter> param ;
#line 584 "IDLParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  ptype = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  type = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  pointer = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  intSt = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  intEd = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  floatSt = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  floatEd = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  digits = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  name = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  count = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 152 "IDL.g"
	
			param=RefCount<CParameter>(new CParameter);
		
#line 599 "IDLParser.cpp"
	
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == TYPEP)) {
			ptype = LT(1);
			match(TYPEP);
			if ( inputState->guessing==0 ) {
#line 159 "IDL.g"
				param->SetType(ptype->getText());
#line 609 "IDLParser.cpp"
			}
		}
		else {
			goto _loop37;
		}
		
	}
	_loop37:;
	} // ( ... )*
	type = LT(1);
	match(IDENT);
	if ( inputState->guessing==0 ) {
#line 160 "IDL.g"
		param->SetType(type->getText());
#line 624 "IDLParser.cpp"
	}
	{
	switch ( LA(1)) {
	case STAR:
	{
		pointer = LT(1);
		match(STAR);
		if ( inputState->guessing==0 ) {
#line 161 "IDL.g"
			param->SetType(pointer->getText()); param->SetPointer(true);
#line 635 "IDLParser.cpp"
		}
		break;
	}
	case AND:
	{
		match(AND);
		if ( inputState->guessing==0 ) {
#line 163 "IDL.g"
			param->SetReference(true);
#line 645 "IDLParser.cpp"
		}
		break;
	}
	case IDENT:
	case LT_:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	{
	switch ( LA(1)) {
	case LT_:
	{
		match(LT_);
		{
		switch ( LA(1)) {
		case INT:
		{
			intSt = LT(1);
			match(INT);
			match(COMMA);
			intEd = LT(1);
			match(INT);
			if ( inputState->guessing==0 ) {
#line 169 "IDL.g"
				param->SetRange(intSt->getText(), intEd->getText());
#line 677 "IDLParser.cpp"
			}
			break;
		}
		case FLOAT:
		{
			floatSt = LT(1);
			match(FLOAT);
			match(COMMA);
			floatEd = LT(1);
			match(FLOAT);
			if ( inputState->guessing==0 ) {
#line 173 "IDL.g"
				param->SetRange(floatSt->getText(), floatEd->getText());
#line 691 "IDLParser.cpp"
			}
			{
			switch ( LA(1)) {
			case COMMA:
			{
				match(COMMA);
				digits = LT(1);
				match(INT);
				if ( inputState->guessing==0 ) {
#line 176 "IDL.g"
					param->SetDigits(digits->getText());
#line 703 "IDLParser.cpp"
				}
				break;
			}
			case GT:
			{
				break;
			}
			default:
			{
				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
			}
			}
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(GT);
		break;
	}
	case IDENT:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	name = LT(1);
	match(IDENT);
	if ( inputState->guessing==0 ) {
#line 180 "IDL.g"
		param->SetName(name->getText());
#line 743 "IDLParser.cpp"
	}
	{
	switch ( LA(1)) {
	case LBRACK:
	{
		match(LBRACK);
		count = LT(1);
		match(INT);
		if ( inputState->guessing==0 ) {
#line 182 "IDL.g"
			param->SetCount(count->getText());
#line 755 "IDLParser.cpp"
		}
		match(RBRACK);
		break;
	}
	case SEMI:
	case COMMA:
	case RPAREN:
	{
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	}
	return param ;
}

void IDLParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& )
{
}
const char* IDLParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"a preprocessor directive",
	"a typedef directive",
	"a struct directive",
	"\"public\"",
	":",
	"\"private\"",
	"\"block\"",
	"\"nonblock\"",
	"\"config\"",
	"{",
	"}",
	"an identifer",
	"=",
	"an integer value",
	"a string literal",
	";",
	"\"class\"",
	"(",
	",",
	")",
	"a type prefix",
	"*",
	"&",
	"<",
	"an floating point value",
	">",
	"[",
	"]",
	"?",
	"|",
	"^",
	".",
	"!",
	"<<",
	">>",
	"/",
	"+",
	"-",
	"~",
	"%",
	"::",
	"white space",
	"a comment",
	"a comment",
	"a character literal",
	"an escape sequence",
	"an escaped character value",
	"a digit",
	"an octal digit",
	"a hexadecimal digit",
	"a hexadecimal value value",
	0
};

const unsigned long IDLParser::_tokenSet_0_data_[] = { 36480UL, 0UL, 0UL, 0UL };
// "public" "private" "block" "nonblock" IDENT 
const ANTLR_USE_NAMESPACE(antlr)BitSet IDLParser::_tokenSet_0(_tokenSet_0_data_,4);


