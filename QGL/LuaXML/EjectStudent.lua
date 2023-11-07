function OpenEjectStudentPanel()
	local panel = UIManager:GetUIItem("EjectStudentPanel");
	if panel == nil then
		LuaUI.AddXMLFile2UIManger("Data\\EjectStudent.xml");
		local ejectProcess = LuaUI.GetEjectStudentProcess();
		if ejectProcess ~= nil then
			ejectProcess:ShowEjectStudent();	      	
		end
		--PlayOpenInterfaceEffect();
		panel:SetEnable(true);
	else
		CloseEjectStudentPanel();
	end

end

function CloseEjectStudentPanel()
	local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:CloseEjectStudent();	 
		ejectProcess:ClearAllEject();     	
	end
	UIManager:DeletePanel("EjectStudentPanel");
end

function EjectStudentScroll()
	local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:EjectScroll(2);
	end
end

function EjectStudentScrollUp()
	local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:EjectScroll(0);	      	
	end
end

function EjectStudentScrollDown()
    local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:EjectScroll(1);
	end
end

function SelectEjectStudent(index)
	local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:SelectEject(index);	      	
	end
end

function SelectEjectStudent00()
	SelectEjectStudent(0);
end

function SelectEjectStudent01()
    SelectEjectStudent(1);
end

function SelectEjectStudent02()
	SelectEjectStudent(2);
end

function SelectEjectStudent03()
	SelectEjectStudent(3);
end

function DBLCLKEjectStudent(index)
	local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:DBClickEject(index);
		--CloseEjectStudentPanel();	      	
	end
end

function DBLCLKEjectStudent00()
	DBLCLKEjectStudent(0);
end

function DBLCLKEjectStudent01()
	DBLCLKEjectStudent(1);
end

function DBLCLKEjectStudent02()
	DBLCLKEjectStudent(2);
end

function DBLCLKEjectStudent03()
	DBLCLKEjectStudent(3);
end

function EjectStudentButtonOK()
	local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:EjectStudentButtonOK();
		--CloseEjectStudentPanel();	      	
	end
end

function EjectStudentButtonCancel()
	CloseEjectStudentPanel();
end

function EjectStudent()
	LuaUI.CloseQuestionBox();
	local ejectProcess = LuaUI.GetEjectStudentProcess();
	if ejectProcess ~= nil then
		ejectProcess:EjectStudent();
		CloseEjectStudentPanel();	     	
	end
end