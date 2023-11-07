
function OpenChooseTimePanel()
  local panel = UIManager:GetUIItem("ChooseTimePanel");
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\ChooseTimePanel.xml");
		local relationSystemProcess = LuaUI.GetRelationSystemProcess();
		if relationSystemProcess ~= nil then
			relationSystemProcess:ShowChooseTime();	      	
		end
	end
end

function CloseChooseTimePanel()
  local panel = UIManager:FindUIPanel("ChooseTimePanel");
	if panel then
		UIManager:DeletePanel("ChooseTimePanel");
	end

end


function ChooseTimeListScrollUp()
local relationSystemProcess = LuaUI.GetRelationSystemProcess();
	if relationSystemProcess ~= nil then
		relationSystemProcess:ChooseTimeScroll(0);	      	
	end
end

function ChooseTimeListScrollDown()
local relationSystemProcess = LuaUI.GetRelationSystemProcess();
	if relationSystemProcess ~= nil then
		relationSystemProcess:ChooseTimeScroll(1);	      	
	end
end

function ChooseTimeListScroll()
local relationSystemProcess = LuaUI.GetRelationSystemProcess();
	if relationSystemProcess ~= nil then
		relationSystemProcess:ChooseTimeScroll(2);	      	
	end
end

function SelectedChooseTime(index)
	local relationSystemProcess = LuaUI.GetRelationSystemProcess();
	if relationSystemProcess ~= nil then
		relationSystemProcess:SelectChooseTime(index);	      	
	end
end

function SelectChooseTime00()
	SelectedChooseTime(0)
end

function SelectChooseTime01()
	SelectedChooseTime(1)
end

function SelectChooseTime02()
	SelectedChooseTime(2)
end

function SelectChooseTime03()
	SelectedChooseTime(3)
end

function ChooseTimeOK()
	local relationSystemProcess = LuaUI.GetRelationSystemProcess();
	if relationSystemProcess ~= nil then
		relationSystemProcess:EstablishChooseTime();	      	
	end
	CloseChooseTimePanel();
end

