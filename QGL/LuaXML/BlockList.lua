function OpenBlockLstWindow(pX, pY)

	if _G.CloseAllPanel ~= true then
		
		local panel = UIManager:GetUIItem("MessagerPanel");
		if panel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\MenuFriendList.xml");
			BlackListGroupLua();
		else
			if panel:IsEnable() then
				BlackListGroupLua();
			else
				panel:SetEnable(true);
				BlackListGroupLua();
			end
		end
	end
end

function AddCharToBlockList()
end

function RemoveCharFromBlockList()
end

function ClearBlockList()
end

function CloseBlockList(pX, pY)
	UIManager:StopDrag();
	UIManager:SetEvent(105);
end