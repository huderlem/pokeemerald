LittlerootTown_BrendansHouse_1F_MapObjects: @ 852D08C
	object_event 1, MAP_OBJ_GFX_MOM, 0, 0, 2, 6, 3, 10, 0, 0, 0, 0, LittlerootTown_BrendansHouse_1F_EventScript_2929C5, 758, 0, 0
	object_event 2, MAP_OBJ_GFX_VIGAROTH_CARRYING_BOX, 0, 0, 1, 3, 3, 28, 3, 0, 0, 0, LittlerootTown_BrendansHouse_1F_EventScript_292ACD, 755, 0, 0
	object_event 3, MAP_OBJ_GFX_VIGAROTH_FACING_AWAY, 0, 0, 4, 5, 3, 65, 0, 0, 0, 0, LittlerootTown_BrendansHouse_1F_EventScript_292ABA, 754, 0, 0
	object_event 4, MAP_OBJ_GFX_WOMAN_5, 0, 0, 2, 7, 3, 10, 0, 0, 0, 0, LittlerootTown_BrendansHouse_1F_EventScript_1F89F3, 784, 0, 0
	object_event 5, MAP_OBJ_GFX_NORMAN, 0, 0, 5, 6, 3, 9, 17, 0, 0, 0, 0x0, 734, 0, 0
	object_event 6, MAP_OBJ_GFX_LITTLE_BOY_1, 0, 0, 1, 5, 3, 5, 17, 0, 0, 0, LittlerootTown_BrendansHouse_1F_EventScript_1F8A3D, 735, 0, 0
	object_event 7, MAP_OBJ_GFX_RIVAL_BRENDAN_NORMAL, 0, 0, 8, 8, 3, 7, 17, 0, 0, 0, 0x0, 745, 0, 0

LittlerootTown_BrendansHouse_1F_MapWarps: @ 852D134
	warp_def 9, 8, 0, 1, LITTLEROOT_TOWN
	warp_def 8, 8, 0, 1, LITTLEROOT_TOWN
	warp_def 8, 2, 0, 0, LITTLEROOT_TOWN_BRENDANS_HOUSE_2F

LittlerootTown_BrendansHouse_1F_MapCoordEvents: @ 852D14C
	coord_event 8, 8, 0, 0, VAR_0x4092, 4, 0, LittlerootTown_BrendansHouse_1F_EventScript_1F78A3
	coord_event 7, 3, 0, 0, VAR_0x408D, 2, 0, LittlerootTown_BrendansHouse_1F_EventScript_1F78BE
	coord_event 8, 4, 0, 0, VAR_0x408D, 2, 0, LittlerootTown_BrendansHouse_1F_EventScript_1F78CA
	coord_event 9, 3, 0, 0, VAR_0x408D, 2, 0, LittlerootTown_BrendansHouse_1F_EventScript_1F78D6

LittlerootTown_BrendansHouse_1F_MapEvents:: @ 852D18C
	map_events LittlerootTown_BrendansHouse_1F_MapObjects, LittlerootTown_BrendansHouse_1F_MapWarps, LittlerootTown_BrendansHouse_1F_MapCoordEvents, 0x0

