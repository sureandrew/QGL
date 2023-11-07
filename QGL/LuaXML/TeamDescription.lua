function OpenTeamDescription()
	local panel = UIManager:GetUIItem("TeamDescriptionPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\TeamDescription.xml");
		LuaUI.UpdateLineupSelectMenu();
		local TeamDescription = LuaUI.GetTeamDescriptionProcess();
		if TeamDescription ~= nil then
			TeamDescription:ShowTeamDescription();
		end
	else
		CloseTeamDescription();
	end
end

function CloseTeamDescription()
	UIManager:StopDrag();
	local TeamDescription = LuaUI.GetTeamDescriptionProcess();
	if TeamDescription ~= nil then
		TeamDescription:CloseTeamDescription();
	end
	UIManager:DeletePanel("TeamDescriptionPanel");
	UIManager: SetKeyInItem("EditLine");
end

function TeamDescriptionFieldBurning()
	TeamDescriptionIndex(1);
end

function TeamDescriptionToPunishEvilTask()
	TeamDescriptionIndex(2);
end

function TeamDescriptionEliminateGreenTask()
	TeamDescriptionIndex(3);
end

function TeamDescriptionVillaTask()
	TeamDescriptionIndex(4);
end

function TeamDescriptionDuplicate()
	TeamDescriptionIndex(5);
end

function TeamDescriptionEvents()
	TeamDescriptionIndex(6);
end

function TeamDescriptionIndex(index)

	local FieldBurningCheckBox = UIManager:FindUICheckBox("TeamDescriptionFieldBurning");
	local ToPunishEvilTaskCheckBox = UIManager:FindUICheckBox("TeamDescriptionToPunishEvilTask");
	local EliminateGreenTaskCheckBox = UIManager:FindUICheckBox("TeamDescriptionEliminateGreenTask");
	local VillaTaskCheckBox = UIManager:FindUICheckBox("TeamDescriptionVillaTask");
	local DuplicateCheckBox = UIManager:FindUICheckBox("TeamDescriptionDuplicate");
	local EventsCheckBox = UIManager:FindUICheckBox("TeamDescriptionEvents");
	
	FieldBurningCheckBox:SetState(true);
	ToPunishEvilTaskCheckBox:SetState(true);
	EliminateGreenTaskCheckBox:SetState(true);
	VillaTaskCheckBox:SetState(true);
	DuplicateCheckBox:SetState(true);
	EventsCheckBox:SetState(true);
	
	if index == 1 then
		FieldBurningCheckBox:SetState(false);
	elseif index == 2 then
		ToPunishEvilTaskCheckBox:SetState(false);
	elseif index == 3 then
		EliminateGreenTaskCheckBox:SetState(false);
	elseif index == 4 then
		VillaTaskCheckBox:SetState(false);
	elseif index == 5 then
		DuplicateCheckBox:SetState(false);
	elseif index == 6 then
		EventsCheckBox:SetState(false);
	end
	
	local TeamDescriptionProcess = LuaUI.GetTeamDescriptionProcess();
	if TeamDescriptionProcess ~= nil then
		TeamDescriptionProcess:TeamDescriptionTabControl(index);	      	
	end
end

function TeamDescriptionOK()
	local Input = UIManager:FindUIEditBox("TeamDescriptionInput");
	local LevelDown = UIManager:FindUIEditBox("TeamDescriptionLevelDown");
	local LevelUp = UIManager:FindUIEditBox("TeamDescriptionLevelUp");
	local TeamDescription = LuaUI.GetTeamDescriptionProcess();

	if Input ~= nil and LevelDown~= nil and LevelUp~= nil and TeamDescription ~= nil then
		TeamDescription:TeamDescriptionInput(Input:GetText());
		TeamDescription:TeamDescriptionLevelDown(LevelDown:GetInt());
		TeamDescription:TeamDescriptionLevelUp(LevelUp:GetInt());
		TeamDescription:SetTeamDescriptionData();
	end

	CloseTeamDescription();
end

function TeamDescriptionCancel()
	CloseTeamDescription();
end
