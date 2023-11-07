//
// Copyright 2004 Bae,Hyun-jik.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#include "StdAfx.h"
#include ".\specification.h"
#include <antlr/SemanticException.hpp>

CSpecification::CSpecification(void)
{
}

CSpecification::~CSpecification(void)
{
}

void CSpecification::SetConfigs(CConfigurations *configs)
{
	m_configurations=*configs;
	m_configurations.m_incID=0;
}

void CSpecification::SetClass(CClass *clas)
{
	m_class = *clas;
}

void CSpecification::AddFunction(CFunction* func)
{
	func->m_public = m_public;
	func->m_block = m_block;
	m_functions.push_back(*func);
	m_functions.back().m_ID = m_configurations.m_startID + m_configurations.m_incID;
	m_configurations.m_incID++;
}

bool CSpecification::CheckFuncExist(string name)
{
	for (Functions::iterator func = m_functions.begin();
		func != m_functions.end(); func++)
	{
		if (func->m_name == name) {
			func->m_implExist = true;
			return true;
		}
	}
	return false;
}

bool CSpecification::CheckAttrExist(string name)
{
	for (CClass::Attributes::iterator pAttr = m_class.m_attrs.begin();
		pAttr != m_class.m_attrs.end(); pAttr++)
	{
		if (pAttr->CheckContain(name))
			return true;
	}
	return false;
}

int CSpecification::NewFuncCount()
{
	int count = 0;
	for (Functions::iterator func = m_functions.begin();
		func != m_functions.end(); func++)
	{
		if (!func->m_implExist)
			count ++;
	}

	return count;
}

int CSpecification::NewAttrCount()
{
	int count = 0;
	for (CClass::Attributes::iterator pAttr = m_class.m_attrs.begin();
		pAttr != m_class.m_attrs.end(); pAttr++)
	{
		count += 1 + pAttr->NewAttrCount();
	}

	return count;
}

CConfigurations::CConfigurations()
{
	m_incID = 0;
	m_startID = 1;
	m_endID = 100;
}

void CConfigurations::AddConfig(CConfiguration *config)
{
	if (config->m_name == "StartID")
	{
		sscanf(config->m_value.c_str(), "%u", &m_startID);
	}
	else if (config->m_name == "EndID")
	{
		sscanf(config->m_value.c_str(), "%u", &m_endID);
	}

	m_configs[config->m_name] = config->m_value;
	/*
	else
	{
		throw SemanticException("Unidentified config variable");
	}
	*/
}

string CConfigurations::GetConfig(string name)
{
	Configs::iterator iter = m_configs.find(name);

	if (iter == m_configs.end())
		return "";
	else
		return m_configs[name];
}

void CConfigurations::SetConfig(string name, string value)
{
	CConfiguration config;
	config.SetName(name);
	config.SetStrValue(value);
	AddConfig(&config);
}

void CConfiguration::SetName(string name)
{
	m_name=name;
}

void CConfiguration::SetIntValue(string value)
{
	m_value=value;
}

void CConfiguration::SetStrValue(string value)
{
	m_value=value.substr(1, value.length() - 2);
}

void CFunction::SetName(string name)
{
	// set callback function type for "CB_" functions
	m_isCb = (name.substr(0, 3) == "CB_");
	m_name = name;
}
void CFunction::AddParameter(CParameter* param)
{
	m_params.push_back(*param);
}

void CParameter::SetType(string type)
{
	m_type += ((m_type.length() > 0 && type.length() > 1) ? " " : "") + type;
}

void CParameter::SetName(string name)
{
	m_name = name;
}

void CParameter::SetCount(string count)
{
	m_count = count;
}

void CParameter::SetRange(string stValue, string edValue)
{
	m_rangeStart = stValue;
	m_rangeEnd = edValue;
}

void CParameter::SetDigits(string digits)
{
	m_digits = digits;
}

/** parsed information data: an RPC class attribute declaration */
void CAttribute::SetName(string name)
{
	m_name = name;
}

void CAttribute::AddField(CParameter *field)
{
	m_fields.push_back(*field);
}

void CAttribute::AddAttribute(CAttribute *attr)
{
	m_childs.push_back(*attr);
}

string CAttribute::GetMasks()
{
	string rstr = m_name + "Mask";

	for (Attributes::iterator pAttr = m_childs.begin();
			pAttr != m_childs.end(); pAttr++) {
		CAttribute &attr = *pAttr;
		rstr += " | " + attr.GetMasks();
	}

	return rstr;
}

bool CAttribute::CheckContain(string name)
{
	if (m_name == name) {
		m_implExist = true;
		return true;
	}

	for (Attributes::iterator pAttr = m_childs.begin();
			pAttr != m_childs.end(); pAttr++) {
		if (pAttr->CheckContain(name))
			return true;
	}

	return false;
}

int CAttribute::NewAttrCount()
{
	int count = 0;

	for (Attributes::iterator pAttr = m_childs.begin();
		pAttr != m_childs.end(); pAttr++)
	{
		count += 1 + pAttr->NewAttrCount();
	}

	return count;
}

/** parsed information data: a RPC class */
CClass::CClass()
{
	m_name = "";
	m_parent = "";
	m_public = true;
	m_block = true;
}

void CClass::SetName(string name)
{
	m_name = name;
}

void CClass::SetParent(string parent)
{
	m_parent = parent;
}

void CClass::AddAttribute(CAttribute *attr)
{
	m_attrs.push_back(*attr);
}

void CClass::AddMethod(CFunction *method)
{
	method->m_public = m_public;
	method->m_block = m_block;
	m_methods.push_back(*method);
}