function OpenAntiHackPanel()
    local panel = UIManager:GetUIItem("AntiHackPanel");

	if panel == nil then
	   LuaUI.AddXMLFile2UIManger("Data\\MenuAntiHack.xml");
	   panel = UIManager:GetUIItem("AntiHackPanel");
	end
	if panel ~= nil then
        panel:SetPos(96, 151);
     	panel:SetEnable(true);
     	SetAntiHackFocus();	
	end

end

function OpenAntiHackLoginVerification()
    local panel = UIManager:GetUIItem("AntiHackPanel");

	if panel == nil then
	   LuaUI.AddXMLFile2UIManger("Data\\MenuAntiHack.xml");
	   panel = UIManager:GetUIItem("AntiHackPanel");
	end
	if panel ~= nil then
        panel:SetPos(96, 151);
     	panel:SetEnable(true);
 	end

end


function SelectHackAnswer0()

	SelectHackAnswer(0)

end

function SelectHackAnswer1()

	SelectHackAnswer(1)

end

function SelectHackAnswer2()

	SelectHackAnswer(2)

end

function SelectHackAnswer3()

	SelectHackAnswer(3)

end

function SelectHackAnswer4()

	SelectHackAnswer(4)

end	
	
function SelectHackAnswer(answer)

	local hackMenu = LuaUI.GetAntiHackProcess();

	if hackMenu ~= nil then
		hackMenu:SetHack(answer);
	--	SwitchOffAntiHack()
	end


end



function SwitchOffAntiHack()
	SetEditLineFocus();	
    local panel = UIManager:GetUIItem("AntiHackPanel");

	if panel ~= nil then
	   panel:SetEnable(false);
	   UIManager:DeletePanel("AntiHackPanel");
    end
    
    
end

function SwitchOffAntiHackVerification()
    local panel = UIManager:GetUIItem("AntiHackPanel");

	if panel ~= nil then
	   panel:SetEnable(false);
	   UIManager:DeletePanel("AntiHackPanel");
    end

end


function OpenHackResult(result)
	if result == true then
	    local panel = UIManager:GetUIItem("HackCorrectResult");
		if panel ~= nil then
	        panel:SetPos(200, 200);
	     	panel:SetEnable(true);
		end
	else
	    local panel = UIManager:GetUIItem("HackWrongResult");
		if panel ~= nil then
	        panel:SetPos(200, 200);
	     	panel:SetEnable(true);
		end
	end
end

function CloseAntiHack()
	local panel = UIManager:GetUIItem("AntiHackPanel");
	if panel ~= nil then
	    UIManager:DeletePanel("AntiHackPanel");
	end

	SetEditLineFocus();
	
	
end

function SetAntiHackFocus()

		UIManager:SetKeyInItem("AntiHackDummyEditBox");

end
