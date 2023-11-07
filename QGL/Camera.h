#pragma once
#ifndef CAMERA_H_
#define CAMERA_H_

#include "EntityBase.h"

class CCamera
{
private:
	//The left up conrner pixel position of the camera
	D3DXVECTOR3 m_LeftUpCurrPos;
	//The position move offset after update
	D3DXVECTOR3 m_PosMoveOffset;
	//previous move time in axis-X
	UInt32 m_PreMoveTime;

	//Windows size
	UInt32 m_CurrWinWidth;
	UInt32 m_CurrWinHeight;

	//StlMap size
	UInt32 m_MapWidth;
	UInt32 m_MapHeight;

	//Viewing Target can be Null
	CEntityBase * m_pEntTarget;

	BOOL m_Moved;
	
public:
	//construct and distruct
	CCamera();
	~CCamera();
	//update function by Player position
	VOID UpDateCam(UInt32 CurrTime);

	//set the window size of the camera
	inline VOID SetWindowSize(UInt32 GivenWinWidth, UInt32 GivenWinHeight)
	{
		m_CurrWinWidth = GivenWinWidth;
		m_CurrWinHeight = GivenWinHeight;
	};

	//set the StlMap size of the camera
	inline VOID SetMapSize(UInt32 GivenMapWidth, UInt32 GivenMapHeight)
	{
		m_MapWidth = GivenMapWidth;
		m_MapHeight = GivenMapHeight;
	};

	//set the center position of the camera
	VOID SetCamerLeftUpPos(PixelPos GivenPixelPos);
	VOID SetCamerLeftUpPos(Int PosX, Int PosY);

	//move to target immediately
	VOID MoveToTargetImm();

	//set the entity target to look at
	inline VOID SetViewingTarget(CEntityBase * pGivenEntTarget){ m_pEntTarget =  pGivenEntTarget;};

	inline BOOL GetCameraHadMoved(){ return m_Moved;};
	inline BOOL SetCameraHadMoved(BOOL Value){ return m_Moved = Value;};

	inline D3DXVECTOR3 GetLeftUpPos(){ return m_LeftUpCurrPos;}

	inline CEntityBase * GetViewingTarget(){return m_pEntTarget;}

	inline D3DXVECTOR3 GetPosMoveOffset(){ return m_PosMoveOffset;}

	VOID ResetMoveOffset();
};
#endif