CreateCharSex = 0; -- -1: haven't choosen 0:male 1:female
CreateCharClass = 1; -- 0: haven't choosen 1:paladin 2:thief 3:doctor 4:assassin
CreateCharHairStyle = 1;
CreateCharHairColor = 1;
CreateCharCloth = 1;
CreateCharacter = -1;

function InitCreateCharacter()
	_G.CreateCharSex = 0; 
	_G.CreateCharClass = 4;
	_G.CreateCharHairStyle = 1;
	_G.CreateCharHairColor = 1;
	_G.CreateCharCloth = 1;
end


function HairStyleNext()
    LuaUI.ChangeCharacterHead(true);
end

function HairStylePrev()
 	LuaUI.ChangeCharacterHead(false);
end

function HairColorNext()
	LuaUI.ChangeCharacterHairCol(true);
end

function HairColorPrev()
    LuaUI.ChangeCharacterHairCol(false);
end

function ClothNext()
	LuaUI.ChangeCharacterCloth(true);
end

function ClothPrev()
	LuaUI.ChangeCharacterCloth(false);
end

function ClothColorNext()
	LuaUI.ChangeCharacterClothCol(true);
end

function ClothColorPrev()
	LuaUI.ChangeCharacterClothCol(false);
end

function CreateCharacterPrev()
	CloseCreateCharacterPanel();
	local choosenCharacterPanel = UIManager:GetUIItem("CharacterChosenPanel");
	choosenCharacterPanel:SetEnable(true);
	UIManager:RenderAll();
	UIManager:ChangeFocus("LoginName");
end

function CreateCharacterNext()
	local nickName = UIManager:FindUIEditBox("CharacterName");
	if nickName ~= nil then

	    if nickName:Empty() == false then
	        LuaUI.CreateCharacter();
			--CloseCreateCharacterPanel();
			--local choosenCharacterPanel = UIManager:GetUIItem("CharacterChosenPanel");
			--choosenCharacterPanel:SetEnable(true);
			--UIManager:RenderAll();
			UIManager:ChangeFocus("LoginName");
		end
	end
end

function CheckCharacterName()
	local nickName = UIManager:FindUIEditBox("CharacterName");
	if nickName ~= nil then

	    if nickName:Empty() == false then
	        LuaUI.CheckCharacterName();
		end
	end
end

function CloseCreateCharacterPanel()
	local createCharacterPanel = UIManager:GetUIItem("CreateCharacterPanel");
	if createCharacterPanel ~= nil then
		UIManager:DeletePanel("CreateCharacterPanel");
	end
	
	--[[local lImagePanel = UIManager:GetUIItem("CharacterLImagePanel");
	if lImagePanel ~= nil then
	    UIManager:DeletePanel("CharacterLImagePanel");
	end
	]]
	_G.CreateCharacter = -1;
	InitCreateCharacter();
	UIManager:RenderAll();
end

function SetCharSex(index)
	_G.CreateCharSex = index;
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass);
end

function SetCharClass(index)
	_G.CreateCharClass = index;
	local paladin = UIManager:FindUICheckBox("Paladin");
	local assassin = UIManager:FindUICheckBox("Assassin");
	local thief = UIManager:FindUICheckBox("Thief");
	local doctor = UIManager:FindUICheckBox("Doctor");
	
	if index ~= 1 then
		paladin:SetState(false);
	else 
		paladin:SetState(true);
	end
	
	if index ~= 4 then
		assassin:SetState(false);
	else
		assassin:SetState(true);
	end
	
	if index ~= 2 then
		thief:SetState(false);
	else
		thief:SetState(true);
	end
	
	if index ~= 3 then
		doctor:SetState(false);
	else
		doctor:SetState(true);
	end
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );
end


function SetCharacter1()
    _G.CreateCharSex = 0;
	_G.CreateCharClass = 1;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );
end

function SetCharacter2()
    _G.CreateCharSex = 1;
	_G.CreateCharClass = 1;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );

end

function SetCharacter3()
    _G.CreateCharSex = 0;
	_G.CreateCharClass = 2;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );

end

function SetCharacter4()
    _G.CreateCharSex = 1;
	_G.CreateCharClass = 2;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );

end

function SetCharacter5()
    _G.CreateCharSex = 0;
	_G.CreateCharClass = 3;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );

end

function SetCharacter6()
    _G.CreateCharSex = 1;
	_G.CreateCharClass = 3;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );

end

function SetCharacter7()

	_G.CreateCharSex = 0;
	_G.CreateCharClass = 4;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );
end

function SetCharacter8()

	_G.CreateCharSex = 1;
	_G.CreateCharClass = 4;
	DisableAllImage();
	LuaUI.ShowCreateCharacter(_G.CreateCharSex, _G.CreateCharClass );
end

function DisableImage( prefName, startIndex, endIndex)
	local img;
	local name;
	
	for i=startIndex, endIndex do
		name = prefName..i;
	    img = UIManager:GetUIItem(name);
	    if img ~= nil then
	        img:SetEnable(false);
	    end
	end
end

function DisableAllImage()
	DisableImage("CharacterLImage", 1, 8);
	DisableImage("CharDefautName", 1, 8);
	DisableImage("HairStyle", 1, 10);
	DisableImage("HairColor", 1, 10);
	DisableImage("Cloth", 1, 10);
	DisableImage("ClothColor", 1, 10);
end

function HeadPicHighLightOff()
	local board = UIManager:GetUIItem("HeadBoard");
	if board ~= nil then
		board:SetEnable(false);
	end
end 

function HeadPicHighLight(index)
	local board = UIManager:GetUIItem("HeadBoard");
	local x;
	local y;
	local image = UIManager:GetUIItem("HeadPic"..index);
	
	if image ~= nil and board ~= nil then
		x = image:GetOffsetX() - 3;
		y = image:GetOffsetY() - 2;
		
		if x ~= board:GetOffsetX() or y ~= board:GetOffsetY() then
			board:SetOffsetXY(x, y);
		end
		
		if board:IsEnable() ~= true then
			board:SetEnable(true);
		end
	end
end

function HeadPicHighLight1()
	HeadPicHighLight(1);
end

function HeadPicHighLight2()
	HeadPicHighLight(2);
end

function HeadPicHighLight3()
	HeadPicHighLight(3);
end

function HeadPicHighLight4()
	HeadPicHighLight(4);
end

function HeadPicHighLight5()
	HeadPicHighLight(5);
end

function HeadPicHighLight6()
	HeadPicHighLight(6);
end

function HeadPicHighLight7()
	HeadPicHighLight(7);
end

function HeadPicHighLight8()
	HeadPicHighLight(8);
end

function CreateCharRotateLeft()
	LuaUI.CreateCharRotate(false);
end

function CreateCharRotateRight()
    LuaUI.CreateCharRotate(true);
end



