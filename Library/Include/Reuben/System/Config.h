#pragma once
#ifndef _REUBEN_SYSTEM_CONFIG_H_
#define _REUBEN_SYSTEM_CONFIG_H_

//--Config Setting Here
#define REUBEN_CONFIG_TRACE_ENABLE						0x01
#define REUBEN_CONFIG_GRAPHIC_ENABLE					0x02
#define REUBEN_CONFIG_MEMORY_MANAGE_ENABLE				0x04
#define REUBEN_CONFIG_MEMORY_DEBUG_CHECK_ENABLE			0x08
//#define REUBEN_CONFIG_NETWORK_PACKET_COUNTER_ENABLE		0x10
#define REUBEN_CONFIG_NETWORK_CRC_CHECK_ENABLE			0x20

//-- Base on config identify the marco symbol
#include "Option.h"

//-- Base on config to include library
#include "Suffix.h"

#endif // _REUBEN_SYSTEM_CONFIG_H_
