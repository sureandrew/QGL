FriendPopupNum = -1

function Party_Popup1(pX, pY)
	FriendPopupNum = 0
	Party_Popup(pX, pY)
end

function Party_Popup2(pX, pY)
	FriendPopupNum = 1
	Party_Popup(pX, pY)
end

function Party_Popup3(pX, pY)
	FriendPopupNum = 2
	Party_Popup(pX, pY)
end

function Party_Popup4(pX, pY)
	FriendPopupNum = 3
	Party_Popup(pX, pY)
end

function Party_PopupMe(pX, pY)
	FriendPopupNum = 4
	Party_Popup(pX, pY)
end

function Party_Popup(pX, pY)
	if LuaUI.HasParty() and (LuaUI.IsBattle() ~= true) then
		local partypop = UIManager:GetUIItem("PartyPanel");
		local temp = UIManager:FindUIPanel("PartyPanel");
		if partypop:IsEnable() then
		    PartyPopupClose(0,0);
		else
		    UIManager:SetPanel2Top(temp);
		    LuaUI.ShowPartyPopUp(FriendPopupNum);
			partypop:SetEnable(true);
			partypop:SetPos(pX, pY);
		end
	end
end

function PartyPopup(pX, pY)
	local MyPartyPop = UIManager:FindUIPopUpMenu("PartyPopup")
	local PopSel = MyPartyPop:GetBarPos()

	if FriendPopupNum ~= -1 then
	    LuaUI.SelectPartyPopUp(PopSel, FriendPopupNum);
		--[[if PopSel == 0 then
			LuaUI.PartyPrivateCall(FriendPopupNum);
		elseif PopSel == 1 then
			LuaUI.PartyKick(FriendPopupNum);
		elseif PopSel == 2 then
			LuaUI.PartyPromote(FriendPopupNum);
		elseif PopSel == 3 then
			LuaUI.PartyQuit();
		end ]]
	end
	
	local partypop = UIManager:GetUIItem("PartyPanel")
	FriendPopupNum = -1;
	partypop:SetEnable(false);

	UIManager:RenderAll();
end

function PartyPopupClose(pX, pY)
	local partypop = UIManager:GetUIItem("PartyPanel")
	partypop:SetEnable(false);
	UIManager:AddDirtyUIItem(partypop);
end

function PartyPopupTimeOut(pMouseIn)
	PartyPopupClose(0,0);
end

function StartCount()
	local partypop = UIManager:GetUIItem("PartyPanel")
	partypop:ResetTimer();
	partypop:SetEnableTimeOutEvent(true);
end

function HoldPopup()
	local partypop = UIManager:GetUIItem("PartyPanel")
	partypop:SetEnableTimeOutEvent(false);
end
