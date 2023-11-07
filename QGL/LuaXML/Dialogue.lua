function OpenDialogueBox(pX, pY)
	local BgPanel = UIManager:FindUIPanelEx("DialogueBox");
	if BgPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\Dialogue.xml");
	else
		-- npc image	
		local imagepanel = UIManager:FindUIPanel("DialogueImagePanel");
		if imagepanel ~= nil then
			local image = UIManager:FindUIImage("NPCPic");
			if image ~= nil then
				UIManager:GetUIItem("NPCPic"):SetEnable(true);
				UIManager:GetUIItem("DialogueImagePanel"):SetEnable(true);
			end
		end
	
		UIManager:GetUIItem("DialogueBox"):SetEnable(true);	
		
		UIManager:SetPanel2Top(imagepanel);
		UIManager:SetPanelEx2Top(BgPanel);
		
		local msgchat = UIManager:FindUIChatPanel("DialogueChat");	
		if msgchat ~= nil then
			msgchat:ClearAllRow()
		end

		local PopUp = UIManager:FindUIPopUpMenu("DialogueChoosePopup");
		if PopUp ~= nil then
			--PopUp:ClearData();
			--PopUp:Redraw();
		end
				
		UIManager:GetUIItem("DialogueChat"):SetEnable(true);
		UIManager:GetUIItem("DialogueUp"):SetEnable(false);
		UIManager:GetUIItem("DialogueDown"):SetEnable(false);
		UIManager:GetUIItem("DialogueChoosePopup"):SetEnable(false);	
	end

	local mono = UIManager:FindUIPanelEx("DialogueBox");
	if mono ~= nil then
		UIManager:ClearMonoPanel();
		UIManager:SetMonoPanelEx(mono);
		UIManager:LockMonoPanel("DialogueBox");
	end
	UIManager:RenderAll();
end

function CloseDialogueBox(pX, pY)
	UIManager:StopDrag();
	local BgPanel = UIManager:FindUIPanelEx("DialogueBox");
	if BgPanel ~= nil then
		-- chat panel
		local msgchat = UIManager:FindUIChatPanel("DialogueChat");	
		if msgchat ~= nil then
			msgchat:ClearAllRow()
		end
		-- pop up menu
		local PopUp = UIManager:FindUIPopUpMenu("DialogueChoosePopup");
		if PopUp ~= nil then
			--PopUp:ClearData();
			--PopUp:Redraw();
			UIManager:GetUIItem("DialogueChoosePopup"):SetEnable(false);
		end

		UIManager:GetUIItem("DialogueBox"):SetEnable(false);
	end
	
	-- npc image
	local image = UIManager:FindUIImage("NPCPic");
	if image ~= nil then
		UIManager:GetUIItem("NPCPic"):SetEnable(false);
	end
	
	local imagepanel = UIManager:FindUIPanel("DialogueImagePanel");
	if imagepanel ~= nil then
		imagepanel:SetEnable(false);
	end
	

	UIManager:UnlockMonoPanel("DialogueBox");
	UIManager:ClearMonoPanel();
	UIManager:RenderAll();
end

function DialogueScrollUp(pX, pY)
	UIManager:FindUIChatPanel("DialogueChat"):UpRow();
end

function DialogueScrollDown(pX, pY)
	UIManager:FindUIChatPanel("DialogueChat"):DownRow();
end

function DialogueChoose(pX, pY)
	local PopUp = UIManager:FindUIPopUpMenu("DialogueChoosePopup");
	
	if PopUp ~= nil then
		local currsel = PopUp:GetBarPos();
		UIManager:GetUIItem("DialogueChoosePopup"):SetEnable(false);
		CloseDialogueBox(0, 0);

		LuaUI.PlayerChooseResult(currsel);
	else
		UIManager:GetUIItem("DialogueChoosePopup"):SetEnable(false);
		CloseDialogueBox(0, 0);
	end

	UIManager:RenderAll();
end

function ClickNextMsg(pX, pY)
	local uim = UIManager:GetUIItem("DialogueBox");
	if uim ~= nil then
		--if uim:IsEnable() == true then
			LuaUI.MBChangeNextPage();
		--end
	end
end

function HideDialogueChoosePopUp()
	local PopUp = UIManager:FindUIPopUpMenu("DialogueChoosePopup");
	if PopUp ~= nil then
		PopUp:ClearData();
		UIManager:GetUIItem("DialogueChoosePopup"):SetEnable(false);	
	end
end

function ShowDialogueChoosePopUp()
	local BgPanel = UIManager:FindUIPanelEx("DialogueBox");
	if BgPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\Dialogue.xml");
	else
		UIManager:GetUIItem("DialogueBox"):SetEnable(true);
	end

	UIManager:GetUIItem("DialogueChat"):SetEnable(false);			
	UIManager:GetUIItem("DialogueUp"):SetEnable(false);
	UIManager:GetUIItem("DialogueDown"):SetEnable(false);	

	local PopUp = UIManager:FindUIPopUpMenu("DialogueChoosePopup");
	if PopUp ~= nil then
		UIManager:GetUIItem("DialogueChoosePopup"):SetEnable(true);
		PopUp:ClearData();	
	end

	UIManager:ClearMonoPanel();
	UIManager:FindUIPanelEx("DialogueBox"):SetMonoPanelEx(BgPanel);
end

function UpdateDialogueChoosePopUp(row, col)
	local PopUp = UIManager:FindUIPopUpMenu("DialogueChoosePopup");
	if PopUp ~= nil then
		PopUp:SetImage(row, col);
		PopUp:SetDetectRect();
		PopUp:Redraw();
	end	
end

----------------------------------------
function AMBNext(pX, pY)
	local button = UIManager:GetUIItem("AMBButtonNext");
	if button ~= nil then
		if button:IsEnable() == true then
			LuaUI.MBChangeNextPage();
		end	
	end
end

function AMBClickNext(pX, pY)
	local uim = UIManager:GetUIItem("BetaMessageBox");
	if uim ~= nil then
		if uim:IsEnable() == true then
			LuaUI.MBChangeNextPage();
		end
	end
end

function AMBScrollUp(pX, pY)
	UIManager:FindUIChatPanel("TalkMsgChat"):UpRow();
end

function AMBScrollDown(pX, pY)
	UIManager:FindUIChatPanel("TalkMsgChat"):DownRow();
end
