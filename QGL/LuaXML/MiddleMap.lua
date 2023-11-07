PrevMidMapX = 0;
PrevMidMapY = 0;

bMidMapShowNpc = true;
bMidMapShowBusinessman = true;
bMidMapShowQuestNpc = true;

function ShowMidMap()

	if LuaUI.IsIndoor() ~= true and LuaUI.IsTalking() ~= true and _G.InBattle ~= true then
		local panel = UIManager:GetUIItem("MidMapPanel");

		if panel ~= nil then
			CloseMiddleMap();
		else
			LuaUI.AddXMLFile2UIManger("Data\\MiddleMap.xml");
			
			local Menu = LuaUI.GetMenuHelpProcess();
			Menu:HandleEvent(24);

			local panel2 = UIManager:GetUIItem("MidMapPanel");
			if panel2 ~= nil then
				LuaUI.ShowMidMap();
			end
			PlayOpenInterfaceEffect();
		end
	elseif LuaUI.IsIndoor() == true then
	    LuaUI.ShowErrMessage("MSG_MIDDLE_MAP_NO_MAP");
	end
end

function MoveCharacterTo()
	local panel = UIManager:GetUIItem("MidMapPanel");
	local locationBoxPanel = UIManager:FindUIPanel("LocationBoxPanel");
	
	if panel ~= nil and locationBoxPanel ~= nil and LuaUI.IsTalking() ~= true   then
		local diffX = 0;
		local diffY = 0;
		
		if panel:GetX() > PrevMidMapX then
			diffX = panel:GetX() - PrevMidMapX;
		else
			diffX = PrevMidMapX - panel:GetX();
		end
		
		if panel:GetY() > PrevMidMapY then
			diffY = panel:GetY() - PrevMidMapY;
		else
			diffY = PrevMidMapY - panel:GetY();
		end
		
		
		if diffX < 10 and diffY < 10 then
			LuaUI.MidMapMoveCharTo(0);	
		end	
		UIManager:SetPanel2Top(locationBoxPanel);
	end
	

end

function DragMidMap()
	
	local panel = UIManager:GetUIItem("MidMapPanel");
	local locationPanel = UIManager:GetUIItem("LocationBoxPanel");
	
	if panel ~= nil and locationPanel ~= nil then
		PrevMidMapX = panel:GetX();
		PrevMidMapY = panel:GetY();
		UIManager:SetDragItemXY(panel,panel:GetX()- UIManager:GetCursorX() ,  panel:GetY()- UIManager:GetCursorY());
		locationPanel:SetEnable(false);
		
	end
	
end

function CloseMiddleMap()
	LuaUI.ResetMidMapValue();
	UIManager:StopDrag();
	local panel2 = UIManager:GetUIItem("LocationBoxPanel");
	if panel2 ~= nil then
		UIManager:DeletePanel("LocationBoxPanel");
	end
	
	local panel = UIManager:GetUIItem("MidMapPanel");
	
	if panel ~= nil then
		PlayCloseInterfaceEffect();
		UIManager:DeletePanel("MidMapPanel");
	end
	
	

end

function MidMapNpcOver()

	LuaUI.MidMapShowName(0);	

end

function MidMapNpcOut()
	LuaUI.MidMapMoveOut();	
end

function OutMidMap()
	local locationBoxPanel = UIManager:GetUIItem("LocationBoxPanel");
		
	if locationBoxPanel ~= nil  then
		locationBoxPanel:SetEnable(false);
	end
end

function InMidMap()
	local locationBoxPanel = UIManager:GetUIItem("LocationBoxPanel");
		
	if locationBoxPanel ~= nil  then
		locationBoxPanel:SetEnable(true);
	end
end

function MidMapJumpPointOver()
	LuaUI.MidMapShowName(1);
end

function MidMapJumpPointOut()
 	LuaUI.MidMapMoveOut();
end

function MidMapJumpClick()
	if LuaUI.IsTalking() ~= true then
		LuaUI.MidMapMoveCharTo(1);
	end
end

function MidMapClickNPC()
	if LuaUI.IsTalking() ~= true then
		LuaUI.MidMapMoveCharTo(3);  	
	end
end

function OpenSearchNPCList()
	local panel = UIManager:GetUIItem("NPCSearchPanel");
	local partyPanel = UIManager:GetUIItem("PartySearchPanel");
	local partySelectPanel = UIManager:GetUIItem("PartyInfoSelectPanel");
	
	if partyPanel ~= nil then
		partyPanel:SetEnable(false);
	end
	if partySelectPanel ~= nil then
		partySelectPanel:SetEnable(false);
	end
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\NPCSearch.xml");
		ClearNPCList();
  		LuaUI.UpdateSearchList(0);
		UIManager:SetKeyInItem("SearchChiTextBox");
		PlayOpenInterfaceEffect();
	else
		CloseNPCSearch();
	end
end 

function CloseNPCSearch()
	
	local panel = UIManager:GetUIItem("NPCSearchPanel");
	
	if panel ~= nil then
		--OutNPCSearch();
		LuaUI.ResetSearchValue();
		ResetKeyIn();
		UIManager:DeletePanel("NPCSearchPanel");
		UIManager:DeletePanel("SearchPartyPanel");
		UIManager:DeletePanel("PartyInfoSelectPanel");
		PlayCloseInterfaceEffect();
	end

	local partyPanel = UIManager:GetUIItem("PartySearchPanel");

	if partyPanel ~= nil  then
		UIManager:DeletePanel("PartySearchPanel");
		PlayCloseInterfaceEffect();
	end

	local charPanel = UIManager:GetUIItem("CharSearchPanel");
	if charPanel ~= nil then
		UIManager:DeletePanel("CharSearchPanel");
		PlayCloseInterfaceEffect();
	end
end 

function ClearNPCList()

	for i = 1, 17 do
		local text = UIManager:FindUIStaticText("NPCName"..i);
		if text ~= nil then
			text:LuaSetText("");
			text:Redraw();
		end
		
		text = UIManager:FindUIStaticText("NPCNamePos"..i);
        if text ~= nil then
			text:LuaSetText("");
			text:Redraw();
		end
		
		local button = UIManager:GetUIItem("NPCNameHighLight"..i);
		if button ~= nil then
			button:SetEnable(false);
		end
	end
	
	button = UIManager:GetUIItem("NPCNameHighLight");
		if button ~= nil then
			button:SetEnable(false);
		end
		
	button = UIManager:FindUIScrollBarV("NPCSearchScrollBar");
	if button ~= nil then
	    button:HomeScroll();
	end
	
end

function NPCSearchScrollUp()
	LuaUI.NPCSearchListUpOrDown(0);
end

function NPCSearchScrollDown()
	LuaUI.NPCSearchListUpOrDown(1);
end

function ShowLocation()
	LuaUI.MidMapShowLocationBox();
end 

function OutNPCSearch()
	--[[local locationBoxPanel = UIManager:GetUIItem("LocationBoxPanel");
		
	if locationBoxPanel ~= nil  then
		locationBoxPanel:SetEnable(false);
		LuaUI.MidMapMoveOut();	
	end
	]]
end



function NPCSearchClick()
    UIManager:SetKeyInItem("SearchTextBox");
	local panel = UIManager:GetUIItem("NPCSearchPanel");
	
	if panel ~= nil then 
		local relY = UIManager:GetCursorY() - panel:GetY();
		
		if relY >= 52 and relY < 68 then
		    LuaUI.NPCSearchClickList(1);
		elseif relY >= 68 and relY < 84 then
			LuaUI.NPCSearchClickList(2);
		elseif relY >= 84 and relY < 100 then
		    LuaUI.NPCSearchClickList(3);
		elseif relY >= 100 and relY < 116 then
			LuaUI.NPCSearchClickList(4);
		elseif relY >= 116 and relY < 132 then
			LuaUI.NPCSearchClickList(5);
		elseif relY >= 132 and relY < 148 then
			LuaUI.NPCSearchClickList(6);
		elseif relY >= 148 and relY < 164 then
			LuaUI.NPCSearchClickList(7);
		elseif relY >= 164 and relY < 180 then
			LuaUI.NPCSearchClickList(8);
		elseif relY >= 180 and relY < 196 then
			LuaUI.NPCSearchClickList(9);
		elseif relY >= 196 and relY < 213 then
			LuaUI.NPCSearchClickList(10);
		elseif relY >= 213 and relY < 228 then
			LuaUI.NPCSearchClickList(11);
		elseif relY >= 228 and relY < 244 then
			LuaUI.NPCSearchClickList(12);
		elseif relY >= 244 and relY < 260 then
			LuaUI.NPCSearchClickList(13);
		elseif relY >= 260 and relY < 276 then
			LuaUI.NPCSearchClickList(14);
		elseif relY >= 276 and relY < 292 then
			LuaUI.NPCSearchClickList(15);
		elseif relY >= 292 and relY < 308 then
			LuaUI.NPCSearchClickList(16);
		elseif relY >= 308 and relY < 327 then
			LuaUI.NPCSearchClickList(17);
		end
		
	end

end

function PartyInfoSearchClick()
	local panel = UIManager:GetUIItem("PartySearchPanel");
	
	if panel ~= nil then 
		local relY = UIManager:GetCursorY() - panel:GetY();
		
		if relY >= 70 and relY < 93 then
		    LuaUI.PartySearchClickList(1);
		elseif relY >= 93 and relY < 116 then
			LuaUI.PartySearchClickList(2);
		elseif relY >= 116 and relY < 139 then
		    LuaUI.PartySearchClickList(3);
		elseif relY >= 139 and relY < 162 then
			LuaUI.PartySearchClickList(4);
		elseif relY >= 162 and relY < 185 then
			LuaUI.PartySearchClickList(5);
		elseif relY >= 185 and relY < 208 then
			LuaUI.PartySearchClickList(6);
		elseif relY >= 208 and relY < 231 then
			LuaUI.PartySearchClickList(7);
		elseif relY >= 232 and relY < 254 then
			LuaUI.PartySearchClickList(8);
		elseif relY >= 254 and relY < 277 then
			LuaUI.PartySearchClickList(9);
		elseif relY >= 277 and relY < 300 then
			LuaUI.PartySearchClickList(10);
		elseif relY >= 300 and relY < 323 then
			LuaUI.PartySearchClickList(11);
		elseif relY >= 323 and relY < 346 then
			LuaUI.PartySearchClickList(12);
		end
		
	end

end

function ShowLeaderInfo()
	local panel = UIManager:GetUIItem("PartySearchPanel");
	
	if panel ~= nil then 
		LuaUI.ShowLeaderInfo();
	end

end

function SetCharacterMove()
	if LuaUI.IsTalking() ~= true then
		LuaUI.MidMapMoveCharTo(2); 
	end
end

function SearchNPCList()
	LuaUI.SearchNPCList(0);

end

function SearchNPCListInChi()
    LuaUI.SearchNPCList(1);
end

function ChangeKeyInToChi()
    UIManager:SetKeyInItem("SearchChiTextBox");
end

function ShowNPCList()
	local npcList = UIManager:FindUICheckBox("ShowNPCList");
	local charList = UIManager:FindUICheckBox("ShowCharList");
    local moveButton = UIManager:GetUIItem("SetCharacterMove");
	local updateButton = UIManager:GetUIItem("SetCharacterUpdate");
	
	if npcList ~= nil and charList ~= nil and
		moveButton ~= nil and updateButton ~= nil then

		charList:SetState(false);
		npcList:SetState(true);
		ClearNPCList();
		LuaUI.ResetSearchValue();
		LuaUI.UpdateSearchList(0);
		
		moveButton:SetEnable(true);
  		updateButton:SetEnable(false);
		
		local partyList = UIManager:FindUICheckBox("ShowPartyList");
		if partyList ~= nil then
			partyList:SetState(false);
		end
	end
end

function ShowCharList()
	local charPanel = UIManager:GetUIItem("CharSearchPanel");
    local npcPanel = UIManager:GetUIItem("NPCSearchPanel");

	if charPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\CharSearch.xml");
		charPanel = UIManager:GetUIItem("CharSearchPanel");
		local charSelectPanel = UIManager:GetUIItem("CharInfoSelectPanel");
		if charSelectPanel ~= nil then
			charSelectPanel:SetEnable(false);
		end
	end
	
	if charPanel ~= nil and npcPanel ~= nil then
		charPanel:SetPos( npcPanel:GetX(), npcPanel:GetY());
		charPanel:SetEnable(true);
		npcPanel:SetEnable(false);
		local npcList = UIManager:FindUICheckBox("ShowNPCList3");
		local partyList = UIManager:FindUICheckBox("ShowPartyList3");
		local charList = UIManager:FindUICheckBox("ShowCharList3");
		if npcList ~= nil and partyList ~= nil and charList ~= nil then
			partyList:SetState(false);
			npcList:SetState(false);
			charList:SetState(true);
		end
		ShowCharList3();
		LuaUI.OnShowCharInfo();
	end
end

function ShowPartyList()	
	local partyPanel = UIManager:GetUIItem("PartySearchPanel");
    local npcPanel = UIManager:GetUIItem("NPCSearchPanel");

	if partyPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\PartySearch.xml");		
		partyPanel = UIManager:GetUIItem("PartySearchPanel");
		local partySelectPanel = UIManager:GetUIItem("PartyInfoSelectPanel");
		if partySelectPanel ~= nil then
			partySelectPanel:SetEnable(false);
		end
	end
	
	if partyPanel ~= nil and npcPanel ~= nil then
		partyPanel:SetPos( npcPanel:GetX(), npcPanel:GetY());
		partyPanel:SetEnable(true);
		npcPanel:SetEnable(false);
		local npcList = UIManager:FindUICheckBox("ShowNPCList2");
		local charList = UIManager:FindUICheckBox("ShowCharList2");
		local partyList = UIManager:FindUICheckBox("ShowPartyList2");
		if npcList ~= nil and charList ~= nil and partyList ~= nil then
			charList:SetState(false);
			npcList:SetState(false);
			partyList:SetState(true);
		end
		ShowPartyList2();
		LuaUI.OnShowPartyInfo();
	end
end

function ShowNPCList2()
	local partyPanel = UIManager:GetUIItem("PartySearchPanel");
    local npcPanel = UIManager:GetUIItem("NPCSearchPanel");
	if partyPanel ~= nil and npcPanel ~= nil then
		npcPanel:SetPos( partyPanel:GetX(), partyPanel:GetY());
		npcPanel:SetEnable(true);
		partyPanel:SetEnable(false);
		ShowNPCList();
	end
    local partySelectPanel = UIManager:GetUIItem("PartyInfoSelectPanel");
	if partySelectPanel ~= nil then
		partySelectPanel:SetEnable(false);
	end
end

function ShowCharList2()
	local charPanel = UIManager:GetUIItem("CharSearchPanel");
    local partyPanel = UIManager:GetUIItem("PartySearchPanel");

	if charPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\CharSearch.xml");
		charPanel = UIManager:GetUIItem("CharSearchPanel");
		local charSelectPanel = UIManager:GetUIItem("CharInfoSelectPanel");
		if charSelectPanel ~= nil then
			charSelectPanel:SetEnable(false);
		end
	end

	if charPanel ~= nil and partyPanel ~= nil then
		charPanel:SetPos( partyPanel:GetX(), partyPanel:GetY());
		charPanel:SetEnable(true);
		partyPanel:SetEnable(false);
		local partyList = UIManager:FindUICheckBox("ShowPartyList3");
		local npcList = UIManager:FindUICheckBox("ShowNPCList3");
		local charList = UIManager:FindUICheckBox("ShowCharList3");
		if partyList ~= nil and npcList ~= nil and charList ~= nil then
			npcList:SetState(false);
			partyList:SetState(false);
			charList:SetState(true);
		end
		ShowCharList3();
		LuaUI.OnShowCharInfo();
	end
end

function ClosePartyInfoSelectPanel()
    local partySelectPanel = UIManager:GetUIItem("PartyInfoSelectPanel");
	if partySelectPanel ~= nil then
		for i = 1, 12 do
			local button = UIManager:GetUIItem("PartyInfoHighLight"..i);
			if button ~= nil then
				button:SetEnable(true);
			end
		end
		partySelectPanel:SetEnable(false);
	end
end

function ShowPartyList2()
	local buttonHighLight = UIManager:GetUIItem("PartyInfoHighLight");
	if buttonHighLight ~= nil then
		buttonHighLight:SetEnable(false);
	end
	for i = 1, 12 do
		local button = UIManager:GetUIItem("PartyInfoHighLight"..i);
		if button ~= nil then
			button:SetEnable(true);
		end
	end
end

function NPCSearchScroll()
	LuaUI.NPCSearchListScroll();
end

function ShowSearchNPC()
	OpenSearchNPCList();
end

function SetCharacterUpdate()
    LuaUI.UpdateSearchList(1);
end

function MidMapShowNormalNpc()
	local button = UIManager:FindUIButton("MidMapNormalNpc");
 	if button ~= nil then
		if _G.bMidMapShowNpc then
		    _G.bMidMapShowNpc = false;
			button:SetClickState();
			button:SetLockState(true);
			
		else
		    _G.bMidMapShowNpc = true;
		    button:SetLockState(false);
		    button:SetNormalState();
		end
	    MidMapShowNpc("NormalNpcIcon", _G.bMidMapShowNpc);
    end
end

function MidMapShowBusinessman()
    local button = UIManager:FindUIButton("MidMapBusinessman");
 	if button ~= nil then
		if _G.bMidMapShowBusinessman then
		    _G.bMidMapShowBusinessman = false;
			button:SetClickState();
			button:SetLockState(true);

		else
		    _G.bMidMapShowBusinessman = true;
		    button:SetLockState(false);
		    button:SetNormalState();
		end
	    MidMapShowNpc("BusinessmanNpcIcon", _G.bMidMapShowBusinessman);
    end
end

function MidMapShowQuestNpc()
    local button = UIManager:FindUIButton("MidMapQuestNpc");
 	if button ~= nil then
		if _G.bMidMapShowQuestNpc then
		    _G.bMidMapShowQuestNpc = false;
			button:SetClickState();
			button:SetLockState(true);

		else
		    _G.bMidMapShowQuestNpc = true;
		    button:SetLockState(false);
		    button:SetNormalState();
		end
	    MidMapShowNpc("QuestNpcIcon", _G.bMidMapShowQuestNpc);
    end
end

function MidMapShowNpc(buttonName, show)
    local i = 0;
	local button = UIManager:GetUIItem(buttonName..i);
	while button ~= nil do
	    if show then
	        button:SetEnable(true);
		else
		    button:SetEnable(false);
		end
		i = i + 1;
		button = UIManager:GetUIItem(buttonName..i);
	end
end

function SearchPartyPopupClose()
	local panel = UIManager:GetUIItem("SearchPartyPanel");
	if panel ~= nil then
	    panel:SetEnable(false);
	    panel:SetEnableTimeOutEvent(false);
	end
end

function StartSearchPartyCloseCount()
	local panel = UIManager:GetUIItem("SearchPartyPanel")
	if panel ~= nil then
		panel:ResetTimer();
		panel:SetEnableTimeOutEvent(true);
	end
end

function ShowSearchPartyPopUp()
    local panel = UIManager:GetUIItem("SearchPartyPanel");
    local charList = UIManager:FindUICheckBox("ShowCharList");
	if panel ~= nil and charList then
	    NPCSearchClick();
	    if charList:GetState() then
	    	panel:SetEnable(false);
	    end
	end
end

function NPCSearchPanelWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("NPCSearchScrollBar"):OnMouseWheel(UIManager:GetUIItem("NPCSearchScrollBar"):GetX()
	,UIManager:GetUIItem("NPCSearchScrollBar"):GetY(),pZ);
end

function PartyInfoDescClick()
	LuaUI.ShowPartySearchList(1);
	local pPanel = UIManager:GetUIItem("PartySearchPanel");
	if pPanel == nil then return end
	
	local pCheckBox = UIManager:FindUICheckBox("PartyInfoDesc");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCount");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
end

function PartyInfoFactionClick()
	LuaUI.ShowPartySearchList(2);
	local pPanel = UIManager:GetUIItem("PartySearchPanel");
	if pPanel == nil then return end
	
	local pCheckBox = UIManager:FindUICheckBox("PartyInfoDesc");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCount");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
end

function PartyInfoCclassClick()
	LuaUI.ShowPartySearchList(3);
	local pPanel = UIManager:GetUIItem("PartySearchPanel");
	if pPanel == nil then return end
	
	local pCheckBox = UIManager:FindUICheckBox("PartyInfoDesc");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCount");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
end

function PartyInfoLevelClick()
	LuaUI.ShowPartySearchList(4);
	local pPanel = UIManager:GetUIItem("PartySearchPanel");
	if pPanel == nil then return end
	
	local pCheckBox = UIManager:FindUICheckBox("PartyInfoDesc");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCount");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
end

function PartyInfoCountClick()
	LuaUI.ShowPartySearchList(5);
	local pPanel = UIManager:GetUIItem("PartySearchPanel");
	if pPanel == nil then return end
	
	local pCheckBox = UIManager:FindUICheckBox("PartyInfoDesc");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("PartyInfoCount");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
end

function OnPartySearchPagePrev()
	LuaUI.OnPartySearchPagePrev();
end

function OnPartySearchPageNext()
	LuaUI.OnPartySearchPageNext();
end

function OnJoinPartySearch()
	LuaUI.OnJoinPartySearch();
end

--CharSearch
function ShowNPCList3()
	local charPanel = UIManager:GetUIItem("CharSearchPanel");
    local npcPanel = UIManager:GetUIItem("NPCSearchPanel");
	if charPanel ~= nil and npcPanel ~= nil then
		npcPanel:SetPos( charPanel:GetX(), charPanel:GetY());
		npcPanel:SetEnable(true);
		charPanel:SetEnable(false);
		ShowNPCList();
	end
    local charSelectPanel = UIManager:GetUIItem("CharInfoSelectPanel");
	if charSelectPanel ~= nil then
		charSelectPanel:SetEnable(false);
	end
end

function ShowCharList3()
	local buttonHighLight = UIManager:GetUIItem("CharInfoHighLight");
	if buttonHighLight ~= nil then
		buttonHighLight:SetEnable(false);
	end
	for i = 1, 12 do
		local button = UIManager:GetUIItem("CharInfoHighLight"..i);
		if button ~= nil then
			button:SetEnable(true);
		end
	end
end

function CloseCharInfoSelectPanel()
    local charSelectPanel = UIManager:GetUIItem("CharInfoSelectPanel");
	if charSelectPanel ~= nil then
		for i = 1, 12 do
			local button = UIManager:GetUIItem("CharInfoHighLight"..i);
			if button ~= nil then
				button:SetEnable(true);
			end
		end
		charSelectPanel:SetEnable(false);
	end
end

function ShowPartyList3()
	local partyPanel = UIManager:GetUIItem("PartySearchPanel");
    local charPanel = UIManager:GetUIItem("CharSearchPanel");

	if partyPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\PartySearch.xml");		
		partyPanel = UIManager:GetUIItem("PartySearchPanel");
		local partySelectPanel = UIManager:GetUIItem("PartyInfoSelectPanel");
		if partySelectPanel ~= nil then
			partySelectPanel:SetEnable(false);
		end
	end
	
	if partyPanel ~= nil and charPanel ~= nil then
		partyPanel:SetPos( charPanel:GetX(), charPanel:GetY());
		partyPanel:SetEnable(true);
		charPanel:SetEnable(false);
		local npcList = UIManager:FindUICheckBox("ShowNPCList2");
		local charList = UIManager:FindUICheckBox("ShowCharList2");
		local partyList = UIManager:FindUICheckBox("ShowPartyList2");
		if npcList ~= nil and charList ~= nil and partyList~= nil then
			charList:SetState(false);
			npcList:SetState(false);
			partyList:SetState(true);
		end
		ShowPartyList2();
		LuaUI.OnShowPartyInfo();
	end
end

function CharInfoSearchClick()
	local panel = UIManager:GetUIItem("CharSearchPanel");
	
	if panel ~= nil then 
		local relY = UIManager:GetCursorY() - panel:GetY();
		
		if relY >= 70 and relY < 93 then
		    LuaUI.CharSearchClickList(1);
		elseif relY >= 93 and relY < 116 then
			LuaUI.CharSearchClickList(2);
		elseif relY >= 116 and relY < 139 then
		    LuaUI.CharSearchClickList(3);
		elseif relY >= 139 and relY < 162 then
			LuaUI.CharSearchClickList(4);
		elseif relY >= 162 and relY < 185 then
			LuaUI.CharSearchClickList(5);
		elseif relY >= 185 and relY < 208 then
			LuaUI.CharSearchClickList(6);
		elseif relY >= 208 and relY < 231 then
			LuaUI.CharSearchClickList(7);
		elseif relY >= 232 and relY < 254 then
			LuaUI.CharSearchClickList(8);
		elseif relY >= 254 and relY < 277 then
			LuaUI.CharSearchClickList(9);
		elseif relY >= 277 and relY < 300 then
			LuaUI.CharSearchClickList(10);
		elseif relY >= 300 and relY < 323 then
			LuaUI.CharSearchClickList(11);
		elseif relY >= 323 and relY < 346 then
			LuaUI.CharSearchClickList(12);
		end
		
	end

end

function CharInfoFactionClick()
	LuaUI.ShowCharSearchList(2);
	local pPanel = UIManager:GetUIItem("CharSearchPanel");
	if pPanel == nil then return end
	
	pCheckBox = UIManager:FindUICheckBox("CharInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	pCheckBox = UIManager:FindUICheckBox("CharInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("CharInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end

end

function CharInfoCclassClick()
	LuaUI.ShowCharSearchList(3);
	local pPanel = UIManager:GetUIItem("CharSearchPanel");
	if pPanel == nil then return end
	
	pCheckBox = UIManager:FindUICheckBox("CharInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("CharInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	pCheckBox = UIManager:FindUICheckBox("CharInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end

end

function CharInfoLevelClick()
	LuaUI.ShowCharSearchList(4);
	local pPanel = UIManager:GetUIItem("CharSearchPanel");
	if pPanel == nil then return end
	
	pCheckBox = UIManager:FindUICheckBox("CharInfoFaction");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("CharInfoCclass");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	pCheckBox = UIManager:FindUICheckBox("CharInfoLevel");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end

end

function OnCharSearchPagePrev()
	LuaUI.OnCharSearchPagePrev();
end

function OnCharSearchPageNext()
	LuaUI.OnCharSearchPageNext();
end

function OnJoinCharSearch()
	LuaUI.OnJoinCharSearch();
end
