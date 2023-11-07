function ChatBalloonPartyClose1(pX, pY)
	ChatBalloonPartyClose("ChatBalloonParty1");
end

function ChatBalloonPartyClose2(pX, pY)
	ChatBalloonPartyClose("ChatBalloonParty2");
end

function ChatBalloonPartyClose3(pX, pY)
	ChatBalloonPartyClose("ChatBalloonParty3");
end

function ChatBalloonPartyClose4(pX, pY)
	ChatBalloonPartyClose("ChatBalloonParty4");
end

function ChatBalloonPartyClose(componentName)
	local panel = UIManager:GetUIItem(componentName)
	if panel ~= nil then
		panel:SetEnable(false);
		UIManager:AddDirtyUIItem(panel);
	end
end
