function CloseItemDetail(pX, pY)
	local panel = UIManager:GetUIItem("ItemDetailMenuPanel");
	if panel ~= nil then 
 	--UIManager:AddDirtyUIItem(panel);
		--panel:SetEnable(false);
		UIManager:DeletePanel("ItemDetailMenuPanel");
		PanelItemDetailState = 0;
		PanelItemDetailCurNo = -1;   
	end
end


