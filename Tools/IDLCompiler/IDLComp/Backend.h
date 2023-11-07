// IDL compiler backend 
//
// Copyright 2004 Bae,Hyun-jik.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#pragma once

#include "Specification.h"

#define RPCMANAGER_H		"RPCManager.h"
#define RPCMESSAGE_H		"RPCMessage.h"
#define RPCMANAGER			"RPCManager"
#define RPCMESSAGE			"RPCMessage"

// for checking functions
string GetSubstitutedFileName(string originalFileName, const char* subpath,
							  const char* postfix, const char* extToSubsitute);
string GetFullName(string originalFileName);
string GetBaseName(string originalFileName);
string GetFileDir(string originalFileName);
string GetFileExt(string originalFileName);
bool IsFileExist(string filename);
bool IsDirExist(string dirname);
bool IsFileModified(string filename);
void CreateDir(string filename);
void ChangeDir(string filename);
void ClearArchive(string filename);
string GetUpper(string s);
string CapString(string s);
string LowCapString(string s);
bool CheckImplExist(istream &is, CSpecification* spec, string suffix);
void RandEnumIDs(CSpecification* spec, unsigned int rev);
unsigned int FindExtraSecret(CSpecification* spec, string funcName);

// for writing general enum, proxy & stub functions
void WriteHeaderStart(ostream &os,string prefix);
void WriteNameSpace(ostream &os, CSpecification* spec, string suffix);
void WriteInclude(ostream &os, CSpecification *spec);
void WriteEnumInclude(ostream &os,CSpecification* spec, string enumPath);
void WriteStubInclude(ostream &os,CSpecification* spec, string suffix);
void WriteSubInclude(ostream &os,CSpecification* spec, string suffix);
void WriteClassRegister(ostream &os, CSpecification* spec, string suffix);
void WriteNamespace(ostream &os,CSpecification* spec, string suffix);
void WriteEnumIDs(ostream &os,CSpecification* spec);
void WriteFunctionDecls(ostream &os,CSpecification* spec, bool callback, bool comment=false);
void WriteFunctionDecl(ostream &os,CFunction* func, string prefix);
void WriteParametersWithComma(ostream &os,CFunction* func);
void WriteFunctionProxies(ostream &os,CSpecification* spec, bool callback, string suffix, bool bHeader=false);
void WriteProxyDecl(ostream &os,CSpecification* spec, bool callback);
void WriteStubDecl(ostream &os,CSpecification* spec, bool callback);
void WriteUpdateDecls(ostream &os,CSpecification* spec, CAttribute& attr, bool comment=false);
void WriteProcess(ostream &os,CSpecification* spec, bool callback, string suffix);
void WriteDecode(ostream &os,CSpecification* spec, string suffix);
void WriteMessage(ostream &os,CSpecification* spec, CParameter &param, bool isAttr=false);
void WriteStubImpl(ostream &os,CSpecification* spec, bool callback, string suffix);
void WriteEncryptMacro(ostream &os,CSpecification* spec);
void WriteErrorImpl(ostream &os,CSpecification* spec);
void WriteParametersWithCommaAndWithoutTypes(ostream &os,CFunction* func);
void WriteHeaderEnd(ostream &os, string prefix);

// for writing RPC class
void WriteClassDecl(ostream &os,CSpecification* spec, const string &className, 
					const string &parentName, bool callback, string suffix, bool bSub=false);
void WriteAttrRead(ostream &os,CSpecification* spec, CAttribute &attr, string mask);
void WriteAttrBit(ostream &os,CSpecification* spec, CAttribute &attr, string mask);
void WriteAttrProxy(ostream &os,CSpecification* spec);
void WriteAttrStub(ostream &os,CSpecification* spec);
void WriteSetterImpl(ostream &os,CSpecification* spec, string suffix);
void WriteUpdateImpl(ostream &os,CSpecification* spec, string suffix);
void WriteWriteImpl(ostream &os,CSpecification* spec, string suffix);
void WriteCloneSet(ostream &os,CSpecification* spec, CAttribute &attr, string mask);
void WriteOnUpdate(ostream &os,CSpecification* spec, CAttribute &attr);
void WriteSendReq(ostream &os,CSpecification* spec);

// for writing main process functions
void WriteRPCManagerDecl(ostream &os, string prefix);
void WriteParentInclude(ostream &os, CSpecification* spec, vector<RefCount<CSpecification> > &specList,
						map<string, bool> &parentMap, string outPath);
void WriteProcessInclude(ostream &os, CSpecification* spec, string suffix, string outPath);
void WriteProcessCase(ostream &os, vector<RefCount<CSpecification> > &specList);
void WriteDecodeCase(ostream &os, vector<RefCount<CSpecification> > &specList);
void WriteEnumName(ostream &os, vector<RefCount<CSpecification> > &specList, unsigned int rev);
