// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayTag.h"

namespace EQGameplayTag
{
	UE_DEFINE_GAMEPLAY_TAG(UI_Notify_ButtonPressed, "UI.Notify.ButtonPressed");

	UE_DEFINE_GAMEPLAY_TAG(Character_State_Jumping, "Character.State.Jumping");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_ToggleCombat, "Character.State.ToggleCombat");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_ToggleCooldown, "Character.State.ToggleCooldown");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Combat, "Character.State.Combat");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Attacking, "Character.State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Skilling, "Character.State.Skilling");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_SkillCooldown, "Character.State.SkillCooldown");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Rolling, "Character.State.Rolling");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Sprinting, "Character.State.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Hit, "Character.State.Hit");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Invincible, "Character.State.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Blocking, "Character.State.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Parrying, "Character.State.Parrying");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Parried, "Character.State.Parried");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Stunned, "Character.State.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_DrinkingPotion, "Character.State.DrinkingPotion");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Dead, "Character.State.Dead");

	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Equip, "Character.Action.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_Unequip, "Character.Action.Unequip");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_AttackHitCheck, "Character.Action.AttackHitCheck");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_SkillHitCheck, "Character.Action.SkillHitCheck");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_HitReaction, "Character.Action.HitReaction");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_BlockingHit, "Character.Action.BlockingHit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_ParriedHit, "Character.Action.ParriedHit");
	UE_DEFINE_GAMEPLAY_TAG(Character_Action_KnockBackHit, "Character.Action.KnockBackHit");

	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Light, "Character.Attack.Light");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Running, "Character.Attack.Running");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Special, "Character.Attack.Special");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Heavy, "Character.Attack.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Character_Attack_Air, "Character.Attack.Air");

	UE_DEFINE_GAMEPLAY_TAG(Character_Data_Damage, "Character.Data.Damage");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Character_AttackHit, "GameplayCue.Character.AttackHit");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Character_Damage, "GameplayCue.Character.Damage");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Character_NotEnoughStamina, "GameplayCue.Character.NotEnoughStamina");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_WarningPrompt_Show, "GameplayCue.WarningPrompt.Show");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_WarningPrompt_Hide, "GameplayCue.WarningPrompt.Hide");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_InteractPrompt_Show, "GameplayCue.InteractPrompt.Show");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_InteractPrompt_Hide, "GameplayCue.InteractPrompt.Hide");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_ItemBox_Open, "GameplayCue.ItemBox.Open");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_PickupItem_Spawn, "GameplayCue.PickupItem.Spawn");
}
