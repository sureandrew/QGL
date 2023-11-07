// IDL compiler backend
//
// Copyright 2004 Bae,Hyun-jik.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#include "StdAfx.h"
#include <windows.h>
#include "backend.h"
#include "FileTemplate.h"

///////////////////////////////////////////////////////////
//--------------- Checking IDL functions ------------------
///////////////////////////////////////////////////////////

extern FileTemplate *pCurTpl;

string GetSubstitutedFileName(string originalFileName, const char* subpath,
							  const char* postfix, const char* extToSubsitute)
{
	char fullPath[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_fullpath(fullPath,originalFileName.c_str(),_MAX_PATH);
	_splitpath(fullPath,drive,dir,fname,ext);
	if (subpath[0] != '\0') {
		strcat(dir, "/");
		strcat(dir, subpath);
	}
	strcat(fname, postfix);
	strcpy(ext,extToSubsitute);

	_makepath(fullPath,drive,dir,fname,ext);

	return string(fullPath);
}

// get file full path name
string GetFullName(string originalFileName)
{
	char fullPath[_MAX_PATH];

	_fullpath(fullPath,originalFileName.c_str(),_MAX_PATH);

	return string(fullPath);
}

// get file base name
string GetBaseName(string originalFileName)
{
	char fullPath[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_fullpath(fullPath,originalFileName.c_str(),_MAX_PATH);
	_splitpath(fullPath,drive,dir,fname,ext);

	return string(fname);
}

// get file directory
string GetFileDir(string originalFileName)
{
	char fullPath[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_fullpath(fullPath,originalFileName.c_str(),_MAX_PATH);
	_splitpath(fullPath,drive,dir,fname,ext);

	return (string(drive) + string(dir));
}

// get file extension (always lower case)
string GetFileExt(string originalFileName)
{
	char fullPath[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_fullpath(fullPath,originalFileName.c_str(),_MAX_PATH);
	_splitpath(fullPath,drive,dir,fname,ext);

	strlwr(ext);

	return string(ext);
}

// check file exist
bool IsFileExist(string filename)
{
	DWORD attr;
	attr = GetFileAttributes(filename.c_str());

	return (attr != INVALID_FILE_ATTRIBUTES && 
		(attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY);
}

// check directory exist
bool IsDirExist(string dirname)
{
	DWORD attr;
	attr = GetFileAttributes(dirname.c_str());

	return (attr != INVALID_FILE_ATTRIBUTES && 
		(attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
}

// check file is modified if archive attribute exist
bool IsFileModified(string filename)
{
	DWORD attr;
	attr = GetFileAttributes(filename.c_str());

	return (attr != INVALID_FILE_ATTRIBUTES && 
		(attr & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE);
}

// create directory if not exist
void CreateDir(string dirname)
{
	if (!IsDirExist(dirname))
	{
		CreateDirectoryA(dirname.c_str(), NULL);
	}
}

// create directory if not exist
void ChangeDir(string dirname)
{
	if (IsDirExist(dirname))
	{
		SetCurrentDirectoryA(dirname.c_str());
	}
}

// clear file archive attribute
void ClearArchive(string filename)
{
	DWORD attr = GetFileAttributes(filename.c_str());

	if (attr != INVALID_FILE_ATTRIBUTES)
		SetFileAttributes(filename.c_str(), attr & !FILE_ATTRIBUTE_ARCHIVE);
}

// get uppercase of string
string GetUpper(string s)
{
	char buf[_MAX_FNAME] = {0};

	s.copy(buf, s.length());
	strupr(buf);

	return string(buf);
}

// capitalize string
string CapString(string s)
{
	char c[2] = {0, 0};

	if (s.length() > 0) {
		s.copy(c, 1, 0);
		strupr(c);
		s.replace(0, 1, c);
	}

	return s;
}

// lower capitalize string
string LowCapString(string s)
{
	char c[2] = {0, 0};

	if (s.length() > 0) {
		s.copy(c, 1, 0);
		strlwr(c);
		s.replace(0, 1, c);
	}

	return s;
}

bool CheckImplExist(istream &is, CSpecification* spec, string suffix)
{
	char buf[4096];
	char *p, *e;
	string srch = "(const ";
	bool newImpl = false;

	while (!is.fail()) {
		is.getline(buf, 4096);
		e = strstr(buf, srch.c_str());
		if (e != NULL) {
			*e = '\0';
			e = strtok(buf, ": \t");
			while (e != NULL) {
				p = e;
				e = strtok(NULL, ": \t");
			}
			if (spec->IsClass() && (strncmp(p, "OnUpdate", 8) == 0)) {
				// attribute update
				newImpl |= spec->CheckAttrExist(LowCapString(p + 8));
			} else {
				// functions or methods
				newImpl |= spec->CheckFuncExist(string(p));
			}
		}
	}

	return newImpl;
}

//////////////////////////////////////////////////////////
//--------------- IDL functions --------------------------
//////////////////////////////////////////////////////////

void WriteHeaderStart(ostream &os, string def)
{
	pCurTpl->prewrite(os, "beginh");
	os<<"#ifndef " << def << endl;
	os<<"#define " << def << endl;
}

void WriteInclude(ostream &os, CSpecification *spec)
{
	pCurTpl->prewrite(os, "include");
	for(vector<string>::iterator line=spec->m_preprocess.m_lines.begin();
		line != spec->m_preprocess.m_lines.end(); line++)
	{
		os << *line;
	}
}

void WriteEnumInclude(ostream &os, CSpecification* spec, string enumPath)
{
	os << "#include \"" << enumPath << spec->m_idlName << "_enum.h\"\n";
}

void WriteStubInclude(ostream &os, CSpecification* spec, string suffix)
{
	pCurTpl->prewrite(os, "include");
	os << "#include \"" << suffix << ".h\"\n";
}

void WriteClassRegister(ostream &os, CSpecification* spec, string suffix)
{
	if (spec->IsClass() && spec->GetConfig("ClassID") != "") {
		pCurTpl->prewrite(os, "register");
		os << "REGISTER_CLASSTYPE(C" << spec->m_idlName << ", " << spec->m_idlName << "_CLASSID);\n";
	}
}

void WriteSubInclude(ostream &os, CSpecification* spec, string suffix)
{
	pCurTpl->prewrite(os, "include");
	os << "#include \"" << suffix << ".h\"\n";
}

void WriteNamespace(ostream &os, CSpecification* spec, string suffix)
{
	pCurTpl->prewrite(os, "namespace");
	os << "namespace " << spec->m_idlName << "_" << suffix << "\n";
}

void WriteFunctionIDs(ostream &os,CSpecification* spec)
{
	unsigned int lastId = 0;

	// write RPC enum
	os << "enum RPC_" << spec->m_idlName << "_Enum\n{\n";
	os << "\tRPC_" << spec->m_idlName << "_ENUM_ST\t= " 
		<< spec->GetStartId() << ",\n";
	for(CSpecification::Functions::iterator func0=spec->m_functions.begin();
		func0!=spec->m_functions.end();
		func0++)
	{
		CFunction& func=*func0;
		os << "\tRPC_" << spec->m_idlName << "_" << func.m_name
			<< "\t= " << func.m_ID << ",\n";
		lastId = func.m_ID;
	}
	os << "\tRPC_" << spec->m_idlName << "_ENUM_ED\t= " << lastId << "\n";

	//write RPC enum end
	os << "};\n";
}

void WriteFunctionDecls(ostream &os,CSpecification* spec, bool callback, bool comment)
{	
	for(CSpecification::Functions::iterator func0=spec->m_functions.begin();
		func0!=spec->m_functions.end();
		func0++)
	{
		CFunction& func=*func0;
		//if (func0->m_isCb != callback) continue;
		if (comment && func0->m_isCb == callback)
			continue;
		if (spec->IsClass())
			os << "\tvirtual ";
		WriteFunctionDecl(os,&func,"");
		if (!comment && spec->IsClass() && func0->m_isCb != callback) {
			//if (spec->GetConfig("CloneMethod") == "1")
			//	os << " { return RPC_RESULT_OK; };\n";
			//else
				os << " = 0;\n";

		} else
			os<<";\n";
	}
}

// Writes an RPC function declaration
void WriteFunctionDecl(ostream &os,CFunction* func, string prefix)
{
	os << "RPCResult " << prefix << func->m_name
		<< "(LPCPEER pPeer, RPCContext &context";
	WriteParametersWithComma(os,func);
	os <<")";
}

// Write RPC function parameters starting with commas
void WriteParametersWithComma(ostream &os,CFunction* func)
{
	for(CFunction::Parameters::iterator param0=func->m_params.begin();param0!=func->m_params.end();param0++)
	{
		CParameter& param=*param0;
		if (param.IsPointer()) {			// pointer type
			os << ", " << param.m_type << " ";
		} else if (param.IsReference()) {	// reference type
			os << ", " << param.m_type << " &";
		} else {
			os << ", const " << param.m_type;
			if (param.m_count != "")
				os << " *";
			else
				os << " &";
		}
		os << param.m_name;
	}
}

void WriteFunctionProxies(ostream &os,CSpecification* spec, bool callback, 
						  string suffix, bool bHeader)
{
	string prefix = spec->m_idlName + "_" + suffix + "::";

	pCurTpl->prewrite(os, "impl");
	for(CSpecification::Functions::iterator func0=spec->m_functions.begin();
		func0!=spec->m_functions.end();
		func0++)
	{
		CFunction& func=*func0;
		if (func0->m_isCb != callback) continue;
		if (bHeader)
			os << "INLINE ";		// INLINE before declaraction
		WriteFunctionDecl(os, &func, prefix);
		os<<"\n{\n";
		os<<"\tif (pPeer == NULL || pPeer->IsAlive() == BOOLEAN_FALSE) return RPC_RESULT_FAIL;\n";
		os<<"\tBuffer* pBuf = SafeCreateObject(Buffer);\n";
		os<<"\tReuben::Network::RPCHeader header(" 
			<< "RPC_"<< spec->m_idlName << "_" << func.m_name << ");\n";
		os<<"\tpBuf->Write<Reuben::Network::RPCHeader>(header);\n";
		if (callback) {
			os<<"\tif (context.bCallback && context.pcdId.subId != 0)\n";
			os<<"\t\tpBuf->Write<RPCContext>(GetCbContext(context));\n";
			os<<"\telse\n";
		}
		os<<"\t\tpBuf->Write<RPCContext>(context);\n";

		if (spec->IsClass()) {
			os<<"\tpBuf->Write<RPCNetID>(m_netId);\n";
		}
		for(CFunction::Parameters::iterator param0=func.m_params.begin();param0!=func.m_params.end();param0++)
		{
			CParameter& param=*param0;
			os <<"\tpBuf->Write";
			WriteMessage(os, spec, param);
		}
		if (func.m_secret != 0) {
			os << "\treturn RPCSend<" << func.m_secret << ">(pPeer, pBuf);\n";
		} else
			os << "\treturn RPCSend(pPeer, pBuf);\n";
		os << "}\n\n";
	}
}

void WriteProxyDecl(ostream &os,CSpecification* spec, bool callback)
{
	//write	RPC namespace begin
	pCurTpl->prewrite(os, "namespace");
	os << "namespace " << spec->m_idlName << "_Proxy" << " {\n";
	//os<<"RPCResult Process(const RPCID rpcId, const Peer &peer, RPCContext &context, RPCMessage &buf);\n";

	pCurTpl->prewrite(os, "decl");
	// write other function declarations
	WriteFunctionDecls(os, spec, callback);

	/*
	// write CB_Error function
	if (spec->IsClass())
		os << "\tvirtual ";
	os << "RPCResult CB_Error(const Peer &peer, RPCContext &context, const UInt32 &errCode, const String &errMsg);\n";
	*/
}

void WriteStubDecl(ostream &os,CSpecification* spec, bool callback)
{
	//write	RPC namespace begin
	pCurTpl->prewrite(os, "namespace");
	os << "namespace " << spec->m_idlName << "_Stub" << " {\n";
	//os<<"RPCResult Process(const RPCID rpcId, const Peer &peer, RPCContext &context, RPCMessage &msg);\n";

	pCurTpl->prewrite(os, "decl");
	// write other function declarations
	WriteFunctionDecls(os, spec, callback);
}

void WriteMessage(ostream &os,CSpecification* spec, CParameter &param, bool isAttr)
{
	if (param.m_count != "")
		os << "Array";
	else if (param.m_digits != "")	// write float with digits
		os << "Float";
	else if (param.m_rangeStart != "" && param.m_rangeEnd != "")	// write range
		os << "Range";
	os << "<" << param.m_type << ">(";
	if (isAttr)
		os << "m.";
	os << param.m_name;
	if (param.m_count != "")
		os << ", " << param.m_count;
	else if (param.m_digits != "")
		os << ", " << param.m_rangeStart << ", " << param.m_rangeEnd
		<< ", " << param.m_digits;
	else if (param.m_rangeStart != "" && param.m_rangeEnd != "")
		os << ", " << param.m_rangeStart << ", " << param.m_rangeEnd;
	os << ");\n";
}

void WriteMessageIndex(ostream &os,CSpecification* spec, CParameter &param, int index, bool isAttr)
{
	if (param.m_digits != "")	// write float with digits
		os << "Float";
	else if (param.m_rangeStart != "" && param.m_rangeEnd != "")	// write range
		os << "Range";
	os << "<" << param.m_type << ">(";
	if (isAttr)
		os << "m.";
	os << param.m_name << index;
	if (param.m_digits != "")
		os << ", " << param.m_rangeStart << ", " << param.m_rangeEnd
		<< ", " << param.m_digits;
	else if (param.m_rangeStart != "" && param.m_rangeEnd != "")
		os << ", " << param.m_rangeStart << ", " << param.m_rangeEnd;
	os << ");\n";
}

void WriteProcess(ostream &os,CSpecification* spec, bool callback, string suffix)
{
	pCurTpl->prewrite(os, "process");
	os << "RPCResult " << spec->m_idlName << "_" << suffix
		<< "::Process(const RPCID rpcId, LPCPEER pPeer, RPCContext &context, Buffer* pBuf)\n";

	// additional case for class
	if (spec->IsClass()) {
		if (suffix == "Proxy") {
			pCurTpl->prewrite(os, "upcase");
			os<<"\tcase RPC_" << spec->m_idlName << "_UP_STATE:" << endl;
		} else if (suffix == "Stub") {
			pCurTpl->prewrite(os, "reqcase");
			os<<"\tcase RPC_" << spec->m_idlName << "_REQ_STATE:" << endl;
			/*
			pCurTpl->prewrite(os, "updcase");
			os<<"\tcase RPC_" << spec->m_idlName << "_SEND_UPD:" << endl;
			*/
		}
	}

	/*
	// write CB_Error case in Process
	if (suffix == "Proxy") {
		pCurTpl->prewrite(os, "errcase");
		os<<"\tcase RPC_" << spec->m_idlName << "_CB_Error:" << endl;
	}
	*/
	pCurTpl->prewrite(os, "procase");

	for(CSpecification::Functions::iterator func0=spec->m_functions.begin();
		func0!=spec->m_functions.end();
		func0++)
	{
		CFunction& func=*func0;
		if (func0->m_isCb != callback) continue;
		if (!func0->m_public)
			os<< "#ifndef CLIENT\n";
		os<<"\tcase RPC_" << spec->m_idlName << "_" << func.m_name << ":" << endl;
//		if (!func.m_public)
//			os <<"\t\tif (peer.IsTrust())\n";
		os<<"\t\t{\n";

//		if (func.m_public && func.m_isCb && func.m_secret != 0) {
//			os << "\t\t\t_result = RPCDecode<" << func.m_secret << ">(peer, pBuf);\n";
//		} else {
//			os << "\t\t\t_result = RPCDecode(peer, pBuf);\n";
//		}
//		os << "\t\t\tif (_result != RPC_RESULT_OK) break;\n";

		for(CFunction::Parameters::iterator param0=func.m_params.begin();param0!=func.m_params.end();param0++)
		{
			CParameter& param=*param0;
			os << "\t\t\t" << param.m_type << " " << param.m_name;
			if (param.m_count != "")
				os << "[" << param.m_count << "]";
			os << ";\n";
			os << "\t\t\tpBuf->Read";
			WriteMessage(os, spec, param);
		}

		os<<"\t\t\t_result = "<<func.m_name<<"(pPeer, context";
		WriteParametersWithCommaAndWithoutTypes(os,&func);
		os<<");\n";
		os<<"\t\t}\n";
		os<<"\t\tbreak;\n";
		if (!func0->m_public)
			os<< "#endif // CLIENT\n";
	}
	pCurTpl->prewrite(os, "procend");
}

void WriteDecode(ostream &os,CSpecification* spec, string suffix)
{
	pCurTpl->prewrite(os, "decode");
	os << "RPCResult " << spec->m_idlName << "_" << suffix
		<< "::Decode(const RPCID rpcId, LPCPEER pPeer, Buffer* pBuf)\n";

	pCurTpl->prewrite(os, "deccase");

	for(CSpecification::Functions::iterator func0=spec->m_extraFuncs.begin();
		func0!=spec->m_extraFuncs.end();
		func0++)
	{
		CFunction& func=*func0;
		if (!func.m_public || !func.m_isCb) continue;
		os<<"\tcase RPC_" << spec->m_idlName << "_" << func.m_name << ":" << endl;

		if (func.m_secret != 0) {
			os << "\t\t\t_result = RPCDecode<" << func.m_secret << ">(pPeer, pBuf);\n";
		} else {
			os << "\t\t\t_result = RPCDecode(pPeer, pBuf);\n";
		}
		os<<"\t\tbreak;\n";
	}
	pCurTpl->prewrite(os, "decend");
}

void WriteStubImpl(ostream &os, CSpecification* spec, bool callback, string suffix)
{
	string prefix;
	
	if (suffix != "")
		prefix = spec->m_idlName + "_" + suffix + "::";
	else
		prefix = "C" + spec->m_idlName + "::";

	pCurTpl->prewrite(os, "impl");
	for(CSpecification::Functions::iterator func0=spec->m_functions.begin();
		func0!=spec->m_functions.end();
		func0++)
	{
		CFunction& func=*func0;
		if (func0->m_isCb != callback || func0->m_implExist) continue;

		WriteFunctionDecl(os, &func, prefix);
		os<<"\n{\n";
		os<<"\treturn RPC_RESULT_OK;\n";
		os<<"}\n\n";
	}
}

void WriteEncryptMacro(ostream &os,CSpecification* spec)
{
	if (!pCurTpl->prewrite(os, "encrypt")) return;
	int j;

	os << "#ifdef _REUBEN_MATH_AES_H_\n";
	for(CSpecification::Functions::iterator it=spec->m_extraFuncs.begin();
		it!=spec->m_extraFuncs.end(); it++)
	{
		CFunction& func=*it;
		if (func.m_secret == 0) continue;

		if (!func.m_isCb) {
			os<<"AES_ENCRYPT_HEAD(" << func.m_secret << ")\n";
			j = 0;
			for (int i = 0; i < 32; i++) {
				if ((func.m_secret >> i) & 1) {
					if (j == 0)
						os << "\tAES_SUB_BYTES\n";
					else if (j == 1)
						os << "\tAES_SHIFT_ROWS\n";
					else if (j == 2)
						os << "\tAES_MIX_COLUMNS\n";
					else if (j == 3)
						os << "\tAES_ADD_ROUNDKEY\n";
				}
				j++;
				if (j == 4) j = 0;
			}
			os<<"AES_ENCRYPT_TAIL\n\n";
		} else {
			os<<"AES_DECRYPT_HEAD(" << func.m_secret << ")\n";
			j = 3;
			for (int i = 31; i >= 0; i--) {
				if ((func.m_secret >> i) & 1) {
					if (j == 0)
						os << "\tAES_ISUB_BYTES\n";
					else if (j == 1)
						os << "\tAES_ISHIFT_ROWS\n";
					else if (j == 2)
						os << "\tAES_IMIX_COLUMNS\n";
					else if (j == 3)
						os << "\tAES_IADD_ROUNDKEY\n";
				}
				j--;
				if (j == -1) j = 3;
			}
			os<<"AES_DECRYPT_TAIL\n\n";
		}
	}
	os << "#endif // _REUBEN_MATH_AES_H_\n";
}

void WriteParametersWithCommaAndWithoutTypes(ostream &os,CFunction* func)
{
	for(CFunction::Parameters::iterator param0=func->m_params.begin();param0!=func->m_params.end();param0++)
	{
		CParameter& param=*param0;
		os<<", ";
		os<<param.m_name;
	}
}

void WriteHeaderEnd(ostream &os, string prefix)
{
	pCurTpl->prewrite(os, "endh");
	os<<"#endif //" << prefix << endl;
}

///////////////////////////////////////////////
//--------------- IDL Class ------------------
///////////////////////////////////////////////

void RandEnumIDs(CSpecification* spec, unsigned int rev)
{
	unsigned int rpcid;
	unsigned int sid = spec->GetStartId();
	int range = spec->GetIdRange();
	map<unsigned short, bool> idMap;
		
	// +1 for UP_STATE
	if (spec->IsClass()) {
		CFunction efunc;

		// add UP_STATE method
		efunc.m_isCb = true;
		efunc.m_public = true;
		efunc.m_name = "UP_STATE";
		spec->m_extraFuncs.push_back(efunc);

		// add REQ_STATE method
		efunc.m_isCb = false;
		efunc.m_public = true;
		efunc.m_name = "REQ_STATE";
		spec->m_extraFuncs.push_back(efunc);

	}
	// set random seed for generating number
	int startId = atoi(spec->GetConfig("StartID").c_str());
	//printf("rand seed: %d, startId %d, revision %d\n", startId ^ rev, startId, rev);
	srand(startId ^ rev);

	for(CSpecification::Functions::iterator func0=spec->m_extraFuncs.begin();
		func0!=spec->m_extraFuncs.end();
		func0++)
	{
		CFunction& func=*func0;
		rpcid = (rand() % range) + sid;
		while (idMap.find(rpcid) != idMap.end()) {
			rpcid = (rand() % range) + sid;
		}
		idMap[rpcid] = true;
		func.m_rpcid = rpcid;
		if (func.m_public) {
			func.m_secret = ((rand() << 16) & 0xffff0000) | (rand() & 0xffff);
		}
// 		printf("Function %s: rpcid = %d, secret = %d\n", func.m_name.c_str(), 
// 			func.m_rpcid, func.m_secret);
	}

	for(CSpecification::Functions::iterator func0=spec->m_functions.begin();
		func0!=spec->m_functions.end();
		func0++)
	{
		CFunction& func=*func0;
		rpcid = (rand() % range) + sid;
		while (idMap.find(rpcid) != idMap.end()) {
			rpcid = (rand() % range) + sid;
		}
		idMap[rpcid] = true;
		func.m_rpcid = rpcid;
		if (func.m_public) {
			func.m_secret = ((rand() << 16) & 0xffff0000) | (rand() & 0xffff);
		}
// 		printf("Function %s: rpcid = %d, secret = %d\n", func.m_name.c_str(), 
// 			func.m_rpcid, func.m_secret);
	}

	// insert all functions into extra list
	spec->m_extraFuncs.insert(spec->m_extraFuncs.end(), spec->m_functions.begin(), 
		spec->m_functions.end());

}

unsigned int FindExtraSecret(CSpecification* spec, string funcName)
{
	for(CSpecification::Functions::iterator func0=spec->m_extraFuncs.begin();
		func0!=spec->m_extraFuncs.end();
		func0++)
	{
		CFunction& func=*func0;
		if (func.m_name == funcName)
			return func.m_secret;
	}

	return 0;
}

/*
void WriteEnumIDs(ostream &os, CSpecification* spec)
{
	unsigned int eid = spec->GetStartId();

	pCurTpl->prewrite(os, "enum");

	// write RPC enum
	os << "enum RPC_" << spec->m_idlName << "_Enum\n";
	pCurTpl->prewrite(os, "decl");
	if (spec->IsClass() && spec->GetConfig("ClassID") != "") {
		os << "\t" << spec->m_idlName << "_CLASSID\t= " 
			<< spec->GetConfig("ClassID") << ",\n"; 
	}
	os << "\tRPC_" << spec->m_idlName << "_ENUM_ST\t= " 
		<< eid << ",\n";
	if (spec->IsClass()) {
		os << "\tRPC_" << spec->m_idlName << "_UP_STATE\t= " 
			<< eid++ << ",\n";
		os << "\tRPC_" << spec->m_idlName << "_REQ_STATE\t= " 
			<< eid++ << ",\n";
		//os << "\tRPC_" << spec->m_idlName << "_CB_Error\t= " 
		//	<< eid++ << ",\n";
		//os << "\tRPC_" << spec->m_idlName << "_SEND_UPD\t= " 
		//	<< eid++ << ",\n";
	}

	for(CSpecification::Functions::iterator func0=spec->m_functions.begin();
		func0!=spec->m_functions.end();
		func0++)
	{
		CFunction& func=*func0;
		os << "\tRPC_" << spec->m_idlName << "_" << func.m_name
			<< "\t= " << eid++ << ",\n";
	}
	os << "\tRPC_" << spec->m_idlName << "_ENUM_ED\t= " << (eid - 1) << "\n";
}
*/

void WriteEnumIDs(ostream &os, CSpecification* spec)
{
	unsigned int eid = spec->GetStartId();

	pCurTpl->prewrite(os, "enum");

	// write RPC enum
	os << "enum RPC_" << spec->m_idlName << "_Enum\n";
	pCurTpl->prewrite(os, "decl");
	if (spec->IsClass() && spec->GetConfig("ClassID") != "") {
		os << "\t" << spec->m_idlName << "_CLASSID\t= " 
			<< spec->GetConfig("ClassID") << ",\n"; 
	}
	os << "\tRPC_" << spec->m_idlName << "_ENUM_ST\t= " 
		<< eid << ",\n";

//	if (spec->IsClass()) {
//		os << "\tRPC_" << spec->m_idlName << "_UP_STATE\t= " 
//			<< eid++ << ",\n";
//		os << "\tRPC_" << spec->m_idlName << "_REQ_STATE\t= " 
//			<< eid++ << ",\n";
//		//os << "\tRPC_" << spec->m_idlName << "_CB_Error\t= " 
//		//	<< eid++ << ",\n";
//		//os << "\tRPC_" << spec->m_idlName << "_SEND_UPD\t= " 
//		//	<< eid++ << ",\n";
//	}

	for(CSpecification::Functions::iterator func0=spec->m_extraFuncs.begin();
		func0!=spec->m_extraFuncs.end();
		func0++)
	{
		CFunction& func=*func0;
		os << "\tRPC_" << spec->m_idlName << "_" << func.m_name
			<< "\t= " << func.m_rpcid << ",\n";
	}
	os << "\tRPC_" << spec->m_idlName << "_ENUM_ED\t= " << spec->GetEndId() << "\n";

	bool bNonblock = false;

	// write add RPC enum function
	pCurTpl->prewrite(os, "addrpc");

	os << "INLINE int _AddRpcEnum_" << spec->m_idlName << "()\n";
	os << "{\n";

	for(CSpecification::Functions::iterator func0=spec->m_extraFuncs.begin();
		func0!=spec->m_extraFuncs.end();
		func0++)
	{
		CFunction& func=*func0;
		if (func.m_isCb || !func.m_block)
			bNonblock = true;
		os << "\tRPC_ENUM_MAP[RPC_" << spec->m_idlName << "_" << func.m_name
			<< "] = " << func.m_secret << ";\n";
	}
	os << "\treturn 0;\n";
	os << "}\n";
	os << "static int _addrpc_" << spec->m_idlName << " = _AddRpcEnum_" 
		<< spec->m_idlName << "();\n";

	// write nonblocking RPC id map function
	pCurTpl->prewrite(os, "nonblock");

	if (bNonblock) {
		os << "INLINE int _NonblockMap_" << spec->m_idlName << "()\n";
		os << "{\n";

		for(CSpecification::Functions::iterator func0=spec->m_extraFuncs.begin();
			func0!=spec->m_extraFuncs.end();
			func0++)
		{
			CFunction& func=*func0;
			if (func.m_isCb || !func.m_block)
				os << "\tRPC_NONBLOCK_MAP[RPC_" << spec->m_idlName << "_" << func.m_name
					<< "] = true;\n";
		}
		os << "\treturn 0;\n";
		os << "}\n";
		os << "static int _nonblock_" << spec->m_idlName << " = _NonblockMap_" 
			<< spec->m_idlName << "();\n";
	}
}

void WriteAttrDecl(ostream &os,CSpecification* spec, CAttribute&attr, string indent)
{
	os << indent << "//" << attr.m_name << endl;

	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++) {
		CParameter &field = *pField;
		//os << indent << field.m_type << " m_" << field.m_name;
		os << indent << field.m_type << " " << field.m_name;
		if (field.m_count != "")
			os << "[" << field.m_count << "]";
		os << ";\n";
	}

	for (CAttribute::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteAttrDecl(os, spec, *pAttr, indent + "\t");
	}
}

void WriteGetterDecls(ostream &os,CSpecification* spec, CAttribute&attr)
{
	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++) {
		CParameter &field = *pField;
		os << "\tconst " << field.m_type << "& Get" << CapString(field.m_name);
		if (field.m_count != "") {
			os << "(const UInt8 i) const { return m." << field.m_name << "[i]; }\n";
		} else {
			os << "() const { return m." << field.m_name << "; }\n";
		}
	}
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteGetterDecls(os, spec, *pAttr);
	}
}

void WriteSetterParams(ostream &os,CSpecification* spec, CAttribute &attr, bool first)
{
	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++)
	{
		CParameter &field = *pField;
		if (first)
			first = false;
		else
			os << ", ";
		if (field.m_count != "") {
			// expand array for parameters
			int count = atoi(field.m_count.c_str());
			for (int i = 0; i < count; i++) {
				os << "const " << field.m_type << " &" << field.m_name << i;
				if (i < count - 1)
					os << ", ";
			}
		} else
			os << "const " << field.m_type << " &" << field.m_name;
	}
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteSetterParams(os, spec, *pAttr, first);
	}
}

void WriteSetterDecls(ostream &os,CSpecification* spec, CAttribute &attr)
{
	os << "\tvoid Set" << CapString(attr.m_name) << "(";
	WriteSetterParams(os, spec, attr, true);
	os << ");\n";

	// write field setter method
	if (!attr.m_fields.empty() && (attr.m_fields.size() > 1 || 
		stricmp(attr.m_name.c_str(), attr.m_fields.front().m_name.c_str()) != 0 ||
		attr.m_fields.front().m_count != ""))
	{
		for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
				pField != attr.m_fields.end(); pField++)
		{
			CParameter &field = *pField;
			if (field.m_count != "") {
				os << "\tvoid Set" << CapString(field.m_name) << "(const UInt8 i, const " 
					<< field.m_type << " &" << field.m_name << ");\n";
			} else {
				os << "\tvoid Set" << CapString(field.m_name) 
					<< "(const " << field.m_type << " &" << field.m_name << ");\n";
			}
		}
	}

	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteSetterDecls(os, spec, *pAttr);
	}
}

void WriteRequestDecls(ostream &os,CSpecification* spec, CAttribute &attr)
{
	os << "\tvoid Request" << CapString(attr.m_name) << "(RPCMask &mask) { mask.MaskBit("
		<< attr.m_name << "Mask); }\n";
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
		pAttr != attr.m_childs.end(); pAttr++) {
			WriteRequestDecls(os, spec, *pAttr);
		}
}

void WriteUpdateDecls(ostream &os,CSpecification* spec, CAttribute&attr, bool comment)
{
	os << "\tvirtual void OnUpdate" << CapString(attr.m_name) << "(";
	WriteSetterParams(os, spec, attr, true);
	if (comment)
		os << ");\n";
	else
		os << ") {};\n";
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteUpdateDecls(os, spec, *pAttr, comment);
	}
}

void WriteWriteDecls(ostream &os,CSpecification* spec, CAttribute &attr)
{
	os << "\tvoid Write" << CapString(attr.m_name) << "(Buffer *buffer, ";
	WriteSetterParams(os, spec, attr, true);
	os << ");\n";
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteWriteDecls(os, spec, *pAttr);
	}
}

void WriteSetterAssign(ostream &os,CSpecification* spec, CAttribute &attr)
{
	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++) {
		CParameter &field = *pField;
		if (field.m_count != "") {
			// expand array for assignment
			int count = atoi(field.m_count.c_str());
			for (int i = 0; i < count; i++) {
				os << "\tif (m." << field.m_name << "[" << i << "] != " 
					<< field.m_name << i << ") { "
					<< "m." << field.m_name << "[" << i << "] = " 
					<< field.m_name << i << "; modified = true; }\n";
			}
		} else
			os << "\tif (m." << field.m_name << " != " << field.m_name << ") { "
				<< "m." << field.m_name << " = " << field.m_name << "; modified = true; }\n";
	}
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteSetterAssign(os, spec, *pAttr);
	}
}

void WriteSetterField(ostream &os,CSpecification* spec, string suffix, CAttribute &attr, 
					  CParameter &field, bool bIndex=false)
{
	string istr;
	if (bIndex)		// field with index
		istr = "[i]";
	os << "void " << spec->m_idlName << "_" << suffix << "::Set" 
		<< CapString(field.m_name) << "(";
	if (bIndex)
		os << "const UInt8 i, ";
	os << "const " << field.m_type << " &" << field.m_name << ")\n";
	os << "{\n";
	os << "\tbool modified = false;\n";
	os << "\tif (m." << field.m_name << istr << " != " << field.m_name << ") { "
		<< "m." << field.m_name << istr << " = " << field.m_name
		<< "; modified = true; }\n";
	os << "\tMarkUpdateAll(" << attr.m_name << "Mask, modified);\n";
	os << "}\n\n";
}

void WriteSetterBody(ostream &os,CSpecification* spec, string suffix, CAttribute &attr)
{
	os << "void " << spec->m_idlName << "_" << suffix
		<< "::Set" << CapString(attr.m_name) << "(";
	WriteSetterParams(os, spec, attr, true);
	os << ")\n";
	os << "{\n";
	os << "\tbool modified = false;\n";
	WriteSetterAssign(os, spec, attr);
	os << "\tMarkUpdateAll(" << attr.m_name << "Mask, modified);\n";
	os << "}\n\n";

	// write field setter method
	if (!attr.m_fields.empty() && (attr.m_fields.size() > 1 || 
		stricmp(attr.m_name.c_str(), attr.m_fields.front().m_name.c_str()) != 0 ||
		attr.m_fields.front().m_count != ""))
	{
		for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
				pField != attr.m_fields.end(); pField++)
		{
			CParameter &field = *pField;
			WriteSetterField(os, spec, suffix, attr, field, (field.m_count != ""));
		}
	}
	// write child attribute body
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteSetterBody(os, spec, suffix, *pAttr);
	}
}

void WriteSetterImpl(ostream &os,CSpecification* spec, string suffix)
{
	pCurTpl->prewrite(os, "setimpl");

	if (suffix == "Stub" || suffix == "Proxy" && spec->GetConfig("TwoDirect") == "1") {
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
				pAttr != spec->m_class.m_attrs.end(); pAttr++) {
			WriteSetterBody(os, spec, suffix, *pAttr);
		}
	}
}

void WriteEnumMask(ostream &os, CAttribute &attr, int &index)
{
	os << "\t\t" << attr.m_name << "Mask\t= Parent::NextFreeMask + " 
		<< index++ << ",\n";
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteEnumMask(os, *pAttr, index);
	}
}

void WriteClearAllMask(ostream &os, CSpecification* spec)
{
	if (spec->GetConfig("ModifyMask") == "1" || spec->GetConfig("ViewMask") == "1") {
		os << "\tINLINE virtual VOID ClearAllMask() {\n\t\tParent::ClearAllMask();\n";
		if (spec->GetConfig("ModifyMask") == "1")
			os << "\t\tm_modifyMask.Clear();\n";
		if (spec->GetConfig("ViewMask") == "1")
			os << "\t\tm_viewMask.Clear();\n";
		os << "\t}\n";
	}
}

void WriteMarkUpdateAll(ostream &os, CSpecification* spec)
{
	if (spec->GetConfig("ModifyMask") == "1" || spec->GetConfig("ViewMask") == "1") {
		os << "\tINLINE virtual VOID MarkUpdateAll(UInt bit, bool modified) {\n\t\tParent::MarkUpdateAll(bit, modified);\n";
		if (spec->GetConfig("ModifyMask") == "1")
			os << "\t\tif (modified) m_modifyMask.MaskBit(bit);\n";
		if (spec->GetConfig("ViewMask") == "1")
			os << "\t\tif (modified) m_viewMask.MaskBit(bit);\n";
		os << "\t}\n";
	}
}

void WriteModifyMask(ostream &os)
{
	os << "\tRPCMask m_modifyMask;\n";
	os << "\tINLINE Boolean IsModified() { return (!m_modifyMask.IsZero()); }\n";
	os << "\tINLINE Boolean IsModified(UInt bit) { return (m_modifyMask.IsBit(bit)); }\n";
	os << "\tINLINE Boolean IsModified(CONST RPCMask& mask) { return (m_modifyMask.IsMask(mask)); }\n";
	os << "\tINLINE VOID SetModifyMask(CONST RPCMask& mask) { m_modifyMask = mask; }\n";
	os << "\tINLINE CONST RPCMask& GetModifyMask() CONST { return m_modifyMask; }\n";
	os << "\tINLINE VOID ClearModifyMask() { m_modifyMask.Clear(); }\n";
	os << "\tINLINE VOID MarkModify(UInt bit) { m_modifyMask.MaskBit(bit); }\n";
	os << "\tINLINE VOID MarkModify(CONST RPCMask& mask) { m_modifyMask.Mask(mask); }\n";
	os << "\tINLINE VOID UnmarkModify(UInt bit) { m_modifyMask.UnmaskBit(bit); }\n";
	os << "\tINLINE VOID UnmarkModify(CONST RPCMask& mask) { m_modifyMask.Unmask(mask); }\n";
}

void WriteViewMask(ostream &os)
{
	os << "\tRPCMask m_viewMask;\n";
	os << "\tINLINE VOID SetViewMask(CONST RPCMask& mask) { m_viewMask = mask; }\n";
	os << "\tINLINE CONST RPCMask& GetViewMask() CONST { return m_viewMask; }\n";
	os << "\tINLINE VOID ClearViewMask() { m_viewMask.Clear(); }\n";
	os << "\tINLINE VOID MarkView(UInt bit) { m_viewMask.MaskBit(bit); }\n";
	os << "\tINLINE VOID MarkView(CONST RPCMask& mask) { m_viewMask.Mask(mask); }\n";
	os << "\tINLINE VOID UnmarkView(UInt bit) { m_viewMask.UnmaskBit(bit); }\n";
	os << "\tINLINE VOID UnmarkView(CONST RPCMask& mask) { m_viewMask.Unmask(mask); }\n";
}

void WriteClassDecl(ostream &os,CSpecification* spec, const string &className, 
					const string &parentName, bool callback, string suffix, bool bSub)
{
	// class header
	if (pCurTpl->prewrite(os, "class")) {
		os << "class " << className;
		if (parentName == "") {
			os << " : public NetObject";
		} else if (parentName == "NetObject" || parentName == "NetGroup" ||
					parentName != spec->m_class.m_parent) {
			os << " : public " << parentName;
		} else {
			os << " : public C" << parentName;
		}
		os << endl;
	}

	// comment for virtual method and OnUpdate method for copy & paste
	if (pCurTpl->prewrite(os, "vmethod")) {
		WriteFunctionDecls(os, spec, callback, true);
		if (suffix == "Proxy") {
			// OnUpdate can be implemented selectively
			os << "\t// OnUpdate can be implemented selectively\n";
			if (!bSub) {
				for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
					pAttr != spec->m_class.m_attrs.end(); pAttr++)
				{
					WriteUpdateDecls(os, spec, *pAttr, true);
				}
			}
		}
	}

	// define Class Id for clone object
	if (pCurTpl->prewrite(os, "classid")) {
		os << "\tDEFINE_CLASS(" << spec->m_idlName << "_CLASSID);\n";
	}

	// class parent typedef
	boolean defParent = false;
	if (pCurTpl->prewrite(os, "parent")) {
		defParent = true;
		if (parentName == "") {
			os << "\ttypedef NetObject Parent;\n";
		} else if (parentName == "NetObject" || parentName == "NetGroup" ||
					parentName != spec->m_class.m_parent) {
			os << "\ttypedef " << parentName << " Parent;\n";
		} else {
			os << "\ttypedef C" << parentName << " Parent;\n";
		}
	}

	// update mask
	if (pCurTpl->prewrite(os, "mask")) {
		if (spec->m_class.m_attrs.size() > 0) {
			os << "\tenum UpdateMask {\n";
			int i = 0;
			for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
					pAttr != spec->m_class.m_attrs.end(); pAttr++) {
				WriteEnumMask(os, *pAttr, i);
			}
			os << "\t\tNextFreeMask\t= Parent::NextFreeMask + " << i++ << "\n";
			os << "\t};\n";
		}
	}

	// method declarations
	if (pCurTpl->prewrite(os, "decl")) {
		WriteFunctionDecls(os, spec, callback);
	}

	// getter declarations
	if (pCurTpl->prewrite(os, "getter")) {
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
				pAttr != spec->m_class.m_attrs.end(); pAttr++) {
			WriteGetterDecls(os, spec, *pAttr);
		}
	}

	/*
	// request attribute method declaration
	if (pCurTpl->prewrite(os, "request")) {
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++)
		{
			WriteRequestDecls(os, spec, *pAttr);
		}
	}
	*/

	// update notify method declaration
	if (pCurTpl->prewrite(os, "updecl")) {
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++)
		{
			WriteUpdateDecls(os, spec, *pAttr);
		}
	}

	if (suffix == "Proxy") {
		/*
		// write attribute and process method declaration
		if (pCurTpl->prewrite(os, "write")) {
			for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
					pAttr != spec->m_class.m_attrs.end(); pAttr++)
			{
				WriteWriteDecls(os, spec, *pAttr);
			}
		}
		*/
		if (pCurTpl->prewrite(os, "setdecl") && spec->GetConfig("TwoDirect") == "1") {
			for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
					pAttr != spec->m_class.m_attrs.end(); pAttr++)
			{
				WriteSetterDecls(os, spec, *pAttr);
			}
		}
	} else if (suffix == "Stub") {
		// setter declarations
		if (pCurTpl->prewrite(os, "setdecl")) {
			if (spec->GetConfig("ModifyMask") == "1") {
				WriteModifyMask(os);
			}
			if (spec->GetConfig("ViewMask") == "1") {
				WriteViewMask(os);
			}
			WriteClearAllMask(os, spec);
			WriteMarkUpdateAll(os, spec);
			for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
					pAttr != spec->m_class.m_attrs.end(); pAttr++)
			{
				WriteSetterDecls(os, spec, *pAttr);
			}
		}
		if (pCurTpl->prewrite(os, "clone") &&
			(spec->GetConfig("CloneMethod") == "1" || spec->GetConfig("TwoDirect") == "1"))
		{
			os << "\tvoid Clone(" << spec->m_idlName << "_Stub* obj, const RPCMask &mask);\n";
			os << "\tfriend void " << spec->m_idlName << "_Stub::Clone(" << spec->m_idlName << "_Stub* obj, const RPCMask &mask);\n";
		}
	}

	// class attribute
	if (pCurTpl->prewrite(os, "attribute")) {
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
				pAttr != spec->m_class.m_attrs.end(); pAttr++) {
			WriteAttrDecl(os, spec, *pAttr, "\t");
		}
	}

	// constructor declaration
	if (pCurTpl->prewrite(os, "construct")) {
		os << "\tINLINE " << className << "()\n";
	}

	// class parent typedef
	if (!defParent && pCurTpl->prewrite(os, "parent")) {
		if (parentName == "") {
			os << "\ttypedef NetObject Parent;\n";
		} else if (parentName == "NetObject" || parentName == "NetGroup" ||
					parentName != spec->m_class.m_parent) {
			os << "\ttypedef " << parentName << " Parent;\n";
		} else {
			os << "\ttypedef C" << parentName << " Parent;\n";
		}
	}

}

void WriteMethodParams(ostream &os,CSpecification* spec, CAttribute&attr, bool first, bool expand=false)
{
	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
		pField != attr.m_fields.end(); pField++)
	{
		CParameter &field = *pField;
		if (first)
			first = false;
		else
			os << ", ";

		if (expand && field.m_count != "") {
			// expand array for parameters
			int count = atoi(field.m_count.c_str());
			for (int i = 0; i < count; i++) {
				os << "m." << field.m_name << "[" << i << "]";
				if (i < count - 1)
					os << ", ";
			}
		} else
			os << "m." << field.m_name;
	}

	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
		pAttr != attr.m_childs.end(); pAttr++)
	{
		WriteMethodParams(os, spec, *pAttr, first, expand);
	}
}

void WriteWriteAssign(ostream &os,CSpecification* spec, CAttribute &attr)
{
	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++) {
		CParameter &field = *pField;
		if (field.m_count != "") {
			int count = atoi(field.m_count.c_str());
			for (int i = 0; i < count; i++) {
				os << "\tbuffer->Write";
				WriteMessageIndex(os, spec, field, i, false);
			}
		} else {
			os << "\tbuffer->Write";
			WriteMessage(os, spec, field, false);
		}
	}
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteWriteAssign(os, spec, *pAttr);
	}
}

void WriteWriteBody(ostream &os,CSpecification* spec, string suffix, CAttribute &attr)
{
	os << "void " << spec->m_idlName << "_" << suffix
		<< "::Write" << CapString(attr.m_name) << "(Buffer *buffer, ";
	WriteSetterParams(os, spec, attr, true);
	os << ")\n";
	os << "{\n";
	os << "\tif (buffer == NULL) return;\n";
	os << "\n";
	os << "\tbuffer->Write<RPCMask>(" << attr.m_name << "Mask);\n";
	WriteWriteAssign(os, spec, attr);
	os << "}\n\n";

	// write child attribute body
	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteWriteBody(os, spec, suffix, *pAttr);
	}
}

void WriteWriteImpl(ostream &os,CSpecification* spec, string suffix)
{
	pCurTpl->prewrite(os, "writeimpl");

	for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++) {
		WriteWriteBody(os, spec, suffix, *pAttr);
	}
}

void WriteUpdateBody(ostream &os,CSpecification* spec, string suffix, CAttribute &attr)
{
	if (!attr.m_implExist) {
		os << "void C" << spec->m_idlName << "::OnUpdate"
			<< CapString(attr.m_name) << "(";
		WriteSetterParams(os, spec, attr, true);
		os << ")\n";
		os << "{\n";
		os << "}\n\n";
	}

	for (CClass::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteUpdateBody(os, spec, suffix, *pAttr);
	}
}

void WriteUpdateImpl(ostream &os,CSpecification* spec, string suffix)
{
	pCurTpl->prewrite(os, "upimpl");
	for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++) {
		WriteUpdateBody(os, spec, suffix, *pAttr);
	}
}

void WriteAttrBit(ostream &os,CSpecification* spec, CAttribute &attr, string mask)
{
	bool single = mask.empty();
	mask += attr.m_name + "Mask";

	if (single)
		os << "\tif (mask.IsBit(" << mask << "))\n";
	else
		os << "\tif (mask.IsBits(" << mask << ", -1))\n";
	os << "\t{\n";
	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++) {
		CParameter &field = *pField;
		os << "\t\tbuf.Write";
		WriteMessage(os, spec, field, true);
	}
	os << "\t}\n";

	for (CAttribute::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteAttrBit(os, spec, *pAttr, mask + ", ");
	}
}

void WriteUpdateBit(ostream &os, CSpecification* spec, CAttribute &attr, bool first)
{
	if (first) {
		os << "\tcase " << attr.m_name << "Mask:\n";
	}

	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++) {
		CParameter &field = *pField;
		os << "\t\tbuffer->Write";
		WriteMessage(os, spec, field, true);
	}
	for (CAttribute::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteUpdateBit(os, spec, *pAttr, false);
	}
	if (first) {
		os << "\t\tOnUpdate" << CapString(attr.m_name) << "(";
		WriteMethodParams(os, spec, attr, true, true);
		os <<");\n";
		os << "\t\tbreak;\n";
		// write child case
		for (CAttribute::Attributes::iterator pAttr = attr.m_childs.begin();
				pAttr != attr.m_childs.end(); pAttr++) {
			WriteUpdateBit(os, spec, *pAttr, true);
		}
	}
}

void WriteAttrStub(ostream &os,CSpecification* spec)
{
	// clone object impl
	pCurTpl->prewrite(os, "cloneimpl");
	if (spec->GetConfig("CloneMethod") == "1" || spec->GetConfig("TwoDirect") == "1")
	{
		os << "void " << spec->m_idlName << "_Stub::Clone(" << spec->m_idlName
			<< "_Stub* obj, const RPCMask &mask)\n";

		pCurTpl->prewrite(os, "cloneset");
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
				pAttr != spec->m_class.m_attrs.end(); pAttr++) {
			WriteCloneSet(os, spec, *pAttr, "");
		}
		pCurTpl->prewrite(os, "cloneend");
	}

	// read attribute impl
	pCurTpl->prewrite(os, "readattr");
	os <<"void " << spec->m_idlName 
		<< "_Stub::ReadAttr(CONST Buffer &buf)\n";

	pCurTpl->prewrite(os, "readbit");
	if (spec->GetConfig("TwoDirect") == "1") {
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
				pAttr != spec->m_class.m_attrs.end(); pAttr++) {
			WriteAttrRead(os, spec, *pAttr, "");
		}
	}

	// write attribute impl
	pCurTpl->prewrite(os, "writeattr");
	os <<"void " << spec->m_idlName 
		<< "_Stub::WriteAttr(Buffer &buf, const RPCMask &mask) CONST\n";

	pCurTpl->prewrite(os, "writebit");
	for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++) {
		WriteAttrBit(os, spec, *pAttr, "");
	}

	// send attribute impl
	pCurTpl->prewrite(os, "sendattr");
	os << "RPCResult " << spec->m_idlName 
		<< "_Stub::SendAttr(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)\n";

	pCurTpl->prewrite(os, "upattr");
	os << "\tReuben::Network::RPCHeader header(RPC_" << spec->m_idlName << "_UP_STATE);\n";
	os << "\tpBuf->Write<Reuben::Network::RPCHeader>(header);\n";

	// send attribute impl
	//pCurTpl->prewrite(os, "sendattr2");
	//os << "INLINE RPCResult " << spec->m_idlName 
	//	<< "_Stub::SendAttr(const Peer &peer, RPCContext &context)\n";

	/*
	// receive update impl
	pCurTpl->prewrite(os, "updattr");
	os <<"INLINE void " << spec->m_idlName 
		<< "_Stub::RecvUpdate(Buffer *buffer)\n";

	pCurTpl->prewrite(os, "updbit");
	for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++) {
		WriteUpdateBit(os, spec, *pAttr, true);
	}
	*/
}

void WriteErrorImpl(ostream &os,CSpecification* spec)
{
	// callback error impl
	pCurTpl->prewrite(os, "errimpl");
	os << "RPCResult " << spec->m_idlName 
		<< "_Stub::CB_Error(LPCPEER pPeer, RPCContext &context, const UInt32 &errCode, const String &errMsg)\n";

	pCurTpl->prewrite(os, "errenum");
	os << "\tReuben::Network::RPCHeader header(RPC_" << spec->m_idlName << "_CB_Error);\n";
	os << "\tpBuf->Write<Reuben::Network::RPCHeader>(header);\n";

	pCurTpl->prewrite(os, "errend");
}

void WriteOnUpdate(ostream &os,CSpecification* spec, CAttribute &attr)
{
	os << "\tif (IsUpdated(" << attr.m_name << "Mask)) OnUpdate"
		<< CapString(attr.m_name) << "(";
	WriteMethodParams(os, spec, attr, true, true);
	os << ");\n";

	for (CAttribute::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteOnUpdate(os, spec, *pAttr);
	}
}

void WriteAttrRead(ostream &os,CSpecification* spec, CAttribute &attr, string mask)
{
	bool single = mask.empty();
	mask += attr.m_name + "Mask";

	if (single)
		os << "\tif (m_updateMask.IsBit(" << mask << "))\n";
	else
		os << "\tif (m_updateMask.IsBits(" << mask << ", -1))\n";
	os << "\t{\n";
	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); pField++) 
	{
		CParameter &field = *pField;
		os << "\t\tbuf.Read";
		WriteMessage(os, spec, field, true);
	}
	os << "\t}\n";

	for (CAttribute::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); pAttr++) {
		WriteAttrRead(os, spec, *pAttr, mask + ", ");
	}
}

void WriteAttrProxy(ostream &os,CSpecification* spec)
{
	// read attribute impl
	pCurTpl->prewrite(os, "readattr");
	os <<"void " << spec->m_idlName 
		<< "_Proxy::ReadAttr(CONST Buffer &buf)\n";

	pCurTpl->prewrite(os, "readbit");
	for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++) {
		WriteAttrRead(os, spec, *pAttr, "");
	}

	// write attribute impl
	pCurTpl->prewrite(os, "writeattr");
	os <<"void " << spec->m_idlName 
		<< "_Proxy::WriteAttr(Buffer &buf, const RPCMask &mask) CONST\n";

	pCurTpl->prewrite(os, "writebit");
	if (spec->GetConfig("TwoDirect") == "1") {
		for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
				pAttr != spec->m_class.m_attrs.end(); pAttr++) {
			WriteAttrBit(os, spec, *pAttr, "");
		}
	}

	// onUpdate callback impl
	pCurTpl->prewrite(os, "onupdate");
	os <<"void " << spec->m_idlName 
		<< "_Proxy::OnUpdate()\n";

	pCurTpl->prewrite(os, "updatecall");
	for (CClass::Attributes::iterator pAttr = spec->m_class.m_attrs.begin();
			pAttr != spec->m_class.m_attrs.end(); pAttr++) {
		WriteOnUpdate(os, spec, *pAttr);
	}

}

void WriteSendReq(ostream &os,CSpecification* spec)
{
	// send request impl
	pCurTpl->prewrite(os, "sendreq");
	os <<"INLINE RPCResult " << spec->m_idlName 
		<< "_Proxy::SendRequest(LPCPEER pPeer, RPCContext &context, const RPCMask &mask)\n";

	pCurTpl->prewrite(os, "reqattr");
	os << "\tReuben::Network::RPCHeader header(RPC_" << spec->m_idlName << "_REQ_STATE);\n";
	os << "\tpBuf->Write<Reuben::Network::RPCHeader>(header);\n";

	pCurTpl->prewrite(os, "sendreq2");
	os << "\treturn RPCSend<" << FindExtraSecret(spec, "REQ_STATE") << ">(pPeer, pBuf);\n";

	/*
	pCurTpl->prewrite(os, "sendupd");
	os <<"INLINE RPCResult " << spec->m_idlName 
		<< "_Proxy::SendUpdate(const Peer &peer, RPCContext &context, Buffer* buffer)\n";

	pCurTpl->prewrite(os, "updenum");
	os << "\tpBuf->Write<RPCID>(RPC_" << spec->m_idlName << "_SEND_UPD);\n";
	*/
}

void WriteCloneSet(ostream &os,CSpecification* spec, CAttribute &attr, string mask)
{
	bool single = mask.empty();
	mask += attr.m_name + "Mask";

	for (CAttribute::Fields::iterator pField = attr.m_fields.begin();
			pField != attr.m_fields.end(); ++pField)
	{
		CParameter &field = *pField;
		if (field.m_count == "") {
			if (single)
				os << "\tif (mask.IsBit(" << mask << ")";
			else
				os << "\tif (mask.IsBits(" << mask << ", -1)";
			os << " && !(obj->m." << field.m_name << " == m." << field.m_name << ")) {\n";
			os << "\t\tm." << field.m_name << " = obj->m." << field.m_name << ";\n";
			os << "\t\tMarkUpdateAll(" << attr.m_name << "Mask, true);\n";
		} else {
			int count = atoi(field.m_count.c_str());
			if (single)
				os << "\tif (mask.IsBit(" << mask << ")) {\n";
			else
				os << "\tif (mask.IsBits(" << mask << ", -1)) {\n";
			os << "\t\tbool updated = false;\n";
			for (int i = 0; i < count; i++)
			{
				os << "\t\tif (!(obj->m." << field.m_name << "[" << i << "] == m." << field.m_name << "[" << i << "])) {\n";
				os << "\t\t\tm." << field.m_name << "[" << i << "] = obj->m." << field.m_name << "[" << i << "];\n";
				os << "\t\t\tupdated = true;\n";
				os << "\t\t}\n";
			}
			os << "\t\tif (updated) MarkUpdateAll(" << attr.m_name << "Mask, true);\n";
		}
		os << "\t}\n";
	}

	for (CAttribute::Attributes::iterator pAttr = attr.m_childs.begin();
			pAttr != attr.m_childs.end(); ++pAttr)
	{
		WriteCloneSet(os, spec, *pAttr, mask + ", ");
	}
}

///////////////////////////////////////////////////////////
//--------------- Main process functions ------------------
///////////////////////////////////////////////////////////

void WriteRPCManagerDecl(ostream &os, string prefix)
{
	os << "#include \"System/Reuben.h\"\n";
	os << "#include \"Network/Peer.h\"\n";
	os << "#include \"Network/RPCManager.h\"\n";
	os << "#include \"Network/EventSelectModel.h\"\n";
	os << "#include \"Utility/Singleton.h\"\n";
	os << "\n";
	os << "typedef Reuben::Network::Peer Peer;\n";
	os << "typedef Reuben::Network::RPCContext RPCContext;\n";
	os << "typedef Reuben::Network::RPCMessage RPCMessage;\n";
	os << "typedef Reuben::Network::RPCID RPCID;\n";
	os << "typedef Reuben::Network::RPCResult RPCResult;\n";
	os << "typedef Reuben::Network::Address Address;\n";
	os << "\n";
	os << "class " << prefix << " : \n";
	os << "\tpublic Reuben::Network::RPCManager<Reuben::Network::EventSelectModel, " << prefix << ">,\n";
	os << "\tpublic Reuben::Utility::Singleton<" << prefix << ">\n";
	os << "{\n";
	os << "public:\n";
	os << "\tRPCResult OnProcess(LPCPEER pPeer, RPCContext &context, Buffer *pBuf);\n";
	os << "private:\n";
	os << "\tLocalRPCManager(){}\n";
	os << "\tSINGLETON(LocalRPCManager);\n";
	os << "};\n";
}

void WriteProcessInclude(ostream &os, CSpecification* spec, string suffix, string outPath)
{
	os << "#include \"" << outPath << spec->m_idlName << "_" << suffix << ".h\"\n";
}

RefCount<CSpecification> GetSpec(const vector<RefCount<CSpecification> > &specList, string name)
{
	for (vector<RefCount<CSpecification> >::const_iterator it = specList.begin();
		it != specList.end(); it++) 
	{
		RefCount<CSpecification> spec = *it;
		if (spec->m_idlName == name)
			return spec;
	}

	return RefCount<CSpecification>();
}

void WriteParentInclude(ostream &os, CSpecification* spec, vector<RefCount<CSpecification> > &specList,
						map<string, bool> &parentMap, string outPath)
{
	// write parent class header when RPC class parent is not "NetObject"
	if (spec->IsClass() && spec->m_class.m_parent != "" && 
		spec->m_class.m_parent != "NetObject" && 
		spec->m_class.m_parent != "NetGroup" && 
		parentMap.find(spec->m_class.m_parent) == parentMap.end())
	{
		RefCount<CSpecification> parentSpec = GetSpec(specList, spec->m_class.m_parent);
		if (parentSpec != 0) {
			os << "#include \"" << outPath << parentSpec->m_idlName << ".h\"\n";
			parentMap[spec->m_class.m_parent] = true;
		} else {
			printf("Error: Parent IDL %s not found for IDL %s\n", 
				spec->m_class.m_parent.c_str(), spec->m_idlName.c_str());
		}
	}
}

void WriteProcessCase(ostream &os, vector<RefCount<CSpecification> > &specList)
{
	pCurTpl->prewrite(os, "process");
	for (vector<RefCount<CSpecification> >::iterator spec = specList.begin(); spec != specList.end(); spec++) {
		os << "\telse if (";
		os << "rpcId >= RPC_" << (*spec)->m_idlName << "_ENUM_ST && rpcId <= RPC_" << (*spec)->m_idlName << "_ENUM_ED)\n";
		os << "\t{\n";
		os << "\t\t_result = ";
		if ((*spec)->IsClass()) {
			os << "Object";
		} else
			os << (*spec)->m_idlName << "_" << (*spec)->m_suffix << "::";
		os << "Process(rpcId, pPeer, context, pBuf);\n";
		os << "\t}\n";
	}
}

void WriteDecodeCase(ostream &os, vector<RefCount<CSpecification> > &specList)
{
	pCurTpl->prewrite(os, "decode");
	for (vector<RefCount<CSpecification> >::iterator spec = specList.begin(); spec != specList.end(); spec++) {
		if ((*spec)->m_suffix != "Proxy")
			continue;

		os << "\telse if (";
		os << "rpcId >= RPC_" << (*spec)->m_idlName << "_ENUM_ST && rpcId <= RPC_" << (*spec)->m_idlName << "_ENUM_ED)\n";
		os << "\t{\n";
		os << "\t\t_result = ";
		os << (*spec)->m_idlName << "_" << (*spec)->m_suffix << "::";
		os << "Decode(rpcId, pPeer, pBuf);\n";
		os << "\t}\n";
	}
}

void WriteEnumName(ostream &os, vector<RefCount<CSpecification> > &specList, unsigned int rev)
{
	pCurTpl->prewrite(os, "enumName");

	for (vector<RefCount<CSpecification> >::iterator it = specList.begin(); 
		it != specList.end(); ++it)
	{

		CSpecification* spec = (*it);

		// skip for special character IDL name
		if (spec->m_idlName.compare(0, 1, "-") == 0)
			continue;

		// not generated rpcid
		if (!spec->m_extraFuncs.empty() && spec->m_extraFuncs.front().m_rpcid == 0 ||
			!spec->m_functions.empty() && spec->m_functions.front().m_rpcid == 0)
		{
			RandEnumIDs(spec, rev);
		}

		for(CSpecification::Functions::iterator func0=spec->m_extraFuncs.begin();
			func0!=spec->m_extraFuncs.end();
			func0++)
		{
			CFunction& func=*func0;
			os << "\tcase " << func.m_rpcid << ": return _T(\"RPC_" << spec->m_idlName
				<< "_" << func.m_name << "\");\n";
		}
	}
}