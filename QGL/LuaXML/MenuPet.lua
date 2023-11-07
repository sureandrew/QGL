local mouseOverSKillIndex = -1;

function OpenPet()
	local panel = UIManager:GetUIItem("PetPanel");
	
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\MenuPet.xml");
	    local Menu = LuaUI.GetMenuPetProcess();
	    Menu:RefreshMenu();
	else
	    ClosePet();
	end
end

function ClosePet()
	UIManager:StopDrag();
	UIManager:DeletePanel("PetPanel");
	ClosePetUseFood()
end


function OpenInfoPanel()
	local panel = UIManager:GetUIItem("InfoPanel");
	
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\MenuInfo.xml");
	else
	    CloseInfoPanel();
	end
end

function CloseInfoPanel()
	UIManager:DeletePanel("InfoPanel");
end


function PetAddLink(Index)
	if LuaUI.CheckShiftDown() then
		local EditBox = UIManager:FindUIEditBox("EditLine")	
		
		if EditBox ~= nil then
			EditBox:ClearWaitText()
			EditBox:AddWaitText(string.format("\15#<#V\\C1\\[PETS:%x:%x\\]\16\17[", 
				LuaUI.LuaGetPetID(Index), LuaUI.LuaGetChar_id()))
			EditBox:AddWaitText(LuaUI.LuaGetPetOriginName(Index))
			--EditBox:AddWaitText(":")
			--EditBox:AddWaitText(LuaUI.LuaGetLineTextName())
			EditBox:AddWaitText("]\20\18")
			EditBox:SendWaitText()
			
			EditBox:Redraw();
			UIManager:RenderAll();
		end
		
	end
end



function SelectPet1()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:SetSelIndex(0);
	PetAddLink(Menu:GetPetSelectIndex())
end

function SelectPet2()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:SetSelIndex(1);
	PetAddLink(Menu:GetPetSelectIndex())
end

function SelectPet3()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:SetSelIndex(2);
	PetAddLink(Menu:GetPetSelectIndex())
end

function SelectPet4()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:SetSelIndex(3);
	PetAddLink(Menu:GetPetSelectIndex())
end

function SelectPet5()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:SetSelIndex(4);
	PetAddLink(Menu:GetPetSelectIndex())
end

function PetScroll()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:Scroll();
end

function PetScrollUp()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:PageUp();
end

function PetScrollDown()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:PageDown();
end

function PetUse()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:UsePet();
end

function PetRest()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:PetRest();
end

function PetDump()
	local Menu = LuaUI.GetMenuPetProcess();
	Menu:DumpPet();
end

function PetChangeNameLua()
end

function PetTrainLua()
end

function OpenChangePetName()
	local petMenu = LuaUI.GetMenuPetProcess();
	if petMenu:GetPetSelectIndex() == -1  or petMenu:HasPet() == false then
		return;
	end
	local panel = UIManager:GetUIItem("PetRenamePanel");
	local petPanel = UIManager:GetUIItem("PetPanel");
	
	if panel == nil and petPanel ~= nil then
		LuaUI.AddXMLFile2UIManger("Data\\PetRename.xml");
		panel = UIManager:GetUIItem("PetRenamePanel");
		if panel ~= nil then
		    panel:SetPos(petPanel:GetX() + 70, petPanel:GetY() + 200);
		end
	end
end

function CloseChangePetName()
	ResetKeyIn();
    UIManager:DeletePanel("PetRenamePanel");
end

function ChanegPetName()
	local petRenameText = UIManager:FindUIEditBox("PetNewName");
	
	if petRenameText ~= nil then
		if petRenameText:Empty() ~= true then
			local petMenu = LuaUI.GetMenuPetProcess();
			if petMenu ~= nil then 
				petMenu:ChangePetName();
			end
			--LuaUI.ChangePetName();
			CloseChangePetName();
		end
	end
end

function OpenConfirmDropPet()
	local petMenu = LuaUI.GetMenuPetProcess();
	if petMenu:GetPetSelectIndex() == -1 or petMenu:HasPet() == false then
		return;
	end
	LuaUI.AddXMLFile2UIManger("Data\\ConfirmDropPet.xml");
	local petPanel = UIManager:GetUIItem("PetPanel");
	local panel = UIManager:FindUIPanel("ConfirmDropPetPanel");
	if panel ~= nil then
	    panel:SetPos(petPanel:GetX() + 70, petPanel:GetY() + 200);
	    local petMenu = LuaUI.GetMenuPetProcess();
		if petMenu ~= nil then 
			petMenu:ShowRemovePetName();
		end
	    UIManager:SetMonoPanel(panel);
	end
end

function CloseConfirmDropPet()
    local panel = UIManager:FindUIPanel("ConfirmDropPetPanel");
	if panel ~= nil then
		UIManager:ClearMonoPanel();
		UIManager:DeletePanel("ConfirmDropPetPanel");
	end
end

function ConfirmDropPet()
    --LuaUI.DeleteOnShowPet();
    local petMenu = LuaUI.GetMenuPetProcess();
	if petMenu ~= nil then 
		petMenu:DumpPet();
	end
    CloseConfirmDropPet();
end

function CheckMouseOverSkillIcon()
	mouseOverSKillIndex = -1;
	for i =0, 7 do
		local msg = string.format("PetSkill%d", i);
		if UIManager:Detect(msg, UIManager:GetCursorX(), UIManager:GetCursorY()) then
			mouseOverSKillIndex = i;
			break;
		end
	end
	
	if mouseOverSKillIndex ~= -1 then
		UIManager:GetUIItem("PetPanel"):SetEnableTimeOutEvent(true);
	else
		UIManager:GetUIItem("PetPanel"):SetEnableTimeOutEvent(false);
		local petMenu = LuaUI.GetMenuPetProcess();
		if petMenu ~= nil then 
			petMenu:ClosePetSkillDetail();
		end
	end
end
function PetSkillDetailPopupTimeOut()
	local petMenu = LuaUI.GetMenuPetProcess();
	if petMenu ~= nil then 
		petMenu:OpenPetSkillDetail(mouseOverSKillIndex);
	end
	UIManager:GetUIItem("PetPanel"):SetEnableTimeOutEvent(false);
end

function ClosePetDetail()
	local panel = UIManager:GetUIItem("PetDetailPanel");
	if panel ~= nil then 
		UIManager:DeletePanel("PetDetailPanel");
	end
end

function PetPanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("PetScrollBar"):OnMouseWheel(UIManager:GetUIItem("PetScrollBar"):GetX()
		,UIManager:GetUIItem("PetScrollBar"):GetY(),pZ);
end

function OpenUsePetFood()
	local petMenu = LuaUI.GetMenuPetProcess();
	if petMenu ~= nil then
		if petMenu:GetPetSelectIndex() == -1  or petMenu:HasPet() == false or  petMenu:CanEatFood() == false then
			return;		
		end
	end
	local panel = UIManager:GetUIItem("PetUseFoodPanel");
	local petPanel = UIManager:GetUIItem("PetPanel");
	
	if panel == nil and petPanel ~= nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuPetUseFood.xml");
		panel = UIManager:GetUIItem("PetUseFoodPanel");
		if panel ~= nil and petMenu ~=nil then
		    panel:SetPos(petPanel:GetX() + 70, petPanel:GetY() + 200);
			petMenu:InitUsePetFoodPanel();
		end
	end
	
end

function ClosePetUseFood()
	local petMenu = LuaUI.GetMenuPetProcess();
	if petMenu ~= nil then
		petMenu:ReSetFoodUseItem();
	end
	local panel = UIManager:FindUIPanel("PetUseFoodPanel");
	if panel ~= nil then
		UIManager:ClearMonoPanel();
		UIManager:DeletePanel("PetUseFoodPanel");
	end
end

function PetUseFoodItem()
	local petMenu = LuaUI.GetMenuPetProcess();
	if petMenu ~= nil then
		petMenu:PetUseFood();
	end
end
