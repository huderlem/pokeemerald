BattleFrontier_BattlePikeRandomRoom1_MapScripts:: @ 825D152
	map_script 5, BattleFrontier_BattlePikeRandomRoom1_MapScript1_2C423E
	map_script 3, BattleFrontier_BattlePikeRandomRoom1_MapScript1_2C3E25
	map_script 2, BattleFrontier_BattlePikeRandomRoom1_MapScript2_25D167
	map_script 4, BattleFrontier_BattlePikeRandomRoom1_MapScript2_2C3EDE
	.byte 0

BattleFrontier_BattlePikeRandomRoom1_MapScript2_25D167: @ 825D167
	map_script_2 VAR_0x4000, 0, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D171
	.2byte 0

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D171:: @ 825D171
	setvar VAR_0x4000, 1
	setvar VAR_0x8004, 5
	special sub_81A703C
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D1C6
	case 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D50E
	case 3, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D605
	case 6, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D231
	case 7, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D53E
	case 8, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D2BF
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D1C6:: @ 825D1C6
	lockall
	delay 16
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D795
	waitmovement 0
	setvar VAR_0x8004, 21
	setvar VAR_0x8005, 0
	special sub_81A703C
	msgbox 0x2021fc4, 4
	waitmessage
	closemessage
	releaseall
	setvar VAR_0x8004, 7
	setvar VAR_0x8005, 0
	special sub_8163AC4
	waitstate
	switch VAR_RESULT
	case 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D226

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D20A:: @ 825D20A
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 0
	setvar VAR_0x8006, 4
	special sub_81A1780
	warp BATTLE_FRONTIER_BATTLE_PIKE_LOBBY, 255, 5, 6
	waitstate
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D226:: @ 825D226
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D798
	waitmovement 0
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D231:: @ 825D231
	lockall
	delay 16
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DDFE, 4
	waitmessage
	closemessage
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D795
	waitmovement 0
	setvar VAR_0x8004, 21
	setvar VAR_0x8005, 0
	special sub_81A703C
	msgbox 0x2021fc4, 4
	waitmessage
	closemessage
	releaseall
	setvar VAR_0x8004, 7
	setvar VAR_0x8005, 0
	special sub_8163AC4
	waitstate
	switch VAR_RESULT
	case 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D285
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D20A
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D285:: @ 825D285
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D798
	waitmovement 0
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B6
	waitmovement 0
	lock
	faceplayer
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DE3F, 4
	playfanfare BGM_ME_ASA
	waitfanfare
	special sp000_heal_pokemon
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DE94, 4
	closemessage
	release
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D798
	waitmovement 0
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D2BF:: @ 825D2BF
	delay 22
	lockall
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7DF
	waitmovement 0
	setvar VAR_0x8004, 23
	special sub_81A703C
	switch VAR_RESULT
	case 0, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D4DC
	case 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D4EA
	case 2, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D4FC

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D2FB:: @ 825D2FB
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E311, 4
	closemessage
	special SpawnScriptFieldObject
	applymovement 127, BattleFrontier_BattlePikeRandomRoom1_Movement_25D9B0
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7E2
	waitmovement 0
	playse 267
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D8E7
	delay 4
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D8A4
	delay 4
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D96D
	delay 4
	waitse
	delay 60
	playse 268
	delay 65
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D8A4
	showobjectat 1, BATTLE_FRONTIER_BATTLE_PIKE_RANDOM_ROOM_1
	delay 4
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D8E7
	delay 4
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D92A
	waitse
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7E7
	waitmovement 0
	setvar VAR_0x8004, 22
	special sub_81A703C
	switch VAR_RESULT
	case 2, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D416
	case 3, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D3BD
	case 4, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D443
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 7
	special sub_81A1780
	compare_var_to_value VAR_RESULT, 0
	goto_if 5, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D3BD
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DED2, 4
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 7
	special sub_81A1780

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D3BD:: @ 825D3BD
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DF71, 4
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D77B
	compare_var_to_value VAR_RESULT, 1
	goto_eq BattleFrontier_BattlePikeRandomRoom1_EventScript_25D3DA
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D20A

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D3DA:: @ 825D3DA
	setvar VAR_0x8004, 12
	special sub_81A1780
	compare_var_to_value VAR_RESULT, 0
	goto_if 5, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D49D
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DFA2, 4
	waitmessage
	playfanfare BGM_ME_BATTLE_SYMBOL
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DFD0
	waitmessage
	waitfanfare
	setvar VAR_0x8004, 13
	special sub_81A1780
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E003, 4
	closemessage
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D49D

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D416:: @ 825D416
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 7
	special sub_81A1780
	compare_var_to_value VAR_RESULT, 0
	goto_if 5, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D443
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E02C, 4
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 7
	special sub_81A1780

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D443:: @ 825D443
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E0E8, 4
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_25D77B
	compare_var_to_value VAR_RESULT, 1
	goto_eq BattleFrontier_BattlePikeRandomRoom1_EventScript_25D460
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D20A

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D460:: @ 825D460
	setvar VAR_0x8004, 12
	special sub_81A1780
	compare_var_to_value VAR_RESULT, 2
	goto_eq BattleFrontier_BattlePikeRandomRoom1_EventScript_25D49D
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E0F6, 4
	waitmessage
	playfanfare BGM_ME_BATTLE_SYMBOL
	message BattleFrontier_BattlePikeRandomRoom1_Text_25E118
	waitmessage
	waitfanfare
	setvar VAR_0x8004, 13
	special sub_81A1780
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E140, 4
	closemessage
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D49D
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D49D:: @ 825D49D
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7EA
	waitmovement 0
	applymovement 255, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7DC
	waitmovement 0
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	special sub_81A703C
	addvar VAR_RESULT, 1
	setvar VAR_0x8004, 2
	setvar VAR_0x8005, 1
	copyvar VAR_0x8006, VAR_RESULT
	special sub_81A703C
	call BattleFrontier_BattlePikeRandomRoom1_EventScript_2C4144
	waitstate
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D4DC:: @ 825D4DC
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E15D, 4
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D2FB
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D4EA:: @ 825D4EA
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E1DD, 4
	playfanfare BGM_ME_ASA
	waitfanfare
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D2FB
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D4FC:: @ 825D4FC
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E238, 4
	playfanfare BGM_ME_ASA
	waitfanfare
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D2FB
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D50E:: @ 825D50E
	lockall
	delay 16
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D795
	waitmovement 0
	message BattleFrontier_BattlePikeRandomRoom1_Text_25D9B6
	waitmessage
	playfanfare BGM_ME_ASA
	waitfanfare
	special sp000_heal_pokemon
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DA13, 4
	closemessage
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D798
	waitmovement 0
	releaseall
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D53E:: @ 825D53E
	lockall
	delay 16
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C1
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C3
	waitmovement 0
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C5
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C5
	waitmovement 0
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C8
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7CA
	waitmovement 0
	applymovement 255, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7BB
	waitmovement 0
	setvar VAR_0x8004, 21
	setvar VAR_0x8005, 0
	special sub_81A703C
	msgbox 0x2021fc4, 4
	closemessage
	delay 16
	applymovement 255, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7BD
	waitmovement 0
	setvar VAR_0x8004, 21
	setvar VAR_0x8005, 1
	special sub_81A703C
	msgbox 0x2021fc4, 4
	closemessage
	setvar VAR_0x8004, 9
	setvar VAR_0x8005, 0
	special sub_8163AC4
	waitstate
	switch VAR_RESULT
	case 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D5DC
	goto BattleFrontier_BattlePikeRandomRoom1_EventScript_25D20A
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D5DC:: @ 825D5DC
	applymovement 255, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7BD
	waitmovement 0
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7CC
	waitmovement 0
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7D4
	waitmovement 0
	applymovement 255, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7BF
	waitmovement 0
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D605:: @ 825D605
	lockall
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DBD1
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7AA
	waitmovement 0
	setvar VAR_0x8004, 12
	special sub_81A703C
	copyvar VAR_0x8004, VAR_RESULT
	compare_var_to_value VAR_0x8004, 0
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D643
	compare_var_to_value VAR_0x8004, 1
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D6D5
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DD3F, 4
	closemessage
	releaseall
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D643:: @ 825D643
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DBE4, 4
	closemessage
	waitse
	playmoncry SPECIES_KIRLIA, 0
	waitmoncry
	setvar VAR_0x8004, 11
	special sub_81A703C
	compare_var_to_value VAR_RESULT, 2
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D751
	compare_var_to_value VAR_RESULT, 1
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D758
	compare_var_to_value VAR_RESULT, 3
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D75F
	compare_var_to_value VAR_RESULT, 4
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D766
	setvar VAR_0x8004, 15
	special sub_81A703C
	waitstate
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7A2
	waitmovement 0
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B1
	waitmovement 0
	waitse
	playmoncry SPECIES_KIRLIA, 0
	waitmoncry
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DCAF, 4
	waitse
	playmoncry SPECIES_KIRLIA, 0
	waitmoncry
	closemessage
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B3
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7A4
	waitmovement 0
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DCCF, 4
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D6D5:: @ 825D6D5
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DC01, 4
	closemessage
	waitse
	playmoncry SPECIES_DUSCLOPS, 0
	waitmoncry
	setvar VAR_0x8004, 11
	special sub_81A703C
	compare_var_to_value VAR_RESULT, 0
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D76D
	compare_var_to_value VAR_RESULT, 1
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D774
	setvar VAR_0x8004, 15
	special sub_81A703C
	waitstate
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7A2
	waitmovement 0
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B1
	waitmovement 0
	waitse
	playmoncry SPECIES_DUSCLOPS, 0
	waitmoncry
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DCAF, 4
	waitse
	playmoncry SPECIES_DUSCLOPS, 0
	waitmoncry
	closemessage
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B3
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7A4
	waitmovement 0
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DD06, 4
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D751:: @ 825D751
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DC20
	waitmessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D758:: @ 825D758
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DC63
	waitmessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D75F:: @ 825D75F
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DC49
	waitmessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D766:: @ 825D766
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DC33
	waitmessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D76D:: @ 825D76D
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DC97
	waitmessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D774:: @ 825D774
	message BattleFrontier_BattlePikeRandomRoom1_Text_25DC7C
	waitmessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D77B:: @ 825D77B
	closemessage
	applymovement 255, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7DC
	waitmovement 0
	setvar VAR_0x8004, 7
	setvar VAR_0x8005, 0
	special sub_8163AC4
	waitstate
	return

BattleFrontier_BattlePikeRandomRoom1_Movement_25D795: @ 825D795
	step_down
	step_down
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D798: @ 825D798
	step_up
	step_up
	step_up
	step_54
	step_up
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D79E: @ 825D79E
	step_up
	step_54
	step_up
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7A2: @ 825D7A2
	step_down
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7A4: @ 825D7A4
	step_down
	step_02
	step_14
	step_14
	step_00
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7AA: @ 825D7AA
	step_15
	step_18
	step_18
	step_15
	step_17
	step_00
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B1: @ 825D7B1
	step_01
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B3: @ 825D7B3
	step_left

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B4: @ 825D7B4
	step_03
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B6: @ 825D7B6
	step_down
	step_down
	step_right
	step_00
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7BB: @ 825D7BB
	step_02
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7BD: @ 825D7BD
	step_03
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7BF: @ 825D7BF
	step_01
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C1: @ 825D7C1
	step_right
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C3: @ 825D7C3
	step_left
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C5: @ 825D7C5
	step_down
	step_down
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7C8: @ 825D7C8
	step_03
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7CA: @ 825D7CA
	step_02
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7CC: @ 825D7CC
	step_up
	step_up
	step_up
	step_right
	step_up
	step_54
	step_up
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7D4: @ 825D7D4
	step_up
	step_up
	step_up
	step_left
	step_up
	step_54
	step_up
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7DC: @ 825D7DC
	step_up
	step_up
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7DF: @ 825D7DF
	step_down
	step_down
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7E2: @ 825D7E2
	step_up
	step_up
	step_up
	step_54
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7E7: @ 825D7E7
	step_55
	step_down
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D7EA: @ 825D7EA
	step_left
	step_03
	step_end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D7ED:: @ 825D7ED
	setvar VAR_0x8004, 5
	special sub_81A703C
	switch VAR_RESULT
	case 2, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D81B
	case 3, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D831
	case 4, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D83E

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D81B:: @ 825D81B
	lock
	faceplayer
	setvar VAR_0x8004, 14
	special sub_81A703C
	msgbox 0x2021fc4, 4
	waitmessage
	closemessage
	release
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D831:: @ 825D831
	lock
	faceplayer
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DDA3, 4
	closemessage
	release
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D83E:: @ 825D83E
	setvar VAR_0x8004, 13
	special sub_81A703C
	compare_var_to_value VAR_RESULT, 2
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D881
	compare_var_to_value VAR_RESULT, 1
	call_if 1, BattleFrontier_BattlePikeRandomRoom1_EventScript_25D875
	playfanfare BGM_ME_ASA
	waitfanfare
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DABE, 4
	closemessage
	release
	applymovement 1, BattleFrontier_BattlePikeRandomRoom1_Movement_25D79E
	waitmovement 0
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D875:: @ 825D875
	lock
	faceplayer
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DA51, 4
	closemessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D881:: @ 825D881
	lock
	faceplayer
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25E32E, 4
	closemessage
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D88D:: @ 825D88D
	lock
	faceplayer
	msgbox BattleFrontier_BattlePikeRandomRoom1_Text_25DDE6, 4
	closemessage
	release
	applymovement 2, BattleFrontier_BattlePikeRandomRoom1_Movement_25D7B4
	waitmovement 0
	end

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D8A4:: @ 825D8A4
	setmetatile 4, 1, 554, 1
	setmetatile 3, 2, 561, 1
	setmetatile 4, 2, 562, 1
	setmetatile 5, 2, 563, 1
	setmetatile 3, 3, 569, 1
	setmetatile 4, 3, 570, 0
	setmetatile 5, 3, 571, 1
	special DrawWholeMapView
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D8E7:: @ 825D8E7
	setmetatile 4, 1, 522, 1
	setmetatile 3, 2, 529, 1
	setmetatile 4, 2, 530, 1
	setmetatile 5, 2, 531, 1
	setmetatile 3, 3, 537, 1
	setmetatile 4, 3, 538, 0
	setmetatile 5, 3, 539, 1
	special DrawWholeMapView
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D92A:: @ 825D92A
	setmetatile 4, 1, 683, 1
	setmetatile 3, 2, 690, 1
	setmetatile 4, 2, 691, 1
	setmetatile 5, 2, 692, 1
	setmetatile 3, 3, 698, 1
	setmetatile 4, 3, 699, 0
	setmetatile 5, 3, 700, 1
	special DrawWholeMapView
	return

BattleFrontier_BattlePikeRandomRoom1_EventScript_25D96D:: @ 825D96D
	setmetatile 4, 1, 586, 1
	setmetatile 3, 2, 593, 1
	setmetatile 4, 2, 594, 1
	setmetatile 5, 2, 595, 1
	setmetatile 3, 3, 601, 1
	setmetatile 4, 3, 602, 0
	setmetatile 5, 3, 603, 1
	special DrawWholeMapView
	return

BattleFrontier_BattlePikeRandomRoom1_Movement_25D9B0: @ 825D9B0
	step_up
	step_up
	step_end

BattleFrontier_BattlePikeRandomRoom1_Movement_25D9B3: @ 825D9B3
	step_down
	step_down
	step_end

BattleFrontier_BattlePikeRandomRoom1_Text_25D9B6: @ 825D9B6
	.string "Welcome…\n"
	.string "You must be commended for your luck…\p"
	.string "Your POKéMON shall be restored\n"
	.string "to full health…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DA13: @ 825DA13
	.string "I urge you to enjoy the rest of your\n"
	.string "Battle Choice challenge…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DA51: @ 825DA51
	.string "Ah, you’re a lucky one.\n"
	.string "I’m in somewhat-good spirits now.\p"
	.string "I will restore one of your POKéMON\n"
	.string "to full health.$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DABE: @ 825DABE
	.string "The best of luck to you.\n"
	.string "Farewell.$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DAE1: @ 825DAE1
	.string "Excuse me…\p"
	.string "Would you care for a battle?\n"
	.string "I’ll restore your POKéMON if you win.$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DB2F: @ 825DB2F
	.string "Very well…\n"
	.string "Now, shall we?$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DB49: @ 825DB49
	.string "I see…\n"
	.string "How unsporting of you…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DB67: @ 825DB67
	.string "Magnificent…\p"
	.string "As promised, I shall restore your\n"
	.string "POKéMON to full health.$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DBAE: @ 825DBAE
	.string "The best of luck to you.\n"
	.string "Farewell.$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DBD1: @ 825DBD1
	.string "Oh, my!\p"
	.string "Watch out!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DBE4: @ 825DBE4
	.string "Now, now!\n"
	.string "KIRLIA, stop that!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DC01: @ 825DC01
	.string "Now, now!\n"
	.string "DUSCLOPS, stop that!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DC20: @ 825DC20
	.string "KIRLIA used TOXIC!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DC33: @ 825DC33
	.string "KIRLIA used HYPNOSIS!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DC49: @ 825DC49
	.string "KIRLIA used THUNDER WAVE!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DC63: @ 825DC63
	.string "KIRLIA used WILL-O-WISP!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DC7C: @ 825DC7C
	.string "DUSCLOPS used WILL-O-WISP!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DC97: @ 825DC97
	.string "DUSCLOPS used ICE BEAM!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DCAF: @ 825DCAF
	.string "Look here!\n"
	.string "That’s quite enough!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DCCF: @ 825DCCF
	.string "I must apologize to you…\p"
	.string "My KIRLIA has a TIMID nature…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DD06: @ 825DD06
	.string "I must apologize to you…\p"
	.string "My DUSCLOPS has a TIMID nature…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DD3F: @ 825DD3F
	.string "It attacks without warning if it is\n"
	.string "startled by another person…\p"
	.string "Are you and your POKéMON all right?$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DDA3: @ 825DDA3
	.string "I do apologize for what happened…\n"
	.string "I do hope your POKéMON are fine.$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DDE6: @ 825DDE6
	.string "… … … … … …\n"
	.string "… … … … … …$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DDFE: @ 825DDFE
	.string "Welcome…\p"
	.string "Here, we will have you battle\n"
	.string "a somewhat-tough TRAINER…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DE3F: @ 825DE3F
	.string "How wonderful…\p"
	.string "To honor your victory, your POKéMON\n"
	.string "shall be restored to full health…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DE94: @ 825DE94
	.string "I urge you to enjoy the rest of your\n"
	.string "Battle Choice challenge…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DED2: @ 825DED2
	.string "I am LUCY…\n"
	.string "I am the law here…\l"
	.string "For I am the PIKE QUEEN…\p"
	.string "You already know it, but to advance,\n"
	.string "you must defeat me…\p"
	.string "…I’m not one for idle chatter.\n"
	.string "Hurry. Come on…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DF71: @ 825DF71
	.string "Your luck…\n"
	.string "I hope you didn’t use it all up here…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DFA2: @ 825DFA2
	.string "LUCY: … … … … … …\n"
	.string "Show me your FRONTIER PASS…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25DFD0: @ 825DFD0
	.string "The Luck Symbol was embossed on\n"
	.string "the FRONTIER PASS!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E003: @ 825E003
	.string "…That’s all there is…\n"
	.string "Disappear already…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E02C: @ 825E02C
	.string "LUCY: …You again…\p"
	.string "… … … … … …\p"
	.string "…I’ve trampled flowers and braved\n"
	.string "storms to get to where I am…\p"
	.string "I don’t feel any compulsion to keep\n"
	.string "losing to the same opponent…\p"
	.string "… … … … … …\n"
	.string "Fine… I’ll do it…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E0E8: @ 825E0E8
	.string "Now!\n"
	.string "Come on!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E0F6: @ 825E0F6
	.string "LUCY: … … … … … …\p"
	.string "…FRONTIER PASS…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E118: @ 825E118
	.string "The Luck Symbol took on\n"
	.string "a golden shine!$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E140: @ 825E140
	.string "…You, I won’t forget…\n"
	.string "…Ever…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E15D: @ 825E15D
	.string "I welcome you…\p"
	.string "Giggle…\n"
	.string "You seem to be bereft of luck…\p"
	.string "If only you hadn’t chosen this room,\n"
	.string "your POKéMON could have been healed…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E1DD: @ 825E1DD
	.string "I welcome you…\p"
	.string "Since you have chosen this room, I will\n"
	.string "restore one POKéMON to full health…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E238: @ 825E238
	.string "I welcome you…\p"
	.string "Since you have chosen this room, I will\n"
	.string "restore two POKéMON to full health…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E293: @ 825E293
	.string "I welcome you…\p"
	.string "Giggle…\n"
	.string "You should thank your lucky stars…\p"
	.string "Since you have chosen this room,\n"
	.string "all your POKéMON will be restored…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E311: @ 825E311
	.string "I shall go fetch our master…$"

BattleFrontier_BattlePikeRandomRoom1_Text_25E32E: @ 825E32E
	.string "Ah, you’re a lucky one!\n"
	.string "I’m in good spirits now.\p"
	.string "I will restore two of your POKéMON\n"
	.string "to full health!$"

