ForceLogoutFlag = 0;

function ForceLogout()
   UIManager:SetEvent(108);
   SetForceLogoutFlag(0);

end

function SetForceLogoutFlag( flag)
   _G.ForceLogoutFlag = flag;
end


function CancelLogout()
	
	local logoutPanel = UIManager:FindUIPanel("ForceLogoutPanel");
	if logoutPanel ~= nil then
		UIManager:DeletePanel("ForceLogoutPanel");
	end
	SetForceLogoutFlag(0);
	if _G.Server == -1 then
	    _G.state = 1;
	else
 		_G.state = 2;
 	end

end                