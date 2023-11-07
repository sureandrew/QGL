function PlaySoundEffect(word)
	local sfx = FSoundManager:CreateSoundA(word);
	local systemSetting = LuaUI.GetMenuSystemSetting();
	if systemSetting ~= nil and sfx ~= nil then
		if systemSetting:IsSoundEffectModeEnable() then
			sfx:SetVolume(systemSetting:GetSoundEffectVolForFmod());
		else
			sfx:SetVolume(0);
		end
		sfx:Play();	
	end
end

function PlayOpenInterfaceEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0001_open.wav");
end

function PlayCloseInterfaceEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0002_close.wav");
end

function PlayDragItemUpEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0003_take.wav");
end

function PlayPointAddEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0019_point_add.wav");
end

function PlayPointDecreaseEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0020_point_decrease.wav");
end

function PlayClickEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0005_click.wav");
end

function PlayConfirmPointEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0026_atk_command.wav");
end

function PlayDropItemEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0007_drop.wav");
end

function PlayBuyItemEffect()
	PlaySoundEffect("Data\\Wav\\SoundE0004_buy_1.wav");
end

function PlayBuyOnlineShopItemEffect()
	PlaySoundEffect("Data\\Wav\\SoundE0005_buy_2.wav");
end

function PlaySellItemEffect()
	PlaySoundEffect("Data\\Wav\\SoundE0006_money.wav");
end

function PlayFinishGivenEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0008_trade_success.wav");
end
   
function PlayWarningEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0010_warning2.wav");
end

function PlayBigMapDesEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0028_destination.wav");
end

function PlayCreateTeamEffect()
	PlaySoundEffect("Data\\Wav\\SoundE0034_CreateTeam.wav");
end

function PlayPartnerLevelUpEffect()
	PlaySoundEffect("Data\\Wav\\SoundE0010_lv_up_partner.wav");
end

function PlayAddTeamEffect()
	PlaySoundEffect("Data\\Wav\\SoundE0008_add_team.wav");
end

function PlayCantEquipEffect()
	PlaySoundEffect("Data\\Wav\\SoundI0025_cant_equip.wav");
end

function PlayCapScreenEffect()
	PlaySoundEffect("Data\\Wav\\SoundE0018_screen_shot.wav");
end

function PlayPutItemEffect()
    PlaySoundEffect("Data\\Wav\\SoundI0004_put.wav");
end

function PlayEqWeaponEffect()
    PlaySoundEffect("Data\\Wav\\SoundI0013_equip_weapon.wav");
end

function PlayOffEqWeaponEffect()
    PlaySoundEffect("Data\\Wav\\SoundI0014_off_weapon.wav");
end

function PlayEqClothEffect()
    PlaySoundEffect("Data\\Wav\\SoundI0015_equip_cloth.wav");
end

function PlayOffEqClothEffect()
    PlaySoundEffect("Data\\Wav\\SoundI0016_off_cloth.wav");
end

function PlayEqOtherEffect()
    PlaySoundEffect("Data\\Wav\\SoundI0017_equip_other.wav");
end

function PlayOffEqOtherEffect()
    PlaySoundEffect("Data\\Wav\\SoundI0018_off_other.wav");
end

function PlayUseItemEffect()
    PlaySoundEffect("Data\\Wav\\SoundE0001_use_item_1.wav");
end