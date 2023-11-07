function OpenQuestionPanel()
	
	local panel = UIManager:GetUIItem("QuestionPanel");

	local panelExplain = UIManager:GetUIItem("QuestionExplainPanel");
	if panel == nil and panelExplain == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\QuestionExplain.xml");
	end
	
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\Question.xml");
	end
	LuaUI.SetHotKeyEnable(false);
	OnQuestion_ResetBt()
end

function CloseQuestionPanel()
	LuaUI.SetHotKeyEnable(true);
	local panel = UIManager:GetUIItem("QuestionPanel");
	if panel ~= nil then 
		UIManager:DeletePanel("QuestionPanel");
	end
	
	panel = UIManager:GetUIItem("QuestionExplainPanel");
	if panel ~= nil then 
		UIManager:DeletePanel("QuestionExplainPanel");
	end
end
function OnQuestion_Close()
	CloseQuestionPanel();
	local QuestionProcess = LuaUI.GetQuestionProcess();
	if QuestionProcess ~= nil then
		QuestionProcess:ExitQuestion();
	end
end

function OnQuestion_Explain()
	local panel = UIManager:GetUIItem("QuestionExplainPanel");
	
	if panel == nil then
	    LuaUI.AddXMLFile2UIManger("Data\\QuestionExplain.xml");
	else
	    UIManager:StopDrag();
			UIManager:DeletePanel("QuestionExplainPanel");
	end
	
end

function OnQuestion_ResetBt()
	Question_EnableBt("Question_OptionButton1");
	Question_EnableBt("Question_OptionButton2");
	Question_EnableBt("Question_OptionButton3");
	Question_EnableBt("Question_OptionButton4");
end

function Question_DisableBt(ButtonName)
	local button = UIManager:FindUIButton(ButtonName);
	local tempButton = UIManager:GetUIItem(ButtonName);
	if button ~= nil and tempButton ~= nil then
		button:SetDisableState();
		UIManager:AddDirtyUIItem(tempButton);
	end
end

function Question_EnableBt(ButtonName)
	local button = UIManager:FindUIButton(ButtonName);
	local tempButton = UIManager:GetUIItem(ButtonName);
	if button ~= nil and tempButton ~= nil then
		button:SetNormalState();
		UIManager:AddDirtyUIItem(tempButton);
	end
end

function OnQuestion_OptionButton1()
	Question_DisableBt("Question_OptionButton2");
	Question_DisableBt("Question_OptionButton3");
	Question_DisableBt("Question_OptionButton4");
	Question_EnableBt("Question_OptionButton1");
	local QuestionProcess = LuaUI.GetQuestionProcess();
	if QuestionProcess ~= nil then
		QuestionProcess:Answer(1);
	end
end

function OnQuestion_OptionButton2()
	Question_DisableBt("Question_OptionButton1");
	Question_DisableBt("Question_OptionButton3");
	Question_DisableBt("Question_OptionButton4");
	Question_EnableBt("Question_OptionButton2");
	local QuestionProcess = LuaUI.GetQuestionProcess();
	if QuestionProcess ~= nil then
		QuestionProcess:Answer(2);
	end
end

function OnQuestion_OptionButton3()
	Question_DisableBt("Question_OptionButton1");
	Question_DisableBt("Question_OptionButton2");
	Question_DisableBt("Question_OptionButton4");
	Question_EnableBt("Question_OptionButton3");
	local QuestionProcess = LuaUI.GetQuestionProcess();
	if QuestionProcess ~= nil then
		QuestionProcess:Answer(3);
	end
end

function OnQuestion_OptionButton4()
	Question_DisableBt("Question_OptionButton1");
	Question_DisableBt("Question_OptionButton2");
	Question_DisableBt("Question_OptionButton3");
	Question_EnableBt("Question_OptionButton4");
	
	local QuestionProcess = LuaUI.GetQuestionProcess();
	if QuestionProcess ~= nil then
		QuestionProcess:Answer(4);
	end
end

function OpenQuestion()
	local QuestionProcess = LuaUI.GetQuestionProcess();
	if QuestionProcess ~= nil then
		QuestionProcess:EnterQuestion();
	end
	LuaUI.CloseQuestionBox();
end

function CloseQuestion()
	LuaUI.CloseQuestionBox();
end

function AnswerYes()
  LuaUI.AnswerCreateMasterAndStudent(true);
  LuaUI.CloseQuestionBox();
end

function AnswerNo()
  LuaUI.AnswerCreateMasterAndStudent(false);
  LuaUI.CloseQuestionBox();
end

function AnswerBetrayYes()
  LuaUI.AnswerBetrayMaster(true);
  LuaUI.CloseQuestionBox();
end

function AnswerBetrayNo()
  LuaUI.AnswerBetrayMaster(false);
  LuaUI.CloseQuestionBox();
end

function HideAnswerResultImage()
  local pAniIcon = UIManager:FindUIAniIcon("AnswerResultImage");
  if pAniIcon then 
  	pAniIcon:SetEnable(false);
  end
end