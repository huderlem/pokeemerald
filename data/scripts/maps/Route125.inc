Route125_MapScripts:: @ 81F6748
	map_script 3, Route125_MapScript1_1F6758
	map_script 1, Route125_MapScript1_1F6783
	map_script 2, Route125_MapScript2_1F679A
	.byte 0

Route125_MapScript1_1F6758: @ 81F6758
	checkflag FLAG_SYS_WEATHER_CTRL
	call_if 1, Route125_EventScript_27207A
	compare_var_to_value VAR_0x4039, 1
	call_if 1, Route125_EventScript_273D13
	compare_var_to_value VAR_0x4037, 11
	call_if 1, Route125_EventScript_273D17
	compare_var_to_value VAR_0x4037, 12
	call_if 1, Route125_EventScript_273D17
	end

Route125_MapScript1_1F6783: @ 81F6783
	compare_var_to_value VAR_0x4037, 11
	call_if 1, Route125_EventScript_273A85
	compare_var_to_value VAR_0x4037, 12
	call_if 1, Route125_EventScript_273AF2
	end

Route125_MapScript2_1F679A: @ 81F679A
	map_script_2 VAR_0x4039, 1, gUnknown_08273D1F
	.2byte 0

Route125_EventScript_1F67A4:: @ 81F67A4
	trainerbattle 0, TRAINER_NOLEN, 0, Route125_Text_2A1527, Route125_Text_2A155A
	msgbox Route125_Text_2A1567, 6
	end

Route125_EventScript_1F67BB:: @ 81F67BB
	trainerbattle 0, TRAINER_STAN, 0, Route125_Text_2A159F, Route125_Text_2A15C7
	msgbox Route125_Text_2A15D5, 6
	end

Route125_EventScript_1F67D2:: @ 81F67D2
	trainerbattle 0, TRAINER_TANYA, 0, Route125_Text_2A1615, Route125_Text_2A164D
	msgbox Route125_Text_2A165E, 6
	end

Route125_EventScript_1F67E9:: @ 81F67E9
	trainerbattle 0, TRAINER_SHARON, 0, Route125_Text_2A1686, Route125_Text_2A16C5
	msgbox Route125_Text_2A16CE, 6
	end

Route125_EventScript_1F6800:: @ 81F6800
	trainerbattle 2, TRAINER_ERNEST_1, 0, Route125_Text_2A16FC, Route125_Text_2A173A, Route125_EventScript_1F682C
	specialvar VAR_RESULT, ShouldTryRematchBattle
	compare_var_to_value VAR_RESULT, 1
	goto_eq Route125_EventScript_1F684B
	msgbox Route125_Text_2A1755, 4
	release
	end

Route125_EventScript_1F682C:: @ 81F682C
	special sub_80B4808
	waitmovement 0
	msgbox Route125_Text_2A17CF, 4
	setvar VAR_0x8004, 492
	special sub_81D1C20
	setorcopyvar VAR_0x8000, 492
	callstd 8
	release
	end

Route125_EventScript_1F684B:: @ 81F684B
	trainerbattle 5, TRAINER_ERNEST_1, 0, Route125_Text_2A180E, Route125_Text_2A1851
	msgbox Route125_Text_2A187A, 6
	end

Route125_EventScript_1F6862:: @ 81F6862
	trainerbattle 4, TRAINER_KIM_AND_IRIS, 0, Route125_Text_2A192B, Route125_Text_2A1989, Route125_Text_2A19F8
	msgbox Route125_Text_2A19A6, 6
	end

Route125_EventScript_1F687D:: @ 81F687D
	trainerbattle 4, TRAINER_KIM_AND_IRIS, 0, Route125_Text_2A1A35, Route125_Text_2A1A71, Route125_Text_2A1AD9
	msgbox Route125_Text_2A1A92, 6
	end

Route125_EventScript_1F6898:: @ 81F6898
	trainerbattle 0, TRAINER_PRESLEY, 0, Route125_Text_2A1B1A, Route125_Text_2A1B4E
	msgbox Route125_Text_2A1B70, 6
	end

Route125_EventScript_1F68AF:: @ 81F68AF
	trainerbattle 0, TRAINER_AURON, 0, Route125_Text_2A1BCE, Route125_Text_2A1BFD
	msgbox Route125_Text_2A1C2A, 6
	end

