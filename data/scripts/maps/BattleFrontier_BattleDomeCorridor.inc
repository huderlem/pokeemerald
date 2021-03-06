BattleFrontier_BattleDomeCorridor_MapScripts:: @ 824B0FE
	map_script 2, BattleFrontier_BattleDomeCorridor_MapScript2_24B104
	.byte 0

BattleFrontier_BattleDomeCorridor_MapScript2_24B104: @ 824B104
	map_script_2 VAR_0x4000, 0, BattleFrontier_BattleDomeCorridor_EventScript_24B10E
	.2byte 0

BattleFrontier_BattleDomeCorridor_EventScript_24B10E:: @ 824B10E
	delay 16
	setvar VAR_0x4000, 1
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	special sub_81A1780
	compare_var_to_value VAR_RESULT, 1
	goto_eq BattleFrontier_BattleDomeCorridor_EventScript_24B161
	applymovement 1, BattleFrontier_BattleDomeCorridor_Movement_24B1B2
	applymovement 255, BattleFrontier_BattleDomeCorridor_Movement_24B1A1
	waitmovement 0
	opendoor 13, 3
	waitdooranim
	applymovement 1, BattleFrontier_BattleDomeCorridor_Movement_24B1BE
	applymovement 255, BattleFrontier_BattleDomeCorridor_Movement_24B1AE
	waitmovement 0
	closedoor 13, 3
	waitdooranim
	goto BattleFrontier_BattleDomeCorridor_EventScript_24B18F

BattleFrontier_BattleDomeCorridor_EventScript_24B161:: @ 824B161
	applymovement 1, BattleFrontier_BattleDomeCorridor_Movement_24B1D6
	applymovement 255, BattleFrontier_BattleDomeCorridor_Movement_24B1C1
	waitmovement 0
	opendoor 37, 3
	waitdooranim
	applymovement 1, BattleFrontier_BattleDomeCorridor_Movement_24B1E6
	applymovement 255, BattleFrontier_BattleDomeCorridor_Movement_24B1D2
	waitmovement 0
	closedoor 37, 3
	waitdooranim

BattleFrontier_BattleDomeCorridor_EventScript_24B18F:: @ 824B18F
	waitmovement 0
	setvar VAR_0x8006, 0
	warp BATTLE_FRONTIER_BATTLE_DOME_PRE_BATTLE_ROOM, 255, 5, 7
	waitstate
	end

BattleFrontier_BattleDomeCorridor_Movement_24B1A1: @ 824B1A1
	step_up
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_26
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1AE: @ 824B1AE
	step_up
	step_up
	step_54
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1B2: @ 824B1B2
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_left
	step_up
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1BE: @ 824B1BE
	step_up
	step_54
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1C1: @ 824B1C1
	step_up
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_26
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1D2: @ 824B1D2
	step_up
	step_up
	step_54
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1D6: @ 824B1D6
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_up
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1E6: @ 824B1E6
	step_up
	step_54
	step_end

BattleFrontier_BattleDomeCorridor_Movement_24B1E9: @ 824B1E9
	step_up
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_right
	step_up
	step_up
	step_54
	step_end
