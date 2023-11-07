L_MENU_QUESTLOG = LuaUI.GetQuestLogProcess();
local bQuestItemDetailTimeOut = false;

function OpenQuestLogPanel(pX, pY)
	local but = UIManager:FindUIButton("DownBMission");
	if but ~= nil then
		but:SetLockState(false);
		but:SetNormalState();
	end
	
	L_MENU_QUESTLOG =  LuaUI.GetQuestLogProcess();
	local panel = UIManager:GetUIItem("QuestLogMenuPanel");
	
	if panel == nil then
		if L_MENU_QUESTLOG ~= nil then
			L_MENU_QUESTLOG:OnUpdateNoteUI();
		end

		LuaUI.AddXMLFile2UIManger("Data\\MenuQuestLog.xml");
		
		L_MENU_QUESTLOG:OnSelectLogDType(2);
--		L_MENU_QUESTLOG:UpdateMainQuestLstUI(-1);
--		if _G.InBattle then
--			QuestLogCheckBattleLua();
--		else
--			QuestLogCheckGangLua();
--		end
		PlayOpenInterfaceEffect();
		
		
	else
		CloseQuestLog(px,py);
	end
end

function CloseQuestLog(pX, pY)
	UIManager:StopDrag();
	bQuestItemDetailTimeOut = false;
	CloseItemDetail(0, 0);
	local panel = UIManager:GetUIItem("QuestLogMenuPanel");
	if panel ~= nil then 
		UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
		UIManager:DeletePanel("QuestLogMenuPanel");
		PlayCloseInterfaceEffect();
		L_MENU_QUESTLOG = nil;
	end
end

function QuestMenuProcess(pX, pY)
	local QuestMenuDesc = UIManager:FindUIChatPanel("QuestMenuDesc")
	if QuestMenuDesc ~= nil then
		local ChatLink = QuestMenuDesc:GetLinkItem()
		if ChatLink ~= nil then
			if ChatLink:GetLinkType() == 3 then
				if LuaUI.CharSetMapPos(ChatLink:GetLinkSubString(5)) then
					CloseQuestLog(pX, pY)
				else
					LuaUI.LuaAddAlertBox("MSG_JUST_USE_SAME_MAP", true)
				end
			end
		end
	end
end

function CancelQuest(pX, pY)
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaCancelQuestClient();
	end
end

function QMQList_ScrollUp(pX, pY)
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaMainLstUpRow();
	end
end

function QMQList_ScrollDown(pX, pY)
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaMainLstDownRow();
	end
end

function QMQList_ScrollMove()
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaMainLstScrollMove();
	end
end

function QMDesc_ScrollUp(pX, pY)
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaSubDescUpRow();
	end
end

function QMDesc_ScrollDown(pX, pY)
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaSubDescDownRow();
	end
end

function QMDesc_UpdateScrollV()
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaUpdateSubDescScrollV();
	end
end

function QMDesc_ScrollMove()
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:LuaSubDescScrollMove();
	end
end

function QuestMenuSel(index)
	L_MENU_QUESTLOG = LuaUI.GetQuestLogProcess();
	if L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:UpdateMainQuestLstUI(index);
	end
end

function QuestMenuSel1()
	QuestMenuSel(0);
end

function QuestMenuSel2()
	QuestMenuSel(1);
end

function QuestMenuSel3()
   	QuestMenuSel(2);
end

function QuestMenuSel4()
   	QuestMenuSel(3);
end

function QuestMenuSel5()
   	QuestMenuSel(4);
end

function QuestMenuSel6()
   	QuestMenuSel(5);
end

function QuestMenuSel7()
   	QuestMenuSel(6);
end

function QuestMenuSel8()
   	QuestMenuSel(7);
end

function QuestMenuSel9()
   	QuestMenuSel(8);
end

function QuestMenuSel10()
   	QuestMenuSel(9);
end

function QuestMenuSel11()
   	QuestMenuSel(10);
end

function QuestMenuSel12()
   	QuestMenuSel(11);
end

function QuestMenuSel13()
   	QuestMenuSel(12);
end

function QuestMenuSel14()
   	QuestMenuSel(13);
end

function QuestMenuSel15()
   	QuestMenuSel(14);
end

function QuestMenuSel16()
   	QuestMenuSel(15);
end

function QuestMenuSel17()
   	QuestMenuSel(16);
end

function QuestMenuSel18()
   	QuestMenuSel(17);
end

function QuestMenuSel19()
   	QuestMenuSel(18);
end

function QuestLogMenuWheel(pX, pY, pZ)
	local x = pX - UIManager:GetUIItem("QuestLogMenuPanel"):GetX();
	if x <= 150 then
	UIManager:FindUIScrollBarV("QMQList_ScrollV"):OnMouseWheel(UIManager:GetUIItem("QMQList_ScrollV"):GetX()
	,UIManager:GetUIItem("QMQList_ScrollV"):GetY(),pZ);
	else
	UIManager:FindUIScrollBarV("QMDesc_ScrollBarV"):OnMouseWheel(UIManager:GetUIItem("QMDesc_ScrollBarV"):GetX()
	,UIManager:GetUIItem("QMDesc_ScrollBarV"):GetY(),pZ);
	end
end

function SelectNoQuestLog()
	local pCheckBox = UIManager:FindUICheckBox("NoGettingQuestLog");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	
	pCheckBox = UIManager:FindUICheckBox("GettingQuestLog");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	
	UIManager:AddDirtyUIItem(pCheckBox);
		
	L_MENU_QUESTLOG = LuaUI.GetQuestLogProcess();
	if L_MENU_QUESTLOG ~= nil then		
		L_MENU_QUESTLOG:OnSelectLogDType(1);
	end
end

function SelectQuestLog()
	local pCheckBox = UIManager:FindUICheckBox("NoGettingQuestLog");
	if pCheckBox ~= nil then
		pCheckBox:SetState(false);
	end
	
	pCheckBox = UIManager:FindUICheckBox("GettingQuestLog");
	if pCheckBox ~= nil then
		pCheckBox:SetState(true);
	end
	
	UIManager:AddDirtyUIItem(pCheckBox);
	
	L_MENU_QUESTLOG = LuaUI.GetQuestLogProcess();
	if L_MENU_QUESTLOG ~= nil then		
		L_MENU_QUESTLOG:OnSelectLogDType(2);
	end
end

function SelectQuestTrace()
	local pCheckBox = UIManager:FindUICheckBox("QTraceCheckBox");
	if pCheckBox ~= nil and L_MENU_QUESTLOG ~= nil then
		L_MENU_QUESTLOG:OnSelectQuestTrace(pCheckBox:GetState());
		UIManager:AddDirtyUIItem(pCheckBox);
	end		
end

function QuestItemOver()
	local panel = UIManager:GetUIItem("QuestLogMenuPanel");
	if panel == nil then
		return;
	end
	
	local itemPos = GetQuestItemSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY());
	if itemPos == -1 then
		return;
	end
	
	local icon = UIManager:GetUIItem(string.format("Quest%03dIcon", itemPos));
	if icon == nil then
		return;
	end
	
	-- set select rect pos 
	local selectrect = UIManager:GetUIItem("QuestSelectRect");
	if selectrect ~= nil then
		if selectrect:IsEnable() == false then
			selectrect:SetEnable(true);	
			UIManager:AddDirtyUIItem(selectrect);
		end
		if (selectrect:GetOffsetX() ~= (icon:GetOffsetX() - 1 )) or 
			(selectrect:GetOffsetY() ~= (icon:GetOffsetY() - 1 )) then	
			selectrect:SetOffsetXY(icon:GetOffsetX() - 1, icon:GetOffsetY() - 1);
			UIManager:AddDirtyUIItem(selectrect);
		end
	end
	
	-- need to show shop item details
	if bQuestItemDetailTimeOut == true then
		QuestItemDetail(1);
	end
end

function ShowQuestSelectRect()
	local image = UIManager:GetUIItem("QuestSelectRect");
	local mainpanel = UIManager:GetUIItem("QuestLogMenuPanel");
	if ( image ~= nil ) and ( mainpanel ~= nil ) then
		--local leftupx = mainpanel:GetX() + image:GetOffsetX();
		--local leftupy = mainpanel:GetY() + image:GetOffsetY();
		local leftupx = image:GetX();
		local leftupy = image:GetY();
		local mx = UIManager:GetCursorX();
		local my = UIManager:GetCursorY();
		if ( ( mx > leftupx ) and ( mx < (leftupx + 40 ) ) and
			 ( my > leftupy ) and ( my < (leftupy + 40) ) ) then
			
			image:SetEnable(true);
			UIManager:AddDirtyUIItem(image);
			if bQuestItemDetailTimeOut == false then
				bQuestItemDetailTimeOut = true;
				QuestItemDetail(1);
				--bQuestItemDetailTimeOut = true;
			end
		else
			image:SetEnable(false);
			bQuestItemDetailTimeOut = false;
		end
	end
end

function QuestItemDetail(pMouseIn)
	if bQuestItemDetailTimeOut == true then	
		local detail = UIManager:GetUIItem("ItemDetailMenuPanel");
		if detail == nil then
			LuaUI.AddXMLFile2UIManger("Data\\ItemDetail.xml");
			detail = UIManager:GetUIItem("ItemDetailMenuPanel");
			
				if detail == nil then
				return;
			else
				detail:SetEnable(false);
			end
			
			local mx = UIManager:GetCursorX();
			local my = UIManager:GetCursorY();
			
			local slotindex = GetQuestItemSlotIndex(mx, my);
			if slotindex ~= -1 then 
				--detail:SetEnable(true);
				LuaUI.UpdateQuestItemDetail(slotindex);
				
				-- reset detail pos 
				local height = detail:GetHeight();
				local width = detail:GetWidth();
			
				local reset_x = mx;
				local reset_y = my;
			
				if mx < 400 then
					reset_x = mx + 40;
				else
					reset_x = mx - width - 40; 
				end
			
				if my < 300 then
					reset_y = my + 5;
				else
					reset_y = my - height - 5;
				end
				
				if (reset_y + height) > 599 then
					reset_y = reset_y - height;
				end
			
				if reset_x < 0 then
					reset_x = 0;
				end
				if reset_y < 0 then
					reset_y = 0;
				end
				--[[
				if mx + width > 799  then
					reset_x = mx - width;
				end
			
				if my + height > 599  then
					reset_y = my - height;
				end
				]]--
			
				detail:SetPos(reset_x, reset_y);  
				UIManager:RenderAll();	
			end
		else
		end
	else
		--ItemDetail(pMouseIn, GetQuestItemSlotIndex(UIManager:GetCursorX(), UIManager:GetCursorY()));
	end	
end

function GetQuestItemSlotIndex(MouseX, MouseY)
	local uim = UIManager:GetUIItem("Quest000Icon");
	if uim ~= nil then
		
		local leftupX = uim:GetX();
		local leftupY = uim:GetY();
		--[[
		if leftupX >= 0 then
			--local tempa = leftupX;
			TalkPanel(0,0, string.format("large :%d", leftupX));
			--leftupX = -tempa;
		else
			leftupX = -leftupX;
			TalkPanel(0,0, string.format("small :%d", leftupX));
		end
		
		if leftupY < 0 then
			leftupY = -leftupY;
		end
		]]
		local x = MouseX - leftupX;
		if x < 1 then 
			x = 0;
		end

		local y = MouseY - leftupY;
		if y < 1 then
			y = 0;
		end

		if ( x > 0 ) and ( x < (41 * 6) ) and ( y > 0 ) and ( y < (41 * 8)) then
			return (LuaUI.Div(y, 41) * 6 + LuaUI.Div(x, 41));
		else
			return -1;
		end
	end
	return -1;
end

function QuestItemMouseOut()
	local selectrect = UIManager:GetUIItem("QuestSelectRect");
	if selectrect ~= nil then
		selectrect:SetEnable(false);
		UIManager:AddDirtyUIItem(selectrect);
	end
	CloseItemDetail(0, 0);
	bQuestItemDetailTimeOut = false;
end
