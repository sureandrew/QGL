function BDB_OpenDialog(pX, pY)
	local BgPanel = UIManager:FindUIPanelEx("BDB_DialogueBox");
	if BgPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\BatDialogue.xml");
	else
		-- npc image	
		local imagepanel = UIManager:FindUIPanel("BDB_DialogImagePanel");
		if imagepanel ~= nil then
			local image = UIManager:FindUIImage("BDB_NPCPic");
			if image ~= nil then
				UIManager:GetUIItem("BDB_NPCPic"):SetEnable(true);
			end
		end
		
		UIManager:GetUIItem("BDB_DialogueBox"):SetEnable(true);	
		UIManager:SetPanel2Top(imagepanel);
		UIManager:SetPanelEx2Top(BgPanel);
		
		local msgchat = UIManager:FindUIChatPanel("BDB_DialogueChat");	
		if msgchat ~= nil then
			msgchat:ClearAllRow()
		end
		
		local PopUp = UIManager:FindUIPopUpMenu("BDB_DialogueChoosePopup");
		if PopUp ~= nil then
			--PopUp:ClearData();
			--PopUp:Redraw();
		end
		
		UIManager:GetUIItem("BDB_DialogueChat"):SetEnable(true);
		UIManager:GetUIItem("BDB_DialogueChoosePopup"):SetEnable(false);	
	end
	
	local mono = UIManager:FindUIPanelEx("BDB_DialogueBox");
	if mono ~= nil then
		UIManager:ClearMonoPanel();
		UIManager:SetMonoPanelEx(mono);
	end
	UIManager:RenderAll();
end

function BDB_CloseDialog(pX, pY)
	UIManager:StopDrag();
	local BgPanel = UIManager:FindUIPanelEx("BDB_DialogueBox");
	if BgPanel ~= nil then
		-- chat panel
		local msgchat = UIManager:FindUIChatPanel("BDB_DialogueChat");	
		if msgchat ~= nil then
			msgchat:ClearAllRow()
		end
		-- pop up menu
		local PopUp = UIManager:FindUIPopUpMenu("BDB_DialogueChoosePopup");
		if PopUp ~= nil then
			--PopUp:ClearData();
			--PopUp:Redraw();
			UIManager:GetUIItem("BDB_DialogueChoosePopup"):SetEnable(false);
		end

		UIManager:GetUIItem("BDB_DialogueBox"):SetEnable(false);
	end
	
	-- npc image
	local image = UIManager:FindUIImage("BDB_NPCPic");
	if image ~= nil then
		UIManager:GetUIItem("BDB_NPCPic"):SetEnable(false);
	end

	UIManager:ClearMonoPanel();
	UIManager:RenderAll();
end

function BDB_DialogueChoose(pX, pY)
	local PopUp = UIManager:FindUIPopUpMenu("BDB_DialogueChoosePopup");
	if PopUp ~= nil then
		local currsel = PopUp:GetBarPos();
		UIManager:GetUIItem("BDB_DialogueChoosePopup"):SetEnable(false);
		BDB_CloseDialog(0, 0);
		LuaUI.PlayerChooseResultInBat(currsel);
	else
		BDB_CloseDialog(0, 0);
	end
end

function BDB_ClickNextMsg(pX, pY)
	local uim = UIManager:GetUIItem("BDB_DialogueBox");
	if (uim ~= nil) then
		LuaUI.BatDialogNextPage();
	end
end

function BDB_ShowPopUp()
	local BgPanel = UIManager:FindUIPanelEx("BDB_DialogueBox");
	if BgPanel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\BatDialogue.xml");
	else
		UIManager:GetUIItem("BDB_DialogueBox"):SetEnable(true);
	end

	UIManager:GetUIItem("BDB_DialogueChat"):SetEnable(false);	

	local PopUp = UIManager:FindUIPopUpMenu("BDB_DialogueChoosePopup");
	if (PopUp ~= nil) then
		UIManager:GetUIItem("BDB_DialogueChoosePopup"):SetEnable(true);
		PopUp:ClearData();	
	end

	UIManager:ClearMonoPanel();
	UIManager:FindUIPanelEx("BDB_DialogueBox"):SetMonoPanelEx(BgPanel);
end

function BDB_UpdatePopUp(row, col)
	local PopUp = UIManager:FindUIPopUpMenu("BDB_DialogueChoosePopup");
	if PopUp ~= nil then
		PopUp:SetImage(row, col);
		PopUp:SetDetectRect();
		PopUp:Redraw();
	end	
end

function BDB_HidePopUp()
	local PopUp = UIManager:FindUIPopUpMenu("BDB_DialogueChoosePopup");
	if PopUp ~= nil then
		PopUp:ClearData();
		UIManager:GetUIItem("BDB_DialogueChoosePopup"):SetEnable(false);	
	end
end





