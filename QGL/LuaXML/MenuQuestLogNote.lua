function CloseQuestLogNote(pX, pY)
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("QuestLogNotePanel");
	if panel ~= nil then 
		UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
		UIManager:DeletePanel("QuestLogNotePanel");
	end
end

function QuestNoteProcess(pX, pY)
	local QuestMenuDesc = UIManager:FindUIChatPanel("QuestNoteDesc");
	if QuestMenuDesc ~= nil then
		local ChatLink = QuestMenuDesc:GetLinkItem();
		if ChatLink ~= nil then
			if ChatLink:GetLinkType() == 3 then
				if LuaUI.CharSetMapPos(ChatLink:GetLinkSubString(5)) then
					local but = UIManager:FindUIButton("DownBMission");
					if but ~= nil then
						but:SetLockState(false);
						but:SetNormalState();
					end				
				else
					LuaUI.LuaAddAlertBox("MSG_JUST_USE_SAME_MAP", true);
				end
				
			elseif ChatLink:GetLinkType() == 7 then
				LuaUI.CharOpenQuestMenu(ChatLink:GetLinkSubString(5));
				local but = UIManager:FindUIButton("DownBMission");
				if but ~= nil then
					but:SetLockState(false);
					but:SetNormalState();
				end				
			end

		else
			LuaUI.UpdateNormalClick(pX, pY);
		end
	end
end