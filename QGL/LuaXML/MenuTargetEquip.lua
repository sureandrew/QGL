TargetEquipIndexNow = -1;
PanelTargetEquipDetailState = 0;
PanelTargetEquipDetailCurNo = -1;
m_TargetEquipDetailX = 0;
m_TargetEquipDetailY = 0;

function OpenTargetEquipMenu()
	local TargetEquipPanel = UIManager:FindUIPanel("TargetEquipPanel")
	
	if TargetEquipPanel ~= nil then
		UIManager:DeletePanel("TargetEquipPanel")
	else
		LuaUI.AddXMLFile2UIManger("Data\\MenuTargetEquip.xml");
		TargetEquipPanel = UIManager:FindUIPanel("TargetEquipPanel");
		if TargetEquipPanel ~= nil then
			TargetEquipPanel:SetPos(UIManager:GetCursorX(), UIManager:GetCursorY() - 120);
			UIManager:AddDirtyUIItem(TargetEquipPanel);
		end
	end
end

function CloseTargetEquipMenu(pX, pY)

	local TargetEquipPanel = UIManager:GetUIItem("TargetEquipPanel");
	if TargetEquipPanel ~= nil then 
		local TargetEquipProcess = LuaUI.GetTargetEquipProcess();
		if TargetEquipProcess ~= nil then
			TargetEquipProcess:FreeTargetEquip();
		end
	
		UIManager:DeletePanel("TargetEquipPanel");
	end
	
end

function RequestTargetEquip()
	local TargetEquipProcess = LuaUI.GetTargetEquipProcess();
	if TargetEquipProcess ~= nil then
		TargetEquipProcess:RequestTargetEquip();
	end
end

function TargetEquipDetail(bagIndex)

	local TargetEquipPanel = UIManager:FindUIPanel("TargetEquipPanel");
	local TargetEquipProcess = LuaUI.GetTargetEquipProcess();
	
	if TargetEquipProcess ~= nil and TargetEquipPanel ~= nil then
		if TargetEquipProcess:HasTargetEquip(bagIndex) then
			local pX = UIManager:GetCursorX();
			local pY = UIManager:GetCursorY();
			local ItemDetailPanel = UIManager:GetUIItem("ItemDetailMenuPanel");

			if ItemDetailPanel == nil then
				LuaUI.AddXMLFile2UIManger("Data\\ItemDetail.xml");
				ItemDetailPanel = UIManager:GetUIItem("ItemDetailMenuPanel")
				
				TargetEquipProcess:UpdateTargetEquipDetail(bagIndex);
		
				local width = ItemDetailPanel:GetWidth();
				local height = ItemDetailPanel:GetHeight();
				
				m_TargetEquipDetailX = pX;
				m_TargetEquipDetailY = pY;
				if m_TargetEquipDetailY + height > 599 then
					m_TargetEquipDetailY = pY - height;
				end
				if m_TargetEquipDetailX + width > 799 then
					m_TargetEquipDetailX = pX - width;
				end
				if m_TargetEquipDetailY < 0 then
					m_TargetEquipDetailY = 0;
				end
				if m_TargetEquipDetailX < 0 then
					m_TargetEquipDetailX = 0;
				end
				ItemDetailPanel:SetPos(m_TargetEquipDetailX, m_TargetEquipDetailY);
				UIManager:AddDirtyUIItem(ItemDetailPanel);
				PanelTargetEquipDetailState = 1;
				PanelTargetEquipDetailCurNo = bagIndex; 
			end
		end
	end
end

function TargetEquipOver000()
	TargetEquipOver(0);
end

function TargetEquipOver001()
	TargetEquipOver(1);
end

function TargetEquipOver002()
	TargetEquipOver(2);
end

function TargetEquipOver003()
	TargetEquipOver(3);
end

function TargetEquipOver004()
	TargetEquipOver(4);
end

function TargetEquipOver005()
	TargetEquipOver(5);
end

function TargetEquipOver006()
	TargetEquipOver(6);
end

function TargetEquipOver007()
	TargetEquipOver(7);
end

function TargetEquipOver008()
	TargetEquipOver(8);
end

function TargetEquipOver(bagIndex)
	local itemName = "";
	itemName = "TargetEquip00"..bagIndex;
	itemName = itemName.."Icon";

	local TargetEquipBoard = UIManager:GetUIItem("TargetEquipBoard");
	TargetEquipBoard:SetEnable(true);
	UIManager:AddDirtyUIItem(TargetEquipBoard);
	
	local item = UIManager:GetUIItem(itemName);
    
	if TargetEquipBoard:GetOffsetX() ~= (item:GetOffsetX() - 1) or TargetEquipBoard:GetOffsetY() ~= (item:GetOffsetY() - 1) then
   		TargetEquipBoard:SetOffsetXY(item:GetOffsetX() - 1 , item:GetOffsetY() - 1 );
		UIManager:AddDirtyUIItem(TargetEquipBoard);
	end

	_G.TargetEquipIndexNow = bagIndex;

	if PanelTargetEquipDetailState == 1 then
		TargetEquipDetail(_G.TargetEquipIndexNow);  
	end
end

function ShowTargetEquipBoard()
	local TargetEquipBoard = UIManager:GetUIItem("TargetEquipBoard");
	local TargetEquipPanel = UIManager:GetUIItem("TargetEquipPanel");
	if TargetEquipBoard ~= nil and TargetEquipPanel ~= nil then
		if (UIManager:GetCursorX() > (TargetEquipBoard:GetOffsetX() + TargetEquipPanel:GetX() ) and UIManager:GetCursorX() < ( TargetEquipBoard:GetOffsetX() + 40 + TargetEquipPanel:GetX() )
			and UIManager:GetCursorY() > (TargetEquipBoard:GetOffsetY() + TargetEquipPanel:GetY() ) and UIManager:GetCursorY() < (TargetEquipBoard:GetOffsetY() + 40 + TargetEquipPanel:GetY() )) then
			--TargetEquipBoard:SetEnable(true);
			if PanelTargetEquipDetailState == 0 then
				TargetEquipDetail(_G.TargetEquipIndexNow);
				PanelTargetEquipDetailState = 1;
			end
		else
		    TargetEquipBoard:SetEnable(false);
		    PanelTargetEquipDetailState = 0;
		end
	
	end
end

function TargetEquipMouseOut()
	local TargetEquipBoard = UIManager:GetUIItem("TargetEquipBoard");
 	TargetEquipBoard:SetEnable(false);
	UIManager:AddDirtyUIItem(TargetEquipBoard);
	CloseItemDetail(0, 0);
end
