AlertCountDownValue = 0;

function CloseAlertBox()
	UIManager:StopDrag();
	local panel = UIManager:FindUIPanel("AlertBoxPanel");
	local temp = UIManager:GetUIItem("AlertBoxPanel");
	if panel ~= nil and temp ~= nil then
		UIManager:ClearMonoPanel();
		temp:SetEnableTimeOutEvent(false);
	    UIManager:DeletePanel("AlertBoxPanel");
	end
end

function ShowAlertOK()
	LuaUI.ShowAlertOK();
end

function SetAlertCountDown(countDown)
	_G.AlertCountDownValue = countDown;
	if countDown > 0 then
	    local textField = UIManager:FindUIStaticText("AlertCountDownText");
		local tempField = UIManager:GetUIItem("AlertCountDownText");
		if textField ~= nil and tempField ~= nil then
		    textField:LuaSetText(_G.AlertCountDownValue.."");
			textField:Redraw();
			UIManager:AddDirtyUIItem(tempField);
		end
	end
end

function AlertCountDown()
	local textField = UIManager:FindUIStaticText("AlertCountDownText");
	local tempField = UIManager:GetUIItem("AlertCountDownText");
	if textField ~= nil and tempField ~= nil then
		if _G.AlertCountDownValue > 0 then
		    _G.AlertCountDownValue = _G.AlertCountDownValue - 1;
			textField:LuaSetText(_G.AlertCountDownValue.."");
			textField:Redraw();
		    if _G.AlertCountDownValue == 0 then
		        tempField:SetEnableTimeOutEvent(false);
		        tempField:SetEnable(false);
		        local otherTextField = UIManager:GetUIItem("AlertText");
		        if otherTextField ~= nil then
		            otherTextField:SetOffsetXY(0, 35);
		            UIManager:AddDirtyUIItem(otherTextField);
		        end
			else
				tempField:SetEnableTimeOutEvent(true);
		    end
		    UIManager:AddDirtyUIItem(tempField);
		else
		    tempField:SetEnableTimeOutEvent(false);
		    tempField:SetEnable(false);
		    local otherTextField = UIManager:GetUIItem("AlertText");
	        if otherTextField ~= nil then
	            otherTextField:SetOffsetXY(0, 35);
	            UIManager:AddDirtyUIItem(otherTextField);
	        end
		end
	end
end