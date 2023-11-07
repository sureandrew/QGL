SelectedLine = -1;

function OpenChangeLine()
	local panel = UIManager:FindUIPanel("ChangeLinePanel");
	if panel == nil then
		local monoPanel = UIManager:GetMonoItem();
	    if LuaUI.HasParty() == false or LuaUI.IsLeader() == true and monoPanel == nil then
			LuaUI.AddXMLFile2UIManger("Data\\ChangeLine.xml");
			panel = UIManager:FindUIPanel("ChangeLinePanel");
			if panel ~= nil then
			    UIManager:ClearMonoPanel();
			    UIManager:SetMonoPanel(panel);
			end
			LuaUI.UpdateChangeLine();
			PlayOpenInterfaceEffect();
			local Menu = LuaUI.GetMenuHelpProcess();
			Menu:HandleEvent(27);
		end
	else
		CloseChangeLine();
	end
end

function CloseChangeLine()
	for i=1, 10 do
		local button = UIManager:FindUIButton("MapLine"..i);
		if button ~= nil then
			button:SetLockState(false);
			button:SetNormalState();
		end
	end
	local panel = UIManager:GetUIItem("ChangeLinePanel");
    if panel ~= nil then
    	PlayCloseInterfaceEffect();
    end
	UIManager:DeletePanel("ChangeLinePanel");
	_G.SelectedLine = -1;
	UIManager:ClearMonoPanel();

end

function ResetMapLineButton()
	if _G.SelectedLine > 0  and _G.SelectedLine <= 10 then
		local button = UIManager:FindUIButton("MapLine".._G.SelectedLine);
		if button ~= nil then
			button:SetLockState(false);
			button:SetNormalState();
		end
		_G.SelectedLine = -1;
	end
end

function SelectMapLine(index)

	if index > 0 and index <= 10 then
		ResetMapLineButton();
		local button = UIManager:FindUIButton("MapLine"..index);
		if button ~= nil then
			button:SetMouseOverState();
			button:SetLockState(true);
		end
		_G.SelectedLine = index;
	end
end

function SelectMapLine1()
	SelectMapLine(1);
end

function SelectMapLine2()
	SelectMapLine(2);
end

function SelectMapLine3()
	SelectMapLine(3);
end

function SelectMapLine4()
	SelectMapLine(4);
end

function SelectMapLine5()
	SelectMapLine(5);
end

function SelectMapLine6()
	SelectMapLine(6);
end

function SelectMapLine7()
	SelectMapLine(7);
end

function SelectMapLine8()
	SelectMapLine(8);
end

function SelectMapLine9()
	SelectMapLine(9);
end

function SelectMapLine10()
	SelectMapLine(10);
end

function DBCLChangeLine1()
	SelectMapLine1();
	ConfirmChangeLine();
end

function DBCLChangeLine2()
	SelectMapLine2();
	ConfirmChangeLine();
end

function DBCLChangeLine3()
	SelectMapLine3();
	ConfirmChangeLine();
end

function DBCLChangeLine4()
	SelectMapLine4();
	ConfirmChangeLine();
end

function DBCLChangeLine5()
	SelectMapLine5();
	ConfirmChangeLine();
end

function DBCLChangeLine6()
	SelectMapLine6();
	ConfirmChangeLine();
end

function DBCLChangeLine7()
	SelectMapLine7();
	ConfirmChangeLine();
end

function DBCLChangeLine8()
	SelectMapLine8();
	ConfirmChangeLine();
end

function DBCLChangeLine9()
	SelectMapLine9();
	ConfirmChangeLine();
end

function DBCLChangeLine10()
	SelectMapLine10();
	ConfirmChangeLine();
end


function ConfirmChangeLine()
	if _G.SelectedLine > 0 and _G.SelectedLine <= 10 then
		local lineIndex = _G.SelectedLine;
		CloseChangeLine();
	    LuaUI.ChangeMapLine(lineIndex);

	end
end

function ChangeLineConfirmSec()
	LuaUI.CloseFriendBox();
	LuaUI.ConfirmChangeLineSec();
end
