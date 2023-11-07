function CloseApplyMembershipListPanel(pX, pY)
   local panel = UIManager:GetUIItem("ApplyMembershipListPanel");
   UIManager:StopDrag();
   if panel ~= nil then 
       	UIManager:AddDirtyUIItem(panel);
		panel:SetEnable(false);
		UIManager:DeletePanel("ApplyMembershipListPanel");
		CloseApplicantPopup();
  
   end
end

function ApplyMembershipListWheel(pX, pY, pZ)
	UIManager:FindUIScrollBarV("ApplicantScrollBar"):OnMouseWheel(UIManager:GetUIItem("ApplicantScrollBar"):GetX()
	,UIManager:GetUIItem("ApplicantScrollBar"):GetY(),pZ);
end