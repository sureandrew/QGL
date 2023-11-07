#ifndef INC_IDLLexer_hpp_
#define INC_IDLLexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.4: "IDL.g" -> "IDLLexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "IDLTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
/********* This is the end of my simple RPC grammar. 
The other grammar definitions below are original IDL grammar. I let them remain because they
may assist you to add more functionality to your own IDL compiler. */
class CUSTOM_API IDLLexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public IDLTokenTypes
{
#line 1 "IDL.g"
#line 18 "IDLLexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	IDLLexer(ANTLR_USE_NAMESPACE(std)istream& in);
	IDLLexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	IDLLexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mSEMI(bool _createToken);
	public: void mQUESTION(bool _createToken);
	public: void mLPAREN(bool _createToken);
	public: void mRPAREN(bool _createToken);
	public: void mLBRACK(bool _createToken);
	public: void mRBRACK(bool _createToken);
	public: void mLCURLY(bool _createToken);
	public: void mRCURLY(bool _createToken);
	public: void mOR(bool _createToken);
	public: void mXOR(bool _createToken);
	public: void mAND(bool _createToken);
	public: void mCOLON(bool _createToken);
	public: void mCOMMA(bool _createToken);
	public: void mDOT(bool _createToken);
	public: void mASSIGN(bool _createToken);
	public: void mNOT(bool _createToken);
	public: void mLT_(bool _createToken);
	public: void mLSHIFT(bool _createToken);
	public: void mGT(bool _createToken);
	public: void mRSHIFT(bool _createToken);
	public: void mDIV(bool _createToken);
	public: void mPLUS(bool _createToken);
	public: void mMINUS(bool _createToken);
	public: void mTILDE(bool _createToken);
	public: void mSTAR(bool _createToken);
	public: void mMOD(bool _createToken);
	public: void mSCOPEOP(bool _createToken);
	public: void mWS_(bool _createToken);
	public: void mSTRUCT(bool _createToken);
	public: void mTYPEDEF(bool _createToken);
	public: void mPREPROC_DIRECTIVE(bool _createToken);
	public: void mSL_COMMENT(bool _createToken);
	public: void mML_COMMENT(bool _createToken);
	public: void mSTRING_LITERAL(bool _createToken);
	public: void mCHAR_LITERAL(bool _createToken);
	protected: void mESC(bool _createToken);
	protected: void mOCTDIGIT(bool _createToken);
	protected: void mHEXDIGIT(bool _createToken);
	protected: void mVOCAB(bool _createToken);
	protected: void mDIGIT(bool _createToken);
	public: void mHEX(bool _createToken);
	public: void mINT(bool _createToken);
	public: void mFLOAT(bool _createToken);
	public: void mTYPEP(bool _createToken);
	public: void mIDENT(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
	static const unsigned long _tokenSet_5_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_5;
	static const unsigned long _tokenSet_6_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_6;
	static const unsigned long _tokenSet_7_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_7;
	static const unsigned long _tokenSet_8_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_8;
	static const unsigned long _tokenSet_9_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_9;
};

#endif /*INC_IDLLexer_hpp_*/
