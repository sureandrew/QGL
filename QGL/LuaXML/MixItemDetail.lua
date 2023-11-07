function CloseMixDetail(pX, pY)
	local panel = UIManager:GetUIItem("MixDetailMenuPanel");
	if panel ~= nil then 
 	--UIManager:AddDirtyUIItem(panel);
		--panel:SetEnable(false);
		UIManager:DeletePanel("MixDetailMenuPanel");  
	end
end


