function FunOpenGenerationPanel()
	local panel = UIManager:GetUIItem("GenerationPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\GenerationPanel.xml");
		local pGenerationProcess = LuaUI.GetGenerationProcess();
		if pGenerationProcess ~= nil then
			pGenerationProcess:UpdateGeneration(true);
		end
		PlayOpenInterfaceEffect();
	else
		FunCloseGenerationPanel();
	end
end

function FunCloseGenerationPanel()
	UIManager:StopDrag();
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:ClearGenerationRequest();
	end
	FunCloseGenerationRequestPanel();
	FunCloseGenerationDisbandPanel();
	local panel = UIManager:GetUIItem("GenerationPanel");
	if panel ~= nil then
		panel:SetEnable(false);
		UIManager:AddDirtyUIItem(panel);
		UIManager:DeletePanel("GenerationPanel");
	end
end

function FunOpenGenerationRequestPanel()
	UIManager:StopDrag();
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:UpDateRequestPanel();
	end
	local panel = UIManager:GetUIItem("GenerationRequestPanel");
	if panel ~= nil then
		panel:SetEnable(true);
	end

end

function FunCloseGenerationRequestPanel()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("GenerationRequestPanel");
	if panel ~= nil then
		panel:SetEnable(false);
	end
end

function FunOpenGenerationDisbandPanel()
	UIManager:StopDrag();
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:UpdateDisbandPanel();
	end

	local panel = UIManager:GetUIItem("GenerationDisbandPanel");
	if panel ~= nil then
		panel:SetEnable(true);
	end

end

function FunCloseGenerationDisbandPanel()
	UIManager:StopDrag();
	local panel = UIManager:GetUIItem("GenerationDisbandPanel");
	if panel ~= nil then
		panel:SetEnable(false);
	end
end

function FunSortByLevel()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SortListByLevel();
	end
end

function FunSortByClass()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SortListByClass();
	end
end

function FunSortByOffline()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SortListByOffline();
	end
end

function FunSortByAccOrExp()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SortListByAccOrExp();
	end
end

function FunSortByGiftOrHisAcc()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SortListByGiftOrHisAcc();
	end
end

function FunAccToExtEqu()
	FunOpenAccShop();
end

function FunSystemDesc()
	OpenHelperPanel(0,0);
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SystemDesc(23);
	end
end

function FunDisbandGeneration()
	FunOpenGenerationDisbandPanel();
end

function FunRequestYounger()
	LuaUI.BBGeneration();
end

function FunGetSavedExp()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:GetSavedExp();
	end
end

function FunRequestElder()
	LuaUI.BBGeneration();
end

function FunGenerationScroll()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:GenerationScroll(2);
	end
end

function FunGenerationScrollUp()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:GenerationScroll(0);
	end
end

function FunGenerationScrollDown()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:GenerationScroll(1);
	end
end

function FunGenerationDisbandConfirm()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:DisbandGeneration();
	end
	FunCloseGenerationDisbandPanel();
end

function FunGenerationDisbandCancle()
	FunCloseGenerationDisbandPanel();
end

function FunGenerationRequestAllow()
	FunCloseGenerationRequestPanel();
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:RequestGenerationAns(true);
	end
end

function FunGenerationRequestRefuse()
	FunCloseGenerationRequestPanel();
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:RequestGenerationAns(false);
	end
end

function FunGenerationSelect00()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(0);
	end
end

function FunGenerationSelect01()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(1);
	end
end

function FunGenerationSelect02()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(2);
	end
end

function FunGenerationSelect03()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(3);
	end
end

function FunGenerationSelect04()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(4);
	end
end

function FunGenerationSelect05()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(5);
	end
end

function FunGenerationSelect06()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(6);
	end
end

function FunGenerationSelect07()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(7);
	end
end

function FunGenerationSelect08()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(8);
	end
end

function FunGenerationSelect09()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:SelectGeneration(9);
	end
end

function FunOpenCharDetail()
	local pGenerationProcess = LuaUI.GetGenerationProcess();
	if pGenerationProcess ~= nil then
		pGenerationProcess:OpenGenCharDetail();
	end
end
