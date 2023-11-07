// IDLComp.cpp : Defines the entry point for the console application.
//
//
// Copyright 2004 Bae,Hyun-jik.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#include "stdafx.h"
#include "IncludeGeneratedFiles.h"
#include "backend.h"
#include "FileTemplate.h"
#include "TaskList.h"
#include <vector>

using namespace antlr;

enum GenTemplate
{
	ENUM_TPL,			// enum template
	FUNC_H_TPL,			// function proxy and stub header template
	FUNC_C_TPL,			// function proxy and stub cpp template
	SUBFUNC_H_TPL,		// sub function proxy and stub header template
	SUBFUNC_C_TPL,		// sub function proxy and stub cpp template
	OBJ_P_H_TPL,		// proxy header template
	OBJ_P_C_TPL,		// proxy cpp template
	SUBOBJ_P_H_TPL,		// sub proxy header template
	SUBOBJ_P_C_TPL,		// sub proxy cpp template
	OBJ_S_H_TPL,		// stub header template
	OBJ_S_C_TPL,		// stub cpp template
	SUBOBJ_S_H_TPL,		// sub stub header template
	SUBOBJ_S_C_TPL,		// sub stub cpp template
	RPCM_C_TPL,			// LocalRPCManager cpp template
	//RPCM_H_TPL		// LocalRPCManager header template
	TPL_MAX,
};

#define REVISION_STRING		"VERSION_REVISION"
#define IDL_REVISION_FILE	"_idl_rev"

FileTemplate tplList[TPL_MAX];			// template list
FileTemplate *pCurTpl = &tplList[0];	// current template

_TCHAR *g_prog;						// program name
_TCHAR *g_config;					// config file name
int rc = 0;							// rc value for return
bool forceRegen = false;			// force all regenerate flag
bool localRegen = false;			// local RPC manager regenerate flag
bool checkVer = false;				// check version to regenerate all IDL
string rpcName = "LocalRPCManager";	// local rpc manager name
int revision = 0;					// project revision

int printUsage()
{
	cout << "A fast and simple RPC compiler by Jacky (GameOne)\n";
	cout << "Usage: " << g_prog << " [options] <IDL config file>\n";
	cout << "Where options are:\n";
	cout << "\t-r : force to regenerate all proxy and stub files\n";
	cout << "\t-l : only regenerate all LocalRPCManger.cpp files\n";
	cout << "\t-v : force to regenerate all proxy and stub files when SVN revision updated\n";
	cout << "\ne.g.:\n";
	cout << "IDLComp idlList.txt\n";

	return -1;
}

// load template
void LoadTemplate()
{
	string path = GetFileDir(g_prog);

	tplList[ENUM_TPL].read(			path + "template/enum.h");
	tplList[FUNC_H_TPL].read(		path + "template/func.h");
	tplList[FUNC_C_TPL].read(		path + "template/func.cpp");
	tplList[SUBFUNC_H_TPL].read(	path + "template/subfunc.h");
	tplList[SUBFUNC_C_TPL].read(	path + "template/subfunc.cpp");
	tplList[OBJ_P_H_TPL].read(		path + "template/obj_proxy.h");
	tplList[OBJ_P_C_TPL].read(		path + "template/obj_proxy.cpp");
	tplList[SUBOBJ_P_H_TPL].read(	path + "template/obj_subproxy.h");
	tplList[SUBOBJ_P_C_TPL].read(	path + "template/obj_subproxy.cpp");
	tplList[OBJ_S_H_TPL].read(		path + "template/obj_stub.h");
	tplList[OBJ_S_C_TPL].read(		path + "template/obj_stub.cpp");
	tplList[SUBOBJ_S_H_TPL].read(	path + "template/obj_substub.h");
	tplList[SUBOBJ_S_C_TPL].read(	path + "template/obj_substub.cpp");
	tplList[RPCM_C_TPL].read(		path + "template/LocalRPCManager.cpp");
}

void GetRevision(int idlRev)
{
	ifstream is;
	const char ws[] = " \t";
	char buf[1024];

	is.open(IDL_REVISION_FILE);
	if (!is.fail()) {
		is.getline(buf, 1024);
		revision = atoi(buf);			// get previous revision value
	}
	is.close();

	// if -v flag and version updated, force regen
	if (checkVer && idlRev != 0 && revision != idlRev) {
		ofstream os;

		os.open(IDL_REVISION_FILE);
		os << idlRev << "\n";
		os.close();
		forceRegen = true;
		revision = idlRev;
	}

	if (revision != 0) {
		printf("Project Version: %d, previous %d, forceRegen %d\n", 
			idlRev, revision, forceRegen);
	}
}

// parse IDL file
RefCount<CSpecification> parseIDL(string idlFile)
{
	RefCount<CSpecification> spec; // parse information
	IDLLexer* lexer = NULL;
	IDLParser* parser = NULL;

	try 
	{
		// front-end: read file and parse it
		{
			ifstream fileIn; 

			fileIn.open(idlFile.c_str());
			lexer = new IDLLexer(fileIn);
			parser = new IDLParser(*lexer);
			spec=parser->specification();
			spec->SetIdlName(GetBaseName(idlFile));
			if (spec->IsClass())
				spec->m_functions = spec->m_class.m_methods;
		}
	}
	catch( ANTLRException& e )
	{
		cerr << "Exception " << idlFile << " at " << lexer->getLine() << ","
			<< lexer->getColumn() << " : " << e.getMessage() << endl;
		rc = -1;
		return spec;
	}
	catch(exception& e)
	{
		cerr << "Exception: " << e.what() << endl;
		rc = -1;
		return spec;
	}

	if ((spec->IsClass() && (int) spec->m_class.m_methods.size() > spec->GetIdRange()) ||
		(int) spec->m_functions.size() > spec->GetIdRange()) {
		cout << "Error: RPC functions are out of the specified ID range.\n";
		rc = -1;
		return spec;
	}

	rc = 0;
	return spec;
}

// Generate enum header file
void GenEnumFile(TaskList &tasks, Section &sect, string &inputFile, RefCount<CSpecification> spec)
{
	// check if already done
	string enumfile = GetSubstitutedFileName(inputFile, 
		(sect.rootPath + sect.enumPath).c_str(), "_enum","h");
	if (tasks.IsDone(enumfile))
		return;

	// write common enum header
	string enumdef = "RPC_" + GetUpper(spec->m_idlName) + "_ENUM";	// header define
	ofstream enumh;

	pCurTpl = &tplList[ENUM_TPL];
	cout << "Generate enum files for " << inputFile << " ...\n";
	enumh.open(enumfile.c_str());
	WriteHeaderStart(enumh, enumdef);
	WriteEnumIDs(enumh,spec);
	WriteHeaderEnd(enumh, enumdef);
	enumh.close();

	tasks.SetDone(enumfile);
}

// Generate proxy header and cpp file
void GenProxyFile(TaskList &tasks, Section &sect, string &inputFile, RefCount<CSpecification> spec)
{
	// check header file if already done
	string proxyfile;
	string proxyPath = sect.proxyPath + spec->GetConfig("SubPath");
	proxyfile = GetSubstitutedFileName(inputFile, (sect.rootPath + proxyPath).c_str(), "_proxy","h");
	if (tasks.IsDone(proxyfile))
		return;

	// create subpath
	CreateDir((sect.rootPath + proxyPath).c_str());

	// generate client proxy header
	string hdef = "_RPC_" + GetUpper(spec->m_idlName) + "_PROXY_H_";	// header define
	ofstream os;

	if (spec->IsClass())
		pCurTpl = &tplList[OBJ_P_H_TPL];
	else
		pCurTpl = &tplList[FUNC_H_TPL];
	cout << "Generate proxy files for " << inputFile << " ...\n";
	os.open(proxyfile.c_str());
	WriteHeaderStart(os, hdef);
	WriteInclude(os, spec);
	WriteEnumInclude(os, spec, sect.enumPath);
	if (spec->IsClass()) {		// RPC class
		WriteClassDecl(os,spec,spec->m_idlName + "_Proxy", spec->m_class.m_parent, false,"Proxy");
		WriteSendReq(os, spec);
	} else {					// RPC functions
		WriteProxyDecl(os,spec,false);
	}
	WriteFunctionProxies(os,spec,false,"Proxy",true);
	WriteEncryptMacro(os,spec);
	WriteHeaderEnd(os, hdef);
	os.close();
	tasks.SetDone(proxyfile);

	// generate client proxy cpp 
	if (spec->IsClass())
		pCurTpl = &tplList[OBJ_P_C_TPL];
	else
		pCurTpl = &tplList[FUNC_C_TPL];

	string cppfile = GetSubstitutedFileName(inputFile, (sect.rootPath + proxyPath).c_str(), "_proxy","cpp");
	os.open(cppfile.c_str());
	WriteStubInclude(os,spec, proxyPath + spec->m_idlName + "_proxy");
	if (spec->IsClass()) {
		WriteSetterImpl(os,spec, "Proxy");
		WriteAttrProxy(os,spec);
	}
	WriteProcess(os,spec,true, "Proxy");
	WriteDecode(os,spec, "Proxy");
	if (!spec->IsClass())					// skip function impl in proxy.cpp
		pCurTpl->prewrite(os, "impl");
	os.close();

	// generate subclass header only once
	if (spec->IsClass())
		pCurTpl = &tplList[SUBOBJ_P_H_TPL];
	else
		pCurTpl = &tplList[SUBFUNC_H_TPL];
	string subhfile = GetSubstitutedFileName(inputFile, (sect.rootPath + proxyPath).c_str(), "","h");
	if (!IsFileExist(subhfile)) {
		hdef = "_" + GetUpper(spec->m_idlName) + "_H_";	// header define
		os.open(subhfile.c_str());
		WriteHeaderStart(os, hdef);
		WriteSubInclude(os, spec, proxyPath + spec->m_idlName + "_Proxy");
		if (spec->IsClass())
			WriteClassDecl(os, spec, "C" + spec->m_idlName, spec->m_idlName + "_Proxy", 
				false,"Proxy", true);
		else
			WriteNamespace(os, spec, "Proxy");
		WriteHeaderEnd(os, hdef);
		os.close();
	}

	// generate subclass cpp only once
	if (spec->IsClass())
		pCurTpl = &tplList[OBJ_P_C_TPL];
	else
		pCurTpl = &tplList[FUNC_C_TPL];
	cppfile = GetSubstitutedFileName(inputFile, (sect.rootPath + proxyPath).c_str(), "","cpp");
	if (!IsFileExist(cppfile)) {
		os.open(cppfile.c_str());
		WriteStubInclude(os,spec, proxyPath + spec->m_idlName);
		if (spec->IsClass()) {
			WriteClassRegister(os,spec, proxyPath + spec->m_idlName);
			//WriteUpdateImpl(os,spec,"Proxy");
			WriteStubImpl(os, spec, true, "");
		} else {
			WriteNamespace(os, spec, "Proxy");
			WriteStubImpl(os, spec, true, "Proxy");
		}
		os.close();
	}

	// skip generate cpp file and generate complete virtual method in header for class 
	/*
	// write client proxy cpp
	proxyfile = GetSubstitutedFileName(inputFile, sect.path.c_str(),
		"_proxy","cpp");
	bool exist = IsFileExist(proxyfile);

	if (exist) {
		ifstream proxyc;
		bool newImpl;

		proxyc.open(proxyfile.c_str());
		newImpl = CheckImplExist(proxyc, spec, "Proxy");
		proxyc.close();

		// skip to modified if no new implementation functions or methods
		if (!newImpl)
			return;
	}

	if (spec->IsClass())
		pCurTpl = &tplList[OBJ_P_C_TPL];
	else
		pCurTpl = &tplList[FUNC_C_TPL];
	proxyh.open(proxyfile.c_str(), ios_base::out | ios_base::app);
	if (!exist) {
		WriteStubInclude(proxyh, spec, "Proxy");
	}
	WriteStubImpl(proxyh,spec, true, "Proxy");
	proxyh.close();
	*/
}

// Generate stub header and cpp file
void GenStubFile(TaskList &tasks, Section &sect, string &inputFile, RefCount<CSpecification> spec)
{
	// check header file if already done
	string stubfile;
	string stubPath = sect.stubPath + spec->GetConfig("SubPath");
	stubfile = GetSubstitutedFileName(inputFile, (sect.rootPath + stubPath).c_str(), "_stub","h");
	if (tasks.IsDone(stubfile))
		return;

	// create subpath
	CreateDir((sect.rootPath + stubPath).c_str());

	// write server stub header
	string hdef = "_RPC_" + GetUpper(spec->m_idlName) + "_STUB_H_";	// header define
	ofstream os;

	if (spec->IsClass())
		pCurTpl = &tplList[OBJ_S_H_TPL];
	else
		pCurTpl = &tplList[FUNC_H_TPL];
	cout << "Generate stub files for " << inputFile << " ...\n";
	os.open(stubfile.c_str());
	WriteHeaderStart(os, hdef);
	WriteInclude(os, spec);
	WriteEnumInclude(os, spec, sect.enumPath);
	if (spec->IsClass()) {			// RPC class
		WriteClassDecl(os,spec,spec->m_idlName + "_Stub", spec->m_class.m_parent, true,"Stub");
	} else {						// RPC functions
		WriteStubDecl(os,spec,false);
	}
	//WriteErrorImpl(os, spec);
	//WriteFunctionProxies(os,spec,true,"Stub",false);
	if (!spec->IsClass()) {					// skip function encrypt and impl in stub.h
		pCurTpl->prewrite(os, "impl");
		pCurTpl->prewrite(os, "encrypt");
	}
	WriteHeaderEnd(os, hdef);
	os.close();
	tasks.SetDone(stubfile);

	// generate stub proxy cpp 
	if (spec->IsClass())
		pCurTpl = &tplList[OBJ_S_C_TPL];
	else
		pCurTpl = &tplList[FUNC_C_TPL];

	string cppfile = GetSubstitutedFileName(inputFile, (sect.rootPath + stubPath).c_str(), "_stub","cpp");
	os.open(cppfile.c_str());
	WriteStubInclude(os,spec, stubPath + spec->m_idlName + "_stub");
	if (spec->IsClass()) {
		WriteSetterImpl(os,spec, "Stub");
		WriteAttrStub(os,spec);
	}
	WriteProcess(os,spec,false, "Stub");
	WriteFunctionProxies(os,spec,true,"Stub",false);
	os.close();

	// generate subclass header only once
	if (spec->IsClass())
		pCurTpl = &tplList[SUBOBJ_S_H_TPL];
	else
		pCurTpl = &tplList[SUBFUNC_H_TPL];
	string subhfile = GetSubstitutedFileName(inputFile, (sect.rootPath + stubPath).c_str(), "","h");
	if (!IsFileExist(subhfile)) {
		hdef = "_" + GetUpper(spec->m_idlName) + "_H_";	// header define
		os.open(subhfile.c_str());
		WriteHeaderStart(os, hdef);
		WriteSubInclude(os, spec, stubPath + spec->m_idlName + "_Stub");
		if (spec->IsClass())
			WriteClassDecl(os,spec, "C" + spec->m_idlName, spec->m_idlName + "_Stub", true,
				"Stub", true);
		else
			WriteNamespace(os, spec, "Stub");
		WriteHeaderEnd(os, hdef);
		os.close();
	}

	// generate subclass cpp only once
	if (spec->IsClass())
		pCurTpl = &tplList[SUBOBJ_S_C_TPL];
	else
		pCurTpl = &tplList[SUBFUNC_C_TPL];
	cppfile = GetSubstitutedFileName(inputFile, (sect.rootPath + stubPath).c_str(), "","cpp");
	if (!IsFileExist(cppfile)) {
		os.open(cppfile.c_str());
		WriteStubInclude(os,spec, stubPath + spec->m_idlName);
		if (spec->IsClass()) {
			WriteClassRegister(os,spec, stubPath + spec->m_idlName);
			WriteStubImpl(os, spec, false, "");
		} else {
			WriteNamespace(os, spec, "Stub");
			WriteStubImpl(os, spec, false, "Stub");
		}
		os.close();
	}

	// skip generate cpp file and generate complete virtual method in header for class 
	/*
	// write server stub cpp
	stubfile = GetSubstitutedFileName(inputFile,
		sect.path.c_str(),"_stub","cpp");
	bool exist = IsFileExist(stubfile);

	if (exist) {
		bool newImpl;
		ifstream stubc;

		stubc.open(stubfile.c_str());
		newImpl = CheckImplExist(stubc, spec, "Stub");
		stubc.close();

		// skip to modified if no new implementation functions or methods
		if (!newImpl)
			return;
	}

	if (spec->IsClass())
		pCurTpl = &tplList[OBJ_S_C_TPL];
	else
		pCurTpl = &tplList[FUNC_C_TPL];
	stubh.open(stubfile.c_str(), ios_base::out | ios_base::app);
	if (!exist) {
		WriteStubInclude(stubh, spec, "Stub");
	}

	if (spec->IsClass()) {
		WriteUpdateImpl(stubh,spec,"Stub");
	}
	WriteStubImpl(stubh,spec,false, "Stub");
	stubh.close();
	*/
}

int GenProxyStubList(TaskList &tasks, Section &sect, IdlList &idlList, string suffix)
{
	RefCount<CSpecification> spec; // parse information

	for (IdlList::iterator idl = idlList.begin(); rc == 0 && idl != idlList.end(); idl++) {
		// remove special character from IDL name
		string idlName(*idl);
		if (idlName.compare(0, 1, "-") == 0)
			idlName = idlName.substr(1);

		// only generate modified IDL
		if (!forceRegen && tasks.CheckIDL(idlName) != TaskList::IDL_MODIFIED)
			continue;

		cout << "Parsing " << idlName << " ...\n";
		spec = parseIDL(idlName);
		if (rc == 0) {
			// random RPC ID and secret number
			RandEnumIDs(spec, revision);
			GenEnumFile(tasks, sect, idlName, spec);
			if (suffix == "Proxy")
				GenProxyFile(tasks, sect, idlName, spec);
			else if (suffix == "Stub")
				GenStubFile(tasks, sect, idlName, spec);
		} else {
			tasks.SetInvalidIDL(idlName);
		}
	}

	return rc;
}

int GenMainProcess(Section &sect, IdlList &proxyList, IdlList &stubList)
{
	string hfile = sect.path + rpcName + ".h";
	string cppfile = sect.path + rpcName + ".cpp";
	ofstream os;
	RefCount<CSpecification> spec; // parse information
	vector<RefCount<CSpecification> > specList;
	map<string, bool> parentMap;	// record down parent class header is included or not

	// write local RPC manager header
	cout << "Generating Local RPC manager at " << sect.path << " ...\n";
	/*
	pCurTpl = &tplList[RPCM_H_TPL];
	os.open(hfile.c_str());
	WriteHeaderStart(os, proxydef);
	//WriteRPCManagerDecl(os, rpcName);
	WriteHeaderEnd(os, proxydef);
	os.close();
	*/

	// write local RPC manager cpp
	pCurTpl = &tplList[RPCM_C_TPL];
	os.open(cppfile.c_str());

	// parse proxy idl
	pCurTpl->prewrite(os, "enum");

	// write include files
	for (IdlList::iterator inc = sect.incList.begin(); inc != sect.incList.end(); ++inc) {
		os << "#include \"" << *inc << "\"\n";
	}

	for (IdlList::iterator idl = proxyList.begin(); rc == 0 && idl != proxyList.end(); idl++) {
		// won't process "-" idl
		string idlName(*idl);
		if (idlName.compare(0, 1, "-") == 0)
			continue;

		spec = parseIDL(idlName);
		if (rc != 0) return rc;

		spec->m_suffix = "Proxy";
		/*
		// write parent class header when RPC class parent is not "NetObject" or "NetGroup"
		WriteParentInclude(os, spec, specList, parentMap, 
							sect.proxyPath + spec->GetConfig("SubPath"));
		*/

		// RPC class just need the enum header, RPC function need the proxy header
//		if (spec->IsClass())
//			WriteEnumInclude(os, spec, sect.enumPath);
//		else
			WriteProcessInclude(os, spec, "Proxy", sect.proxyPath + spec->GetConfig("SubPath"));
		specList.push_back(spec);
	}

	// parse stub idl
	for (IdlList::iterator idl = stubList.begin(); rc == 0 && idl != stubList.end(); idl++) {
		// won't process "-" idl
		string idlName(*idl);
		if (idlName.compare(0, 1, "-") == 0)
			continue;

		spec = parseIDL(idlName);
		spec->m_suffix = "Stub";
		if (rc != 0) return rc;

		spec->m_suffix = "Stub";
		/*
		// write parent class header when RPC class parent is not "NetObject" or "NetGroup"
		WriteParentInclude(os, spec, specList, parentMap, 
							sect.stubPath + spec->GetConfig("SubPath"));
		*/
		// RPC class just need the enum header, RPC function need the proxy header
		if (spec->IsClass())
			WriteEnumInclude(os, spec, sect.enumPath);
		else
			WriteProcessInclude(os, spec, "Stub", sect.stubPath + spec->GetConfig("SubPath"));
		specList.push_back(spec);
	}
	WriteProcessCase(os, specList);
	WriteDecodeCase(os, specList);
	WriteEnumName(os, specList, revision);
	pCurTpl->prewrite(os, "end");
	os.close();

	return 0;
}

int main(int argc, _TCHAR* argv[])
{
	TaskList tasks;
	g_prog = argv[0];
	int argi = 1;
	string options;

	while (argi < argc && *argv[argi] == '-') {
	// regenerate all proxy and stub files regardless idl modification or not
		if (stricmp(argv[argi], "-r") == 0) {
			forceRegen = true;
			options += argv[argi];
		} else if (stricmp(argv[argi], "-l") == 0) {
			localRegen = true;
			options += argv[argi];
		} else if (stricmp(argv[argi], "-v") == 0) {
			checkVer = true;
			options += argv[argi];
		}
		argi ++;
	}
	if (!options.empty()) {
		printf("%s: options %s\n", g_prog, options.c_str());
	}

	if (argc <= argi)
		return printUsage();

	// get config file and change current directory to config file
	g_config = argv[argi];
	ChangeDir(GetFileDir(g_config));

	// load task config file
	tasks.LoadTask(g_config);

	// get project revision
	GetRevision(tasks.GetVersion());

	// load template files
	LoadTemplate();

	// generate rpc manager, enum, proxy and stub for each section
	for (int i = 0; rc == 0 && i < tasks.GetSectionCount(); i++) {
		Section &sect = tasks.GetSection(i);

		cout << "For section [" << sect.name << "] ...\n";
		if (rc == 0 && (sect.proxyNeed || forceRegen))
			rc = GenProxyStubList(tasks, sect, sect.proxyList, "Proxy");
		if (rc == 0 && (sect.stubNeed || forceRegen))
			rc = GenProxyStubList(tasks, sect, sect.stubList, "Stub");
		if (rc == 0 && (sect.proxyNeed || sect.stubNeed || forceRegen || localRegen))
			rc = GenMainProcess(sect, sect.proxyList, sect.stubList);
	}
	// reset all modified IDL archive flag
	if (rc == 0)
		tasks.ClearAllArchive();

	return rc;
}

/*
int main(int argc, _TCHAR* argv[])
{
	int argi = 1;
	IdlList proxyList;
	IdlList stubList;

	g_prog = argv[0];

	while (argi < argc) {
		if (stricmp(argv[argi], "-path") == 0) {
			sect.path = argv[argi + 1];
			argi += 2;
		} else if (stricmp(argv[argi], "-enum") == 0) {
			enumInc = argv[argi + 1];
			argi += 2;
		} else if (stricmp(argv[argi], "-enumPath") == 0) {
			enumPath = argv[argi + 1];
			argi += 2;
			//} else if (stricmp(argv[argi], "-rpc") == 0) {
			//	rpcName = argv[argi + 1];
			//	argi += 2;
		} else if (stricmp(argv[argi], "-proxy") == 0) {
			argi ++;
			while (argi < argc && (argv[argi][0], '-') != 0 && 
				GetFileExt(argv[argi]) == ".idl")
			{
				proxyList.push_back(argv[argi]);
				argi++;
			}
		} else if (stricmp(argv[argi], "-stub") == 0) {
			argi ++;
			while (argi < argc && (argv[argi][0], '-') != 0 && 
				GetFileExt(argv[argi]) == ".idl")
			{
				stubList.push_back(argv[argi]);
				argi++;
			}
		} else {
			return printUsage();
		}
	}

	if (sect.path == "" || enumPath == "" || rpcName == "") {
		cout << "Error: Missing output path, enum output path or rpc manager name.\n";
		rc = printUsage();
	} else if (!IsDirExist(sect.path) || !IsDirExist(enumPath)) {
		cout << "Error: output path or enum output path does not exist.\n";
		rc = printUsage();
	} else {
		LoadTemplate();
		rc = GenProxyStubList(proxyList, "Proxy");
		rc = GenProxyStubList(stubList, "Stub");
		if (rc == 0)
			rc = GenMainProcess(proxyList, stubList);
	}

	return rc;
}
*/