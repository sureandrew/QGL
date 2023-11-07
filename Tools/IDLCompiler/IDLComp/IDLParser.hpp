#ifndef INC_IDLParser_hpp_
#define INC_IDLParser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.4: "IDL.g" -> "IDLParser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "IDLTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

class CUSTOM_API IDLParser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public IDLTokenTypes
{
#line 1 "IDL.g"
#line 15 "IDLParser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	IDLParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	IDLParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return IDLParser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return IDLParser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return IDLParser::tokenNames;
	}
	public:  RefCount<CSpecification>  specification();
	public:  RefCount<CConfigurations>  configurations();
	public:  RefCount<CClass>  classDefinition();
	public:  RefCount<CFunction>  functionDefinition();
	public:  RefCount<CConfiguration>  configuration();
	public:  RefCount<CAttribute>  attrDefinition();
	public:  RefCount<CParameter>  parameterDefinition();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return returnAST;
	}
	
protected:
	ANTLR_USE_NAMESPACE(antlr)RefAST returnAST;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 55;
#else
	enum {
		NUM_TOKENS = 55
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
};

#endif /*INC_IDLParser_hpp_*/
