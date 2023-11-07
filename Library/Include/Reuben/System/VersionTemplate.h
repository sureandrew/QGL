#pragma once
#ifndef _REUBEN_SYSTEM_VERSION_H_
#define _REUBEN_SYSTEM_VERSION_H_

//-- Version Translation

#define RN_VERSION_TRANSLATE(_major_, _minor_, _publish_, _revision_, _build_)		_T(#_major_) _T(".") _T(#_minor_) _T(".") _T(#_publish_) _T(".") _T(#_revision_) _T(".") _T(#_build_)
#define RN_VERSION_TO_STRING(_major_, _minor_, _publish_, _revision_, _build_)		RN_VERSION_TRANSLATE(_major_, _minor_, _publish_, _revision_, _build_)
#define RN_VERSION_TO_NUMBER(_major_, _minor_, _publish_, _revision_)				((UInt32)(0xF0000000 & ((UInt32)_major_ << 28)) | (0x0FF00000 & ((UInt32)_minor_ << 20)) | (0x000F0000 & ((UInt32)_publish_ << 16)) | (0x0000FFFF & (UInt32)_revision_))
#define RN_MARCO_TRANSLATE(_macro_)													_T(#_macro_)
#define RN_MARCO_TO_STRING(_macro_)													RN_MARCO_TRANSLATE(_macro_)

//-- Reuben Version

#define REUBEN_VER_MAJOR		0
#define REUBEN_VER_MINOR		1
#define REUBEN_VER_PUBLISH		0
#define REUBEN_VER_REVISION		$WCREV$
#define REUBEN_VER_BUILD		0

#define REUBEN_VER_MAJOR_STRING		RN_MARCO_TO_STRING(REUBEN_VER_MAJOR)
#define REUBEN_VER_MINOR_STRING		RN_MARCO_TO_STRING(REUBEN_VER_MINOR)
#define REUBEN_VER_PUBLISH_STRING	RN_MARCO_TO_STRING(REUBEN_VER_PUBLISH)
#define REUBEN_VER_REVISION_STRING	RN_MARCO_TO_STRING(REUBEN_VER_REVISION)
#define REUBEN_VER_BUILD_STRING		RN_MARCO_TO_STRING(REUBEN_VER_BUILD)

#define REUBEN_VER_NUMBER	RN_VERSION_TO_NUMBER(REUBEN_VER_MAJOR, REUBEN_VER_MINOR, REUBEN_VER_PUBLISH, REUBEN_VER_REVISION)
#define REUBEN_VER_STRING	RN_VERSION_TO_STRING(REUBEN_VER_MAJOR, REUBEN_VER_MINOR, REUBEN_VER_PUBLISH, REUBEN_VER_REVISION, REUBEN_VER_BUILD)

#endif // _REUBEN_SYSTEM_VERSION_H_
