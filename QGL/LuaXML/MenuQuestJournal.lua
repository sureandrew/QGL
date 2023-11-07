function OpenQuestJournal()
	
	local panel = UIManager:GetUIItem("QuestJournalPanel");
	
	local MailSparkUi = UIManager:FindUIAniIcon("MailSpark");
	
	if MailSparkUi:IsEnable() then
			MailSparkUi:SetEnable(false);
	end
	
	
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\MenuQuestJournal.xml");
		local journalProcess = LuaUI.GetQuestJournalProcess();
		if journalProcess ~= nil then
			journalProcess:ShowQuestJournal();	      	
		end
		PlayOpenInterfaceEffect();
	else
		CloseQuestJournalPanel();
	end
end

function CloseQuestJournalPanel()
	UIManager:StopDrag();
	local journalProcess = LuaUI.GetQuestJournalProcess();
	if journalProcess ~= nil then
		journalProcess:CloseQuestJournal();	      	
	end
	UIManager:DeletePanel("QuestJournalPanel");
end

function QuestJournalScroll()
	local journalProcess = LuaUI.GetQuestJournalProcess();
	if journalProcess ~= nil then
		journalProcess:JournalScroll(2);
	end
end

function QuestJournalScrollUp()
	local journalProcess = LuaUI.GetQuestJournalProcess();
	if journalProcess ~= nil then
		journalProcess:JournalScroll(0);	      	
	end
end

function QuestJournalScrollDown()
    local journalProcess = LuaUI.GetQuestJournalProcess();
	if journalProcess ~= nil then
		journalProcess:JournalScroll(1);
	end
end

function SelectQuestJournal(index)
	local journalProcess = LuaUI.GetQuestJournalProcess();
	if journalProcess ~= nil then
		journalProcess:SelectJournal(index);	      	
	end
end

function SelectQuestJournal00()
	SelectQuestJournal(0);
end

function SelectQuestJournal01()
    SelectQuestJournal(1);
end

function SelectQuestJournal02()
	SelectQuestJournal(2);
end

function SelectQuestJournal03()
	SelectQuestJournal(3);
end

function SelectQuestJournal04()
	SelectQuestJournal(4);
end

function SelectQuestJournal05()
	SelectQuestJournal(5);
end

function SelectQuestJournal06()
	SelectQuestJournal(6);
end

function SelectQuestJournal07()
	SelectQuestJournal(7);
end

function SelectQuestJournal08()
	SelectQuestJournal(8);
end

function SelectQuestJournal09()
	SelectQuestJournal(9);
end

function DBLCLKQuestJournal(index)
	local journalProcess = LuaUI.GetQuestJournalProcess();
	if journalProcess ~= nil then
		journalProcess:DBClickJournal(index);	      	
	end
end

function DBLCLKQuestJournal00()
	DBLCLKQuestJournal(0);
end

function DBLCLKQuestJournal01()
	DBLCLKQuestJournal(1);
end

function DBLCLKQuestJournal02()
	DBLCLKQuestJournal(2);
end

function DBLCLKQuestJournal03()
	DBLCLKQuestJournal(3);
end

function DBLCLKQuestJournal04()
	DBLCLKQuestJournal(4);
end

function DBLCLKQuestJournal05()
	DBLCLKQuestJournal(5);
end

function DBLCLKQuestJournal06()
	DBLCLKQuestJournal(6);
end

function DBLCLKQuestJournal07()
	DBLCLKQuestJournal(7);
end

function DBLCLKQuestJournal08()
	DBLCLKQuestJournal(8);
end

function DBLCLKQuestJournal09()
	DBLCLKQuestJournal(9);
end
