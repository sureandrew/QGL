UIManager = LuaUI.GetUIManager()
--OggPlayer = LuaUI.GetOggPlayer();
FSoundManager = LuaUI.GetFSoundManager();

SubUIManager = LuaUI.GetSubUIManager()

EditBox = UIManager:FindUIEditBox("EditLine");
MyChat = UIManager:FindUIChatPanel("Chat")
MyPop = UIManager:FindUIPopUpMenu("TestPop")

local SubBgIndex = 0;
function InitSubWindow()
	local SubWinUIManger = LuaUI.GetSubUIManager()
		
	if SubWinUIManger ~= nil then
		SubWinUIManger:FindUICheckBox("SubChannelMapButton"):SetState(not LuaUI.GetLisentToChannel(0));
		SubWinUIManger:FindUICheckBox("SubChannelPartyButton"):SetState(not LuaUI.GetLisentToChannel(1));
		SubWinUIManger:FindUICheckBox("SubChannelGuildButton"):SetState(not LuaUI.GetLisentToChannel(2));
		SubWinUIManger:FindUICheckBox("SubChannelGangButton"):SetState(not LuaUI.GetLisentToChannel(3));
		SubWinUIManger:FindUICheckBox("SubChannelPrivateButton"):SetState(not LuaUI.GetLisentToChannel(4));
		SubWinUIManger:FindUICheckBox("SubChannelGlobalButton"):SetState(not LuaUI.GetLisentToChannel(5));
		SubWinUIManger:FindUICheckBox("SubChannelCoupleButton"):SetState(not LuaUI.GetLisentToChannel(6));
		SubWinUIManger:RenderAll();
	end
end
function SubChatUp()
	local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubChat = SubWinUIManger:FindUIChatPanel("SubChat");
		SubChat:UpRow(); 
		SubChat:LockChatPanel();
		SubWinUIManger:RenderAll();
	end
end

function SubChatScrollLua()
	local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubChat = SubWinUIManger:FindUIChatPanel("SubChat"); 
		SubChat:LockChatPanel();
	end
	LuaUI.SubChatScroll();
end

function SubChatDown()
	local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubChat = SubWinUIManger:FindUIChatPanel("SubChat");
		SubChat:DownRow();
		SubChat:LockChatPanel(); 
		SubWinUIManger:RenderAll();
	end
end

function Blank()
end

function SubChannelMap()
	local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubMap = SubWinUIManger:FindUICheckBox("SubChannelMapButton");
		
		if SubMap:GetState() then
			LuaUI.RejectChannel(0)
		else
			LuaUI.ReceiveChannel(0)
		end
		
		SubWinUIManger:RenderAll();
	end
end
function SubChannelParty()
local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubMap = SubWinUIManger:FindUICheckBox("SubChannelPartyButton");
		
		if SubMap:GetState() then
			LuaUI.RejectChannel(1)
		else
			LuaUI.ReceiveChannel(1)
		end
		
		SubWinUIManger:RenderAll();
	end
end

function SubChannelGang()
local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubMap = SubWinUIManger:FindUICheckBox("SubChannelGangButton");
		
		if SubMap:GetState() then
			LuaUI.RejectChannel(2)
		else
			LuaUI.ReceiveChannel(2)
		end
		
		SubWinUIManger:RenderAll();
	end
end

function SubChannelGuild()
local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubMap = SubWinUIManger:FindUICheckBox("SubChannelGuildButton");
		
		if SubMap:GetState() then
			LuaUI.RejectChannel(3)
		else
			LuaUI.ReceiveChannel(3)
		end
		
		SubWinUIManger:RenderAll();
	end
end

function SubChannelPrivate()
local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubMap = SubWinUIManger:FindUICheckBox("SubChannelPrivateButton");
		
		if SubMap:GetState() then
			LuaUI.RejectChannel(4)
		else
			LuaUI.ReceiveChannel(4)
		end
		
		SubWinUIManger:RenderAll();
	end
end
function SubChannelGlobal()
local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubMap = SubWinUIManger:FindUICheckBox("SubChannelGlobalButton");
		
		if SubMap:GetState() then
			LuaUI.RejectChannel(5)
		else
			LuaUI.ReceiveChannel(5)
		end
		
		SubWinUIManger:RenderAll();
	end
end

function SubChannelCouple()
local SubWinUIManger = LuaUI.GetSubUIManager()
	if SubWinUIManger ~= nil then
		local SubMap = SubWinUIManger:FindUICheckBox("SubChannelCoupleButton");
		
		if SubMap:GetState() then
			LuaUI.RejectChannel(6)
		else
			LuaUI.ReceiveChannel(6)
		end
		
		SubWinUIManger:RenderAll();
	end
end

function ClearSubChatMessage()
local SubWinUIManger = LuaUI.GetSubUIManager()
local SubChat = SubWinUIManger:FindUIChatPanel("SubChat");
	SubChat:ClearAllRow()
	LuaUI.GetChatPool():ResetCopyIndex();
	SubWinUIManger:RenderAll();
end

function SubEndRow(pX, pY)
	local SubWinUIManger = LuaUI.GetSubUIManager()
	local SubChat = SubWinUIManger:FindUIChatPanel("SubChat");
	local box = SubWinUIManger:FindUICheckBox("SubChatEnableScroll");
	SubChat:UnLockChatPanel();
	SubChat:EndRow();

	local scrollBar = UIManager:FindUIScrollBarV("SubChatScrollButton");
	if scrollBar ~= nil then
		ScrollBar:SetNumCount(SubChat:RowListCount())
		scrollBar:EndScroll();
	end
end

function SubChatMouseWheel(pX, pY, pZ)
	local SubWinUIManger = LuaUI.GetSubUIManager()
	local scrollBar = SubWinUIManger:FindUIScrollBarV("SubChatScrollButton");
	if scrollBar ~= nil then
		if pZ > 0 then
			scrollBar:MoveUp()
		else
			scrollBar:MoveDown()
		end
	end
	SubWinUIManager:RenderAll();
end

function ChangeSubChatBg()
	
	local SubWinUIManger = LuaUI.GetSubUIManager()
		
	if SubWinUIManger ~= nil then
			SubBgIndex = SubBgIndex + 1;
			if SubBgIndex > 4 then
				SubBgIndex = 0;
			end
			
			if SubBgIndex == 0 then
			SubWinUIManger:SetBackImage(SubWinUIManger:GetBackImage(), 364, 171, 433, 312);
			elseif SubBgIndex == 1 then
			SubWinUIManger:SetBackImage(SubWinUIManger:GetBackImage(), 437, 171, 506, 312);
			elseif SubBgIndex == 2 then
			SubWinUIManger:SetBackImage(SubWinUIManger:GetBackImage(), 364, 316, 433, 457);
			elseif SubBgIndex == 3 then
			SubWinUIManger:SetBackImage(SubWinUIManger:GetBackImage(), 437, 316, 506, 457);
			elseif SubBgIndex == 4 then
			SubWinUIManger:SetBackImage(SubWinUIManger:GetBackImage(), 291, 171, 360, 312);
			end
			
			SubWinUIManger:RenderAll();
	end
	
end


function SubChatLinkProcess(pX, pY)
	local SubWinUIManager = LuaUI.GetSubUIManager()
	local SubChat = SubWinUIManager:FindUIChatPanel("SubChat")
	local ChatLink = SubChat:GetLinkItem()
	local FloatNamePanel = UIManager:FindUIPanel("FloatNamePanel")
	local SubFloatNamePanel = SubWinUIManager:FindUIPanel("SubFloatNamePanel")
	
	if ChatLink ~= nil and FloatNamePanel ~= nil then 
		if ChatLink:GetLinkType() == 1 then
			local Float = UIManager:FindUIChatPanel("FloatNameStr")
			FloatNamePanel:SetEnable(false)
			Float:ClearAllRow();
			g_LinkString = ChatLink:GetLinkSubString(5)
			Float:AddStringW(g_LinkString);
			Float:HomeRow()
			Float:Redraw()
			

			local Row = Float:GetHomeRow();
			local Width = Row:GetWidth() + 2;

			FloatNamePanel:SetWH(Width, 20);
			FloatNamePanel:SetTextureWH(Width, 20);
			FloatNamePanel:SetTextureUI(UIManager);
			FloatNamePanel:SetDetectRect();
			
			UIManager:RenderAll();
			UIManager:SetDragItemXY(FloatNamePanel, 0,  0);
			UIManager:AddDirtyUIItem(FloatNamePanel);
			
			Float = SubWinUIManager:FindUIChatPanel("SubFloatNameStr")
			SubFloatNamePanel:SetEnable(true)
			Float:ClearAllRow();
			g_LinkString = ChatLink:GetLinkSubString(5)
			Float:AddStringW(g_LinkString);
			Float:HomeRow()
			Float:Redraw()
			
			SubFloatNamePanel:SetWH(Width, 20);
			SubFloatNamePanel:SetTextureWH(Width, 20);
			SubFloatNamePanel:SetTextureUI(SubWinUIManager);
			SubFloatNamePanel:SetDetectRect();
			
			SubWinUIManager:RenderAll();
			SubWinUIManager:SetDragItemXY(SubFloatNamePanel, 0,  0);
			SubWinUIManager:AddDirtyUIItem(SubFloatNamePanel);
			
		elseif ChatLink:GetLinkType() == 3 then
			LuaUI.CharSetMapPos(ChatLink:GetLinkSubString(5));
		elseif ChatLink:GetLinkType() == 4 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local ItemID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);
			
			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ShowPublicItemDetail(CharID, ItemID);
			end
		elseif ChatLink:GetLinkType() == 5 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local PartnerID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);
			
			local PartnerProcess = LuaUI.GetPartnerProcess();
			if  PartnerProcess ~= nil then
				 PartnerProcess:ShowPublicPartnerDetail(CharID, PartnerID);
			end
		elseif ChatLink:GetLinkType() == 6 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local PetID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);
			
			local PetProcess = LuaUI.GetMenuPetProcess();
			if  PetProcess ~= nil then
				 PetProcess:ShowPublicPetDetail(CharID, PetID);
			end
		end
	end
end


function SubChatLinkProcessR(pX, pY)
	local SubWinUIManager = LuaUI.GetSubUIManager()
	local SubChat = SubWinUIManager:FindUIChatPanel("SubChat")
	local ChatLink = SubChat:GetLinkItem()
	local FloatNamePanel = UIManager:FindUIPanel("FloatNamePanel")
	
	
	
	if ChatLink ~= nil and FloatNamePanel ~= nil then 
		if ChatLink:GetLinkType() == 1 then
			local menu = LuaUI.GetMenuTargetProcess();
			g_LinkString = ChatLink:GetLinkSubString(5)
			menu:FindCharNameFriend(g_LinkString);
			
		elseif ChatLink:GetLinkType() == 4 then
			g_LinkString = ChatLink:GetLinkSubString(5)
			local ItemID = LuaUI.GetFirstNum(g_LinkString);
			local CharID = LuaUI.GetLastNum(g_LinkString);
			
			local itemProcess = LuaUI.GetItemProcess();
			if itemProcess ~= nil then
				itemProcess:ShowPublicItemDetail(CharID, ItemID);
			end
		end
	end
end


