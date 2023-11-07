
_G.assetLockType = -1

function OpenAssetLockPanel()
    local panel = UIManager:GetUIItem("AssetLockPasswordPanel");

	if panel == nil then
	   LuaUI.AddXMLFile2UIManger("Data\\AssetLockPassword.xml");
	   panel = UIManager:GetUIItem("AssetLockPasswordPanel");
	end
	if panel ~= nil then
     	panel:SetEnable(true);
     	InitializeSetAssetLockPanel()
     	SetAssetLockFocus()	
	end
end

function OpenChangeAssetLockPanel()
    local panel = UIManager:GetUIItem("AssetLockPasswordPanel");

	if panel == nil then
	   LuaUI.AddXMLFile2UIManger("Data\\AssetLockPassword.xml");
	   panel = UIManager:GetUIItem("AssetLockPasswordPanel");
	end
	if panel ~= nil then
     	panel:SetEnable(true);
     	SetupChangeAssetLockPanel()
     	SetAssetLockFocus()
        UIManager:ClearMonoPanel()
        UIManager:SetPanel2Top(panel)
     	UIManager:SetMonoPanel(panel);
	end
end

function CloseAssetLockPanel()

	local panel = UIManager:GetUIItem("AssetLockPasswordPanel");
	if panel ~= nil then
	   	UIManager:DeletePanel("AssetLockPasswordPanel");
	end
	local panel1 = UIManager:GetUIItem("MiniKeyBoardPanel");

   	if panel1 ~= nill then
	    panel1:SetEnable(false);
    end

 	_G.assetLockType = -1;	
	SetEditLineFocus();
	
end

function SetupChangeAssetLockPanel()

	ItemMenuSetEnable("AssetLockTitle1", false)
	ItemMenuSetEnable("AssetLockTitle2", true)
	ItemMenuSetEnable("AssetLockInputArea1", false)
	ItemMenuSetEnable("AssetLockInputArea2", true)	
	ItemMenuSetEnable("AssetNewLockPasswordEdit11", false)
	ItemMenuSetEnable("AssetNewLockPasswordEdit12", false)
	ItemMenuSetEnable("AssetOldLockPasswordEdit", true)
	ItemMenuSetEnable("AssetNewLockPasswordEdit21", true)
	ItemMenuSetEnable("AssetNewLockPasswordEdit22", true)
	_G.assetLockType = 1

end
function InitializeSetAssetLockPanel()

	ItemMenuSetEnable("AssetLockTitle1", true)
	ItemMenuSetEnable("AssetLockTitle2", false)
	ItemMenuSetEnable("AssetLockInputArea1", true)
	ItemMenuSetEnable("AssetLockInputArea2", false)	
	ItemMenuSetEnable("AssetNewLockPasswordEdit11", true)
	ItemMenuSetEnable("AssetNewLockPasswordEdit12", true)
	ItemMenuSetEnable("AssetOldLockPasswordEdit", false)
	ItemMenuSetEnable("AssetNewLockPasswordEdit21", false)
	ItemMenuSetEnable("AssetNewLockPasswordEdit22", false)
	_G.assetLockType = 0

end


function ConfirmAssetLockPassword()
	if _G.assetLockType == 0 then
		local editPassword1 = UIManager:FindUIEditBox("AssetNewLockPasswordEdit11");
		local editPassword2 = UIManager:FindUIEditBox("AssetNewLockPasswordEdit12");
		local newPassword1 = editPassword1:GetText();
		local newPassword2 = editPassword2:GetText();
		if ((newPassword1 ~= nil and newPassword1 ~= "") and (newPassword2 ~= nil and newPassword2 ~= "")) then
			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ConfirmAssetLockPassword(_G.assetLockType);
			end
			CloseAssetLockPanel()		
		end 
	elseif _G.assetLockType == 1 then
		local editPassword1 = UIManager:FindUIEditBox("AssetOldLockPasswordEdit");
		local editPassword2 = UIManager:FindUIEditBox("AssetNewLockPasswordEdit21");
		local editPassword3 = UIManager:FindUIEditBox("AssetNewLockPasswordEdit22");	
		local oldPassword = editPassword1:GetText();
		local newPassword1 = editPassword2:GetText();
		local newPassword2 = editPassword3:GetText();
		if ((oldPassword ~= nil and oldPassword ~= "") and (newPassword1 ~= nil and newPassword1 ~= "") and 
			(newPassword2 ~= nil and newPassword2 ~= "")) then
			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ConfirmAssetLockPassword(_G.assetLockType);
			end
			CloseAssetLockPanel()		
		end 	
	end
end


function SetAssetLockFocus()

	if _G.assetLockType == 0 then
		UIManager:SetKeyInItem("AssetNewLockPasswordEdit11");
		_G.selectField = 4;
	elseif _G.assetLockType == 1 then
	    UIManager:SetKeyInItem("AssetOldLockPasswordEdit");
	    _G.selectField = 6;
	end 

end

function SelectAssetUnlockField11()
	_G.selectField = 4;
end


function SelectAssetUnlockField12()
	_G.selectField = 5;
end


function SelectAssetUnlockField21()
	_G.selectField = 6;
end


function SelectAssetUnlockField22()
	_G.selectField = 7;
end


function SelectAssetUnlockField23()
	_G.selectField = 8;
end


function ShowAssetLockKeyboard()
	local panel = UIManager:GetUIItem("MiniKeyBoardPanel");
   	if panel ~= nill then
        if _G.keyboardFlag == 0 then
            panel:SetEnable(true);
            _G.keyboardFlag = 1;
		else
		    panel:SetEnable(false);
		    _G.keyboardFlag = 0;
		end
	else
	    LuaUI.AddXMLFile2UIManger("Data\\MiniKeyboard.xml");
	    _G.keyboardFlag = 1;
    end
   UIManager:RenderAll();
end

function AssetChangeEditBox1(KeyCode)
    if KeyCode == LuaUI.VK_TAB then
        if _G.selectField == 4 then
            SelectAssetUnlockField12();
        else
            SelectAssetUnlockField11();
        end
    end
end

function AssetChangeEditBox2(KeyCode)
    if KeyCode == LuaUI.VK_TAB then
        if _G.selectField == 6 then
            SelectAssetUnlockField22();
        elseif _G.selectField == 7 then
            SelectAssetUnlockField23();
        elseif _G.selectField == 8 then
            SelectAssetUnlockField21();
        end
    end
end