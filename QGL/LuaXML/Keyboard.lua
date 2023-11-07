shiftFlag = 0;
lockFlag = 0;

function RandomKeyPosition()
	if _G.lockFlag == 0 then
	    arrayX = {4, 28, 52, 76, 100, 124, 148, 172, 196, 220, 244, 268, 292}
		arrayY = {6, 31, 56, 83}
		startX = math.floor(math.random(1,12));
		startY = math.floor(math.random(1,3));

		if (startY == 4) then
		    if (startX < 5) then
		        startX = 5;
			end
		end

		KeyEng = 26;
		KeyNum = 10;
		KeySymbol = 11;

		count = 0;
		countEng = 1;
		countNum = 0;
		countSymbol = 1;

		while count ~= 47 do

		    if (countEng < (KeyEng + 1 )) then
				buttonName = "KeyEng"..countEng;
				countEng = countEng + 1;
			elseif (countNum < KeyNum) then
			    buttonName = "KeyNum"..countNum;
			    countNum = countNum + 1;
			elseif (countSymbol < (KeySymbol + 1 )) then
			    buttonName = "KeySymbol"..countSymbol;
			    countSymbol = countSymbol + 1;
			end
			button = UIManager:FindUIButton(buttonName);
			posX = arrayX[startX];
			posY = arrayY[startY];
			
			for i=0,3 do
				button:SetSubOffSet(i, posX, posY);
			end
			button:SetOffsetXY(posX, posY);
			
			startX = startX + 1;
			if startX > 13 then
			   startX = 1;

			   startY = startY + 1;
				if startY > 4 then
			    	startY = 1;
				elseif startY == 4 then
				    startX = 6;
				end
			end

		    count = count + 1;
		end
	 	UIManager:RenderAll();
	end
end


function AppendString(char)
	if _G.selectField == 1 then
		editFieldName = "LoginName";
	elseif _G.selectField == 2 then
	    editFieldName = "Password"
	elseif _G.selectField == 3 then
	    editFieldName = "AssetUnLockPasswordEdit"
	elseif _G.selectField == 4 then
	    editFieldName = "AssetNewLockPasswordEdit11"
	elseif _G.selectField == 5 then
	    editFieldName = "AssetNewLockPasswordEdit12"
	elseif _G.selectField == 6 then
	    editFieldName = "AssetOldLockPasswordEdit"
	elseif _G.selectField == 7 then
	    editFieldName = "AssetNewLockPasswordEdit21"
	elseif _G.selectField == 8 then
	    editFieldName = "AssetNewLockPasswordEdit22"
	end
	local editField = UIManager:FindUIEditBox(editFieldName);
	if editField ~= nil then
		editField:InsertText(char);
	 	editField:Redraw();

		local temp = UIManager:GetUIItem(editFieldName);
		UIManager:AddDirtyUIItem(temp);
	end
end

function InsertA()
	if shiftFlag == 0 then
   		AppendString("a");
	else
	    AppendString("A");
	end
	
   RandomKeyPosition();
end

function InsertB()
	if shiftFlag == 0 then
   		AppendString("b");
	else
	    AppendString("B");
	end
   RandomKeyPosition();
end

function InsertC()

	if shiftFlag == 0 then
   		AppendString("c");
	else
	    AppendString("C");
	end
   RandomKeyPosition();
end

function InsertD()
    if shiftFlag == 0 then
   		AppendString("d");
	else
	    AppendString("D");
	end

   RandomKeyPosition();
end

function InsertE()
    if shiftFlag == 0 then
   		AppendString("e");
	else
	    AppendString("E");
	end

   RandomKeyPosition();
end

function InsertF()
    if shiftFlag == 0 then
   		AppendString("f");
	else
	    AppendString("F");
	end

   RandomKeyPosition();
end

function InsertG()
    if shiftFlag == 0 then
   		AppendString("g");
	else
	    AppendString("G");
	end

   RandomKeyPosition();
end

function InsertH()
    if shiftFlag == 0 then
   		AppendString("h");
	else
	    AppendString("H");
	end

   RandomKeyPosition();
end

function InsertI()
    if shiftFlag == 0 then
   		AppendString("i");
	else
	    AppendString("I");
	end

   RandomKeyPosition();
end

function InsertJ()
    if shiftFlag == 0 then
   		AppendString("j");
	else
	    AppendString("J");
	end

   RandomKeyPosition();
end

function InsertK()
    if shiftFlag == 0 then
   		AppendString("k");
	else
	    AppendString("K");
	end

   RandomKeyPosition();
end

function InsertL()
    if shiftFlag == 0 then
   		AppendString("l");
	else
	    AppendString("L");
	end

   RandomKeyPosition();
end

function InsertM()
    if shiftFlag == 0 then
   		AppendString("m");
	else
	    AppendString("M");
	end

   RandomKeyPosition();
end

function InsertN()

    if shiftFlag == 0 then
   		AppendString("n");
	else
	    AppendString("N");
	end

   RandomKeyPosition();
end

function InsertO()
    if shiftFlag == 0 then
   		AppendString("o");
	else
	    AppendString("O");
	end

   RandomKeyPosition();
end

function InsertP()
    if shiftFlag == 0 then
   		AppendString("p");
	else
	    AppendString("P");
	end

   RandomKeyPosition();
end

function InsertQ()
    if shiftFlag == 0 then
   		AppendString("q");
	else
	    AppendString("Q");
	end

   RandomKeyPosition();
end

function InsertR()
    if shiftFlag == 0 then
   		AppendString("r");
	else
	    AppendString("R");
	end

   RandomKeyPosition();
end

function InsertS()
    if shiftFlag == 0 then
   		AppendString("s");
	else
	    AppendString("S");
	end

   RandomKeyPosition();
end

function InsertT()
    if shiftFlag == 0 then
   		AppendString("t");
	else
	    AppendString("T");
	end

   RandomKeyPosition();
end

function InsertU()
    if shiftFlag == 0 then
   		AppendString("u");
	else
	    AppendString("U");
	end

   RandomKeyPosition();
end

function InsertV()
    if shiftFlag == 0 then
   		AppendString("v");
	else
	    AppendString("V");
	end

   RandomKeyPosition();
end


function InsertW()
    if shiftFlag == 0 then
   		AppendString("w");
	else
	    AppendString("W");
	end

   RandomKeyPosition();
end

function InsertX()
    if shiftFlag == 0 then
   		AppendString("x");
	else
	    AppendString("X");
	end

   RandomKeyPosition();
end

function InsertY()
    if shiftFlag == 0 then
   		AppendString("y");
	else
	    AppendString("Y");
	end

   RandomKeyPosition();
end

function InsertZ()
    if shiftFlag == 0 then
   		AppendString("z");
	else
	    AppendString("Z");
	end

   RandomKeyPosition();
end

function Insert0()
    if shiftFlag == 0 then
   		AppendString("0");
	else
	    AppendString(")");
	end

   RandomKeyPosition();
end

function Insert1()
    if shiftFlag == 0 then
   		AppendString("1");
	else
	    AppendString("!");
	end

   RandomKeyPosition();
end

function Insert2()
    if shiftFlag == 0 then
   		AppendString("2");
	else
	    AppendString("@");
	end

   RandomKeyPosition();
end

function Insert3()
    if shiftFlag == 0 then
   		AppendString("3");
	else
	    AppendString("#");
	end

   RandomKeyPosition();
end

function Insert4()
    if shiftFlag == 0 then
   		AppendString("4");
	else
	    AppendString("$");
	end

   RandomKeyPosition();
end

function Insert5()
    if shiftFlag == 0 then
   		AppendString("5");
	else
	    AppendString("%");
	end

   RandomKeyPosition();
end

function Insert6()
    if shiftFlag == 0 then
   		AppendString("6");
	else
	    AppendString("^");
	end

   RandomKeyPosition();
end

function Insert7()
    if shiftFlag == 0 then
   		AppendString("7");
	else
	    AppendString("&");
	end
   RandomKeyPosition();
end

function Insert8()
    if shiftFlag == 0 then
   		AppendString("8");
	else
	    AppendString("*");
	end

   RandomKeyPosition();
end

function Insert9()
    if shiftFlag == 0 then
   		AppendString("9");
	else
	    AppendString("(");
	end
    RandomKeyPosition();
end

function InsertSymbol1()

	if shiftFlag == 0 then
   		AppendString("]");
	else
	    AppendString("}");
	end

   RandomKeyPosition();
end

function InsertSymbol2()
    if shiftFlag == 0 then
   		AppendString("`");
	else
	    AppendString("~");
	end

   RandomKeyPosition();
end

function InsertSymbol3()
    if shiftFlag == 0 then
   		AppendString(";");
	else
	    AppendString(":");
	end

   RandomKeyPosition();
end

function InsertSymbol4()
   if shiftFlag == 0 then
   		AppendString("=");
	else
	    AppendString("+");
	end
   RandomKeyPosition();
end


function InsertSymbol5()
    if shiftFlag == 0 then
   		AppendString("[");
	else
	    AppendString("{");
	end

   RandomKeyPosition();
end

function InsertSymbol6()
    if shiftFlag == 0 then
   		AppendString("'");
	else
	    AppendString("\"");
	end

   RandomKeyPosition();
end

function InsertSymbol7()
    if shiftFlag == 0 then
   		AppendString(",");
	else
	    AppendString("<");
	end

   RandomKeyPosition();
end

function InsertSymbol8()
    if shiftFlag == 0 then
   		AppendString("-");
	else
	    AppendString("_");
	end

   RandomKeyPosition();
end

function InsertSymbol9()
    if shiftFlag == 0 then
   		AppendString(".");
	else
	    AppendString(">");
	end

   RandomKeyPosition();
end

function InsertSymbol10()
    if shiftFlag == 0 then
   		AppendString("/");
	else
	    AppendString("?");
	end

   RandomKeyPosition();
end

function InsertSymbol11()
    if shiftFlag == 0 then
   		AppendString("\\");
	else
	    AppendString("|");
	end

   RandomKeyPosition();
end

function EnableShift()
   	if _G.shiftFlag == 0 then
        _G.shiftFlag = 1;
	else
	    _G.shiftFlag = 0;
	end
end

function EnableLock()
   if _G.lockFlag == 0 then
        _G.lockFlag = 1;
   else
        _G.lockFlag = 0;
	end
end

function EnableBackSpace()
    if _G.selectField == 1 then
		editFieldName = "LoginName";
	elseif _G.selectField == 2 then
	    editFieldName = "Password"
	end
	
	local editField = UIManager:FindUIEditBox(editFieldName);
	if editField ~= nil then
		editField:BackSpace();
		editField:Redraw();   
		local temp = UIManager:GetUIItem(editFieldName);
		UIManager:AddDirtyUIItem(temp);
	end


end