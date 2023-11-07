
TargetUnLockItem = -1
TargetUnLockType = -1

function OpenAssetUnLockPanel(index, assetType)
    local panel = UIManager:GetUIItem("AssetUnLockPanel");

	if panel == nil then
	   LuaUI.AddXMLFile2UIManger("Data\\AssetUnLock.xml");
	   panel = UIManager:GetUIItem("AssetUnLockPanel");
	end
	if panel ~= nil then
     	panel:SetEnable(true);
     	SetAssetUnLockFocus()	
	end
	_G.TargetUnLockItem = index	
	_G.TargetUnLockType = assetType
	
end


function CloseAssetUnlockPanel()

	local panel = UIManager:GetUIItem("AssetUnLockPanel");
	if panel ~= nil then
	    UIManager:ClearMonoPanel()
	   	UIManager:DeletePanel("AssetUnLockPanel");
	end
	local panel1 = UIManager:GetUIItem("MiniKeyBoardPanel");

   	if panel1 ~= nill then
	    panel1:SetEnable(false);
    end

	SetEditLineFocus();
	_G.TargetUnLockItem = -1
	_G.TargetUnLockType = -1		
	
end

function ChangeAssetPassword()

	CloseAssetUnlockPanel()
	OpenChangeAssetLockPanel()
end


function ConfirmAssetUnlockPassword()
	local editPassword = UIManager:FindUIEditBox("AssetUnLockPasswordEdit");
	local newPassword = editPassword:GetText();
	if (newPassword ~= nil and newPassword ~= "") then
		local itemProcess = LuaUI.GetItemProcess();
		if itemProcess ~= nil then
			itemProcess:ConfirmAssetUnLockPassword(_G.TargetUnLockItem, _G.TargetUnLockType);
		end
		CloseAssetUnlockPanel()		
	end 
end


function SetAssetUnLockFocus()

	UIManager:SetKeyInItem("AssetUnLockPasswordEdit");
	_G.selectField = 3;

end

function ShowAssetUnlockKeyboard()
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

function SelectAssetUnlockField()
	_G.selectField = 3;
end

function ClearAssetPassword()
	local itemProcess = LuaUI.GetItemProcess();
	if itemProcess ~= nil then
	    itemProcess:ForceResetAssetPassword();
	end
	CloseAssetUnlockPanel();
end