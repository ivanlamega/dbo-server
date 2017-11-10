#include "AttributeManager.h"
#include "GameNetwork.h"
#include "NtlAvatar.h"

#include <ppl.h>

AttributeManager::AttributeManager()
{
}

AttributeManager::~AttributeManager()
{
}
/*
By Luiz45
He will load Attributes and compress then into a readable field to send a packet
*/
void AttributeManager::LoadAttribute(sAVATAR_ATTRIBUTE* pAttribute)
{

	//Load All Attributes One time only - Luiz  IN ORDER --Kalisto
	pAvatarMaintAttribute.dwBaseMaxLP = pAttribute->dwBaseMaxLP;
	pAvatarMaintAttribute.dwLastMaxLP = pAttribute->dwLastMaxLP;
	pAvatarMaintAttribute.wBaseMaxEP = pAttribute->wBaseMaxEP;
	pAvatarMaintAttribute.wLastMaxEP = pAttribute->wLastMaxEP;
	pAvatarMaintAttribute.wBaseMaxRP = pAttribute->wBaseMaxRP;
	pAvatarMaintAttribute.wLastMaxRP = pAttribute->wLastMaxRP;

	pAvatarMaintAttribute.wBaseStr = pAttribute->wBaseStr;
	pAvatarMaintAttribute.wLastStr = pAttribute->wLastStr;
	pAvatarMaintAttribute.wBaseCon = pAttribute->wBaseCon;
	pAvatarMaintAttribute.wLastCon = pAttribute->wLastCon;
	pAvatarMaintAttribute.wBaseFoc = pAttribute->wBaseFoc;
	pAvatarMaintAttribute.wLastFoc = pAttribute->wLastFoc;
	pAvatarMaintAttribute.wBaseDex = pAttribute->wBaseDex;
	pAvatarMaintAttribute.wLastDex = pAttribute->wLastDex;
	pAvatarMaintAttribute.wBaseSol = pAttribute->wBaseSol;
	pAvatarMaintAttribute.wLastSol = pAttribute->wLastSol;
	pAvatarMaintAttribute.wBaseEng = pAttribute->wBaseEng;
	pAvatarMaintAttribute.wLastEng = pAttribute->wLastEng;

	pAvatarMaintAttribute.wLastRpRegen = pAttribute->wLastRpRegen;
	pAvatarMaintAttribute.wLastRpDimimutionRate = pAttribute->wLastRpDimimutionRate;

	pAvatarMaintAttribute.wBasePhysicalOffence = pAttribute->wBasePhysicalOffence;
	pAvatarMaintAttribute.wLastPhysicalOffence = pAttribute->wLastPhysicalOffence;
	pAvatarMaintAttribute.wBasePhysicalDefence = pAttribute->wBasePhysicalDefence;
	pAvatarMaintAttribute.wLastPhysicalDefence = pAttribute->wLastPhysicalDefence;
	pAvatarMaintAttribute.wBaseEnergyOffence = pAttribute->wBaseEnergyOffence;
	pAvatarMaintAttribute.wLastEnergyOffence = pAttribute->wLastEnergyOffence;
	pAvatarMaintAttribute.wBaseEnergyDefence = pAttribute->wBaseEnergyDefence;
	pAvatarMaintAttribute.wLastEnergyDefence = pAttribute->wLastEnergyDefence;

	pAvatarMaintAttribute.wBaseAttackRate = pAttribute->wBaseAttackRate;
	pAvatarMaintAttribute.wLastAttackRate = pAttribute->wLastAttackRate;
	pAvatarMaintAttribute.wBaseDodgeRate = pAttribute->wBaseDodgeRate;
	pAvatarMaintAttribute.wLastDodgeRate = pAttribute->wLastDodgeRate;
	pAvatarMaintAttribute.wBaseBlockRate = pAttribute->wBaseBlockRate;
	pAvatarMaintAttribute.wLastBlockRate = pAttribute->wLastBlockRate;

	pAvatarMaintAttribute.wBaseCurseSuccessRate = pAttribute->wBaseCurseSuccessRate;
	pAvatarMaintAttribute.wLastCurseSuccessRate = pAttribute->wLastCurseSuccessRate;
	pAvatarMaintAttribute.wBaseCurseToleranceRate = pAttribute->wBaseCurseToleranceRate;
	pAvatarMaintAttribute.wLastCurseToleranceRate = pAttribute->wLastCurseToleranceRate;

	pAvatarMaintAttribute.wBasePhysicalCriticalRate = pAttribute->wBasePhysicalCriticalRate;
	pAvatarMaintAttribute.wLastPhysicalCriticalRate = pAttribute->wLastPhysicalCriticalRate;
	pAvatarMaintAttribute.wBaseEnergyCriticalRate = pAttribute->wBaseEnergyCriticalRate;
	pAvatarMaintAttribute.wLastEnergyCriticalRate = pAttribute->wLastEnergyCriticalRate;

	pAvatarMaintAttribute.wBaseAttackSpeedRate = pAttribute->wBaseAttackSpeedRate;
	pAvatarMaintAttribute.wLastAttackSpeedRate = pAttribute->wLastAttackSpeedRate;
	pAvatarMaintAttribute.fBaseAttackRange = pAttribute->fBaseAttackRange;
	pAvatarMaintAttribute.fLastAttackRange = pAttribute->fLastAttackRange;

	pAvatarMaintAttribute.fLastRunSpeed = pAttribute->fLastRunSpeed;

	pAvatarMaintAttribute.fCastingTimeChangePercent = pAttribute->fCastingTimeChangePercent;
	pAvatarMaintAttribute.fCoolTimeChangePercent = pAttribute->fCoolTimeChangePercent;
	pAvatarMaintAttribute.fKeepTimeChangePercent = pAttribute->fKeepTimeChangePercent;
	pAvatarMaintAttribute.fDotValueChangePercent = pAttribute->fDotValueChangePercent;
	pAvatarMaintAttribute.fDotTimeChangeAbsolute = pAttribute->fDotTimeChangeAbsolute;
	pAvatarMaintAttribute.fRequiredEpChangePercent = pAttribute->fRequiredEpChangePercent;

	pAvatarMaintAttribute.fHonestOffence = pAttribute->fHonestOffence;
	pAvatarMaintAttribute.fHonestDefence = pAttribute->fHonestDefence;
	pAvatarMaintAttribute.fStrangeOffence = pAttribute->fStrangeOffence;
	pAvatarMaintAttribute.fStrangeDefence = pAttribute->fStrangeDefence;
	pAvatarMaintAttribute.fWildOffence = pAttribute->fWildOffence;
	pAvatarMaintAttribute.fWildDefence = pAttribute->fWildDefence;
	pAvatarMaintAttribute.fEleganceOffence = pAttribute->fEleganceOffence;
	pAvatarMaintAttribute.fEleganceDefence = pAttribute->fEleganceDefence;
	pAvatarMaintAttribute.fFunnyOffence = pAttribute->fFunnyOffence;
	pAvatarMaintAttribute.fFunnyDefence = pAttribute->fFunnyDefence;
}

void AttributeManager::CompressAttributes()
{
	//Bases
	void* byBaseCon = &pAvatarMaintAttribute.wBaseCon;
	void* byBaseStr = &pAvatarMaintAttribute.wBaseStr;
	void* byBaseSol = &pAvatarMaintAttribute.wBaseSol;
	void* byBaseFoc = &pAvatarMaintAttribute.wBaseFoc;
	void* byBaseEng = &pAvatarMaintAttribute.wBaseEng;
	void* byBaseDex = &pAvatarMaintAttribute.wBaseDex;

	void* wBaseAttackRate = &pAvatarMaintAttribute.wBaseAttackRate;
	void* wBaseAttackSpeedRate = &pAvatarMaintAttribute.wBaseAttackSpeedRate;
	void* wBaseBlockRate = &pAvatarMaintAttribute.wBaseBlockRate;
	void* wBaseCurseSuccessRate = &pAvatarMaintAttribute.wBaseCurseSuccessRate;
	void* wBaseCurseToleranceRate = &pAvatarMaintAttribute.wBaseCurseToleranceRate;
	void* wBaseDodgeRate = &pAvatarMaintAttribute.wBaseDodgeRate;
	void* wBaseEnergyCriticalRate = &pAvatarMaintAttribute.wBaseEnergyCriticalRate;
	void* wBaseEnergyDefence = &pAvatarMaintAttribute.wBaseEnergyDefence;
	void* wBaseEnergyOffence = &pAvatarMaintAttribute.wBaseEnergyOffence;
	void* wBasePhysicalCriticalRate = &pAvatarMaintAttribute.wBasePhysicalCriticalRate;
	void* wBasePhysicalOffence = &pAvatarMaintAttribute.wBasePhysicalOffence;
	void* wBasePhysicalDefence = &pAvatarMaintAttribute.wBasePhysicalDefence;
	void* wBaseEpBattleRegen = &pAvatarMaintAttribute.wBaseEpBattleRegen;
	void* wBaseEpRegen = &pAvatarMaintAttribute.wBaseEpRegen;
	void* wBaseEpSitdownRegen = &pAvatarMaintAttribute.wBaseEpSitdownRegen;
	void* wBaseLpBattleRegen = &pAvatarMaintAttribute.wBaseLpBattleRegen;
	void* wBaseLpRegen = &pAvatarMaintAttribute.wBaseLpRegen;
	void* wBaseLpSitdownRegen = &pAvatarMaintAttribute.wBaseLpSitdownRegen;
	void* wBaseRpRegen = &pAvatarMaintAttribute.wBaseRpRegen;
	void* wBaseMaxEP = &pAvatarMaintAttribute.wBaseMaxEP;
	void* wBaseMaxLP = &pAvatarMaintAttribute.dwBaseMaxLP;
	void* wBaseMaxRP = &pAvatarMaintAttribute.wBaseMaxRP;
	//Lasts
	void* byLastCon = &pAvatarMaintAttribute.wLastCon;
	void* byLastStr = &pAvatarMaintAttribute.wLastStr;
	void* byLastSol = &pAvatarMaintAttribute.wLastSol;
	void* byLastFoc = &pAvatarMaintAttribute.wLastFoc;
	void* byLastEng = &pAvatarMaintAttribute.wLastEng;
	void* byLastDex = &pAvatarMaintAttribute.wLastDex;

	void* wLastAttackRate = &pAvatarMaintAttribute.wLastAttackRate;
	void* wLastAttackSpeedRate = &pAvatarMaintAttribute.wLastAttackSpeedRate;
	void* wLastBlockRate = &pAvatarMaintAttribute.wLastBlockRate;
	void* wLastCurseSuccessRate = &pAvatarMaintAttribute.wLastCurseSuccessRate;
	void* wLastCurseToleranceRate = &pAvatarMaintAttribute.wLastCurseToleranceRate;
	void* wLastDodgeRate = &pAvatarMaintAttribute.wLastDodgeRate;
	void* wLastEnergyCriticalRate = &pAvatarMaintAttribute.wLastEnergyCriticalRate;
	void* wLastEnergyOffence = &pAvatarMaintAttribute.wLastEnergyOffence;
	void* wLastEnergyDefence = &pAvatarMaintAttribute.wLastEnergyDefence;
	void* wLastPhysicalCriticalRate = &pAvatarMaintAttribute.wLastPhysicalCriticalRate;
	void* wLastPhysicalOffence = &pAvatarMaintAttribute.wLastPhysicalOffence;
	void* wLastPhysicalDefence = &pAvatarMaintAttribute.wLastPhysicalDefence;
	void* wLastEpBattleRegen = &pAvatarMaintAttribute.wLastEpBattleRegen;
	void* wLastEpRegen = &pAvatarMaintAttribute.wLastEpRegen;
	void* wLastEpSitdownRegen = &pAvatarMaintAttribute.wLastEpSitdownRegen;
	void* wLastLpBattleRegen = &pAvatarMaintAttribute.wLastLpBattleRegen;
	void* wLastLpRegen = &pAvatarMaintAttribute.wLastLpRegen;
	void* wLastLpSitdownRegen = &pAvatarMaintAttribute.wLastLpSitdownRegen;
	void* wLastRpRegen = &pAvatarMaintAttribute.wLastRpRegen;
	void* wLastRpDimimutionRate = &pAvatarMaintAttribute.wLastRpDimimutionRate;
	void* wLastMaxEP = &pAvatarMaintAttribute.wLastMaxEP;
	void* wLastMaxLP = &pAvatarMaintAttribute.dwLastMaxLP;
	void* wLastMaxRP = &pAvatarMaintAttribute.wLastMaxRP;
	void* wGuardRate = &pAvatarMaintAttribute.wGuardRate;
	void* wParalyzeToleranceRate = &pAvatarMaintAttribute.wParalyzeToleranceRate;
	void* wTerrorToleranceRate = &pAvatarMaintAttribute.wTerrorToleranceRate;
	void* wConfuseToleranceRate = &pAvatarMaintAttribute.wConfuseToleranceRate;
	void* wStoneToleranceRate = &pAvatarMaintAttribute.wStoneToleranceRate;
	void* wCandyToleranceRate = &pAvatarMaintAttribute.wCandyToleranceRate;
	//Others
	void* fBaseAttackRange = &pAvatarMaintAttribute.fBaseAttackRange;
	void* fBleedingKeepTimeDown = &pAvatarMaintAttribute.fBleedingKeepTimeDown;
	void* fCandyKeepTimeDown = &pAvatarMaintAttribute.fCandyKeepTimeDown;
	void* fCastingTimeChangePercent = &pAvatarMaintAttribute.fCastingTimeChangePercent;
	void* fConfuseKeepTimeDown = &pAvatarMaintAttribute.fConfuseKeepTimeDown;
	void* fCoolTimeChangePercent = &pAvatarMaintAttribute.fCoolTimeChangePercent;
	void* fCriticalBlockSuccessRate = &pAvatarMaintAttribute.fCriticalBlockSuccessRate;
	void* fCurseBlockModeSuccessRate = &pAvatarMaintAttribute.fCurseBlockModeSuccessRate;
	void* fDotTimeChangeAbsolute = &pAvatarMaintAttribute.fDotTimeChangeAbsolute;
	void* fDotValueChangePercent = &pAvatarMaintAttribute.fDotValueChangePercent;
	void* fEleganceDefence = &pAvatarMaintAttribute.fEleganceDefence;
	void* fEleganceOffence = &pAvatarMaintAttribute.fEleganceOffence;
	void* fEnergyCriticalDamageBonusRate = &pAvatarMaintAttribute.fEnergyCriticalDamageBonusRate;
	void* fFunnyDefence = &pAvatarMaintAttribute.fFunnyDefence;
	void* fFunnyOffence = &pAvatarMaintAttribute.fFunnyOffence;
	void* fHonestDefence = &pAvatarMaintAttribute.fHonestDefence;
	void* fHonestOffence = &pAvatarMaintAttribute.fHonestOffence;
	void* fHtbBlockModeSuccessRate = &pAvatarMaintAttribute.fHtbBlockModeSuccessRate;
	void* fItemUpgradeBonusRate = &pAvatarMaintAttribute.fItemUpgradeBonusRate;
	void* fItemUpgradeBreakBonusRate = &pAvatarMaintAttribute.fItemUpgradeBreakBonusRate;
	void* fKeepTimeChangePercent = &pAvatarMaintAttribute.fKeepTimeChangePercent;
	void* fKnockdownBlockModeSuccessRate = &pAvatarMaintAttribute.fKnockdownBlockModeSuccessRate;
	void* fLastAttackRange = &pAvatarMaintAttribute.fLastAttackRange;
	void* fLastRunSpeed = &pAvatarMaintAttribute.fLastRunSpeed;
	void* fParalyzeKeepTimeDown = &pAvatarMaintAttribute.fParalyzeKeepTimeDown;
	void* fPhysicalCriticalDamageBonusRate = &pAvatarMaintAttribute.fPhysicalCriticalDamageBonusRate;
	void* fPoisonKeepTimeDown = &pAvatarMaintAttribute.fPoisonKeepTimeDown;
	void* fRequiredEpChangePercent = &pAvatarMaintAttribute.fRequiredEpChangePercent;
	void* fSitDownEpRegenBonusRate = &pAvatarMaintAttribute.fSitDownEpRegenBonusRate;
	void* fSitDownLpRegenBonusRate = &pAvatarMaintAttribute.fSitDownLpRegenBonusRate;
	void* fSkillDamageBlockModeSuccessRate = &pAvatarMaintAttribute.fSkillDamageBlockModeSuccessRate;
	void* fStomachacheKeepTimeDown = &pAvatarMaintAttribute.fStomachacheKeepTimeDown;
	void* fStoneKeepTimeDown = &pAvatarMaintAttribute.fStoneKeepTimeDown;
	void* fStrangeDefence = &pAvatarMaintAttribute.fStrangeDefence;
	void* fStrangeOffence = &pAvatarMaintAttribute.fStrangeOffence;
	void* fTerrorKeepTimeDown = &pAvatarMaintAttribute.fTerrorKeepTimeDown;
	void* fWildDefence = &pAvatarMaintAttribute.fWildDefence;
	void* fWildOffence = &pAvatarMaintAttribute.fWildOffence;

	//New Attribs
	pAvatarAttributeLink.pbyBaseCon = (BYTE*)byBaseCon;
	pAvatarAttributeLink.pbyBaseStr = (BYTE*)byBaseStr;
	pAvatarAttributeLink.pbyBaseSol = (BYTE*)byBaseSol;
	pAvatarAttributeLink.pbyBaseFoc = (BYTE*)byBaseFoc;
	pAvatarAttributeLink.pbyBaseEng = (BYTE*)byBaseEng;
	pAvatarAttributeLink.pbyBaseDex = (BYTE*)byBaseDex;

	pAvatarAttributeLink.pbyLastCon = (BYTE*)byLastCon;
	pAvatarAttributeLink.pbyLastStr = (BYTE*)byLastStr;
	pAvatarAttributeLink.pbyLastSol = (BYTE*)byLastSol;
	pAvatarAttributeLink.pbyLastFoc = (BYTE*)byLastFoc;
	pAvatarAttributeLink.pbyLastEng = (BYTE*)byLastEng;
	pAvatarAttributeLink.pbyLastDex = (BYTE*)byLastDex;

	pAvatarAttributeLink.pwBaseAttackRate = (WORD*)wBaseAttackRate;
	pAvatarAttributeLink.pwBaseAttackSpeedRate = (WORD*)wBaseAttackSpeedRate;
	pAvatarAttributeLink.pwBaseBlockRate = (WORD*)wBaseBlockRate;
	pAvatarAttributeLink.pwBaseCurseSuccessRate = (WORD*)wBaseCurseSuccessRate;
	pAvatarAttributeLink.pwBaseCurseToleranceRate = (WORD*)wBaseCurseToleranceRate;
	pAvatarAttributeLink.pwBaseDodgeRate = (WORD*)wBaseDodgeRate;
	pAvatarAttributeLink.pwBaseEnergyCriticalRate = (WORD*)wBaseEnergyCriticalRate;
	pAvatarAttributeLink.pwBaseEnergyOffence = (WORD*)wBaseEnergyOffence;
	pAvatarAttributeLink.pwBaseEnergyDefence = (WORD*)wBaseEnergyDefence;
	pAvatarAttributeLink.pwBasePhysicalCriticalRate = (WORD*)wBasePhysicalCriticalRate;
	pAvatarAttributeLink.pwBasePhysicalOffence = (WORD*)wBasePhysicalOffence;
	pAvatarAttributeLink.pwBasePhysicalDefence = (WORD*)wBasePhysicalDefence;
	pAvatarAttributeLink.pwBaseEpBattleRegen = (WORD*)wBaseEpBattleRegen;
	pAvatarAttributeLink.pwBaseEpRegen = (WORD*)wBaseEpRegen;
	pAvatarAttributeLink.pwBaseEpSitdownRegen = (WORD*)wBaseEpSitdownRegen;
	pAvatarAttributeLink.pwBaseLpBattleRegen = (WORD*)wBaseLpBattleRegen;
	pAvatarAttributeLink.pwBaseLpRegen = (WORD*)wBaseLpRegen;
	pAvatarAttributeLink.pwBaseLpSitdownRegen = (WORD*)wBaseLpSitdownRegen;
	pAvatarAttributeLink.pwBaseRpRegen = (WORD*)wBaseRpRegen;
	pAvatarAttributeLink.pwBaseMaxEP = (WORD*)wBaseMaxEP;
	pAvatarAttributeLink.pwBaseMaxLP = (WORD*)wBaseMaxLP;
	pAvatarAttributeLink.pwBaseMaxRP = (WORD*)wBaseMaxRP;
	pAvatarAttributeLink.pwBaseEpSitdownRegen = (WORD*)wBaseEpSitdownRegen;

	pAvatarAttributeLink.pwLastAttackRate = (WORD*)wLastAttackRate;
	pAvatarAttributeLink.pwLastAttackSpeedRate = (WORD*)wLastAttackSpeedRate;
	pAvatarAttributeLink.pwLastBlockRate = (WORD*)wLastBlockRate;
	pAvatarAttributeLink.pwLastCurseSuccessRate = (WORD*)wLastCurseSuccessRate;
	pAvatarAttributeLink.pwLastCurseToleranceRate = (WORD*)wLastCurseToleranceRate;
	pAvatarAttributeLink.pwLastDodgeRate = (WORD*)wLastDodgeRate;
	pAvatarAttributeLink.pwLastEnergyCriticalRate = (WORD*)wLastEnergyCriticalRate;
	pAvatarAttributeLink.pwLastEnergyOffence = (WORD*)wLastEnergyOffence;
	pAvatarAttributeLink.pwLastEnergyDefence = (WORD*)wLastEnergyDefence;
	pAvatarAttributeLink.pwLastPhysicalCriticalRate = (WORD*)wLastPhysicalCriticalRate;
	pAvatarAttributeLink.pwLastPhysicalOffence = (WORD*)wLastPhysicalOffence;
	pAvatarAttributeLink.pwLastPhysicalDefence = (WORD*)wLastPhysicalDefence;
	pAvatarAttributeLink.pwLastEpBattleRegen = (WORD*)wLastEpBattleRegen;
	pAvatarAttributeLink.pwLastEpRegen = (WORD*)wLastEpRegen;
	pAvatarAttributeLink.pwLastEpSitdownRegen = (WORD*)wLastEpSitdownRegen;
	pAvatarAttributeLink.pwLastLpBattleRegen = (WORD*)wLastLpBattleRegen;
	pAvatarAttributeLink.pwLastLpRegen = (WORD*)wLastLpRegen;
	pAvatarAttributeLink.pwLastLpSitdownRegen = (WORD*)wLastLpSitdownRegen;
	pAvatarAttributeLink.pwLastRpRegen = (WORD*)wLastRpRegen;
	pAvatarAttributeLink.pwLastRpDimimutionRate = (WORD*)wLastRpDimimutionRate;
	pAvatarAttributeLink.pwLastMaxEP = (WORD*)wLastMaxEP;
	pAvatarAttributeLink.pwLastMaxLP = (WORD*)wLastMaxLP;
	pAvatarAttributeLink.pwLastMaxRP = (WORD*)wLastMaxRP;

	pAvatarAttributeLink.pwGuardRate = (WORD*)wGuardRate;
	pAvatarAttributeLink.pwParalyzeToleranceRate = (WORD*)wParalyzeToleranceRate;
	pAvatarAttributeLink.pwTerrorToleranceRate = (WORD*)wTerrorToleranceRate;
	pAvatarAttributeLink.pwConfuseToleranceRate = (WORD*)wConfuseToleranceRate;
	pAvatarAttributeLink.pwCandyToleranceRate = (WORD*)wCandyToleranceRate;
	pAvatarAttributeLink.pwStoneToleranceRate = (WORD*)wStoneToleranceRate;

	pAvatarAttributeLink.pfBaseAttackRange = (float*)fBaseAttackRange;
	pAvatarAttributeLink.pfBleedingKeepTimeDown = (float*)fBleedingKeepTimeDown;
	pAvatarAttributeLink.pfCandyKeepTimeDown = (float*)fCandyKeepTimeDown;
	pAvatarAttributeLink.pfCastingTimeChangePercent = (float*)fCastingTimeChangePercent;
	pAvatarAttributeLink.pfConfuseKeepTimeDown = (float*)fConfuseKeepTimeDown;
	pAvatarAttributeLink.pfCoolTimeChangePercent = (float*)fCoolTimeChangePercent;
	pAvatarAttributeLink.pfCriticalBlockSuccessRate = (float*)fCriticalBlockSuccessRate;
	pAvatarAttributeLink.pfCurseBlockModeSuccessRate = (float*)fCurseBlockModeSuccessRate;
	pAvatarAttributeLink.pfDotTimeChangeAbsolute = (float*)fDotTimeChangeAbsolute;
	pAvatarAttributeLink.pfDotValueChangePercent = (float*)fDotValueChangePercent;
	pAvatarAttributeLink.pfEleganceOffence = (float*)fEleganceOffence;
	pAvatarAttributeLink.pfEleganceDefence = (float*)fEleganceDefence;
	pAvatarAttributeLink.pfEnergyCriticalDamageBonusRate = (float*)fEnergyCriticalDamageBonusRate;
	pAvatarAttributeLink.pfFunnyOffence = (float*)fFunnyOffence;
	pAvatarAttributeLink.pfFunnyDefence = (float*)fFunnyDefence;
	pAvatarAttributeLink.pfHonestOffence = (float*)fHonestOffence;
	pAvatarAttributeLink.pfHonestDefence = (float*)fHonestDefence;
	pAvatarAttributeLink.pfHtbBlockModeSuccessRate = (float*)fHtbBlockModeSuccessRate;
	pAvatarAttributeLink.pfItemUpgradeBonusRate = (float*)fItemUpgradeBonusRate;
	pAvatarAttributeLink.pfItemUpgradeBreakBonusRate = (float*)fItemUpgradeBreakBonusRate;
	pAvatarAttributeLink.pfKeepTimeChangePercent = (float*)fKeepTimeChangePercent;
	pAvatarAttributeLink.pfKnockdownBlockModeSuccessRate = (float*)fKnockdownBlockModeSuccessRate;
	pAvatarAttributeLink.pfLastAttackRange = (float*)fLastAttackRange;
	pAvatarAttributeLink.pfLastRunSpeed = (float*)fLastRunSpeed;
	pAvatarAttributeLink.pfParalyzeKeepTimeDown = (float*)fParalyzeKeepTimeDown;
	pAvatarAttributeLink.pfPhysicalCriticalDamageBonusRate = (float*)fPhysicalCriticalDamageBonusRate;
	pAvatarAttributeLink.pfPoisonKeepTimeDown = (float*)fPoisonKeepTimeDown;
	pAvatarAttributeLink.pfRequiredEpChangePercent = (float*)fRequiredEpChangePercent;
	pAvatarAttributeLink.pfSitDownEpRegenBonusRate = (float*)fSitDownEpRegenBonusRate;
	pAvatarAttributeLink.pfSitDownLpRegenBonusRate = (float*)fSitDownLpRegenBonusRate;
	pAvatarAttributeLink.pfSkillDamageBlockModeSuccessRate = (float*)fSkillDamageBlockModeSuccessRate;
	pAvatarAttributeLink.pfStomachacheKeepTimeDown = (float*)fStomachacheKeepTimeDown;
	pAvatarAttributeLink.pfStoneKeepTimeDown = (float*)fStoneKeepTimeDown;
	pAvatarAttributeLink.pfStrangeDefence = (float*)fStrangeDefence;
	pAvatarAttributeLink.pfStrangeOffence = (float*)fStrangeOffence;
	pAvatarAttributeLink.pfTerrorKeepTimeDown = (float*)fTerrorKeepTimeDown;
	pAvatarAttributeLink.pfWildDefence = (float*)fWildDefence;
	pAvatarAttributeLink.pfWildOffence = (float*)fWildOffence;
}
//By Luiz45 Calculate Attributes
void AttributeManager::CalculateAttributes()
{

}
//By Luiz45 Send Avatar
sGU_AVATAR_ATTRIBUTE_UPDATE AttributeManager::PrepareUpdatePacket(HOBJECT playerSerialID)
{
	CompressAttributes();
	sGU_AVATAR_ATTRIBUTE_UPDATE pUpdateAttribute;
	memset(&pUpdateAttribute, 0, sizeof(pUpdateAttribute));
	

	CNtlBitFlagManager flagMgr;

	if (false == flagMgr.Create(&pAvatarMaintAttribute, ATTRIBUTE_TO_UPDATE_COUNT))
	{
		Logger::Log("FlagManager for Attribute failed", this); 
	}

	for (BYTE byIndex = ATTRIBUTE_TO_UPDATE_FIRST; byIndex <= ATTRIBUTE_TO_UPDATE_LAST; byIndex++)
	{
		flagMgr.Set(byIndex);
	}

	DWORD buffer[2048];
	DWORD datasize;

	if (CNtlAvatar::GetInstance()->SaveAvatarAttribute(&flagMgr, &pAvatarAttributeLink, &buffer, &datasize) == false){
		Logger::Log("Save Avatar was failed", this);
		return pUpdateAttribute;
	}
	
	memcpy(pUpdateAttribute.abyFlexibleField, &buffer, sizeof(sAVATAR_ATTRIBUTE));
	pUpdateAttribute.byAttributeTotalCount = ATTRIBUTE_TO_UPDATE_COUNT;

	pUpdateAttribute.wOpCode = GU_AVATAR_ATTRIBUTE_UPDATE;
	pUpdateAttribute.hHandle = playerSerialID;

	return pUpdateAttribute;
}
//By luiz45 - Update Attribute with Equiped Items
void AttributeManager::UpdateWithEquipment(sITEM_TBLDAT* itemTBL, bool bRemove, BYTE byGrade)
{
	if (itemTBL->wPhysical_Offence < 65535 && itemTBL->wPhysical_Offence > 0)
	{
		if (bRemove)
			pAvatarMaintAttribute.wLastPhysicalOffence -= Dbo_GetFinalOffence(itemTBL->wPhysical_Offence, byGrade);
		else
			pAvatarMaintAttribute.wLastPhysicalOffence += Dbo_GetFinalOffence(itemTBL->wPhysical_Offence, byGrade);
	}
	if (itemTBL->wPhysical_Defence < 65535 && itemTBL->wPhysical_Defence > 0)
	{
		if (bRemove)
			pAvatarMaintAttribute.wLastPhysicalDefence -= Dbo_GetFinalDefence(itemTBL->wPhysical_Defence, byGrade);
		else
			pAvatarMaintAttribute.wLastPhysicalDefence += Dbo_GetFinalDefence(itemTBL->wPhysical_Defence, byGrade);
	}
	if (itemTBL->wEnergy_Offence < 65535 && itemTBL->wEnergy_Offence > 0)
	{
		if (bRemove)
			pAvatarMaintAttribute.wLastEnergyOffence -= Dbo_GetFinalOffence(itemTBL->wEnergy_Offence, byGrade);
		else
			pAvatarMaintAttribute.wLastEnergyOffence += Dbo_GetFinalOffence(itemTBL->wEnergy_Offence, byGrade);
	}
	if (itemTBL->wEnergy_Defence < 65535 && itemTBL->wEnergy_Defence > 0)
	{
		if (bRemove)
			pAvatarMaintAttribute.wLastEnergyDefence -= Dbo_GetFinalDefence(itemTBL->wEnergy_Defence, byGrade);
		else
			pAvatarMaintAttribute.wLastEnergyDefence += Dbo_GetFinalDefence(itemTBL->wEnergy_Defence, byGrade);
	}
	if (itemTBL->fAttack_Range_Bonus < 65535 && itemTBL->fAttack_Range_Bonus > 0)
	{
		pAvatarMaintAttribute.fLastAttackRange += itemTBL->fAttack_Range_Bonus;
	}
}
//By Luiz45 - Just return the Avatar attribute uncrompressed()
sAVATAR_ATTRIBUTE AttributeManager::GetAvatarAttribute()
{
	return this->pAvatarMaintAttribute;
}