LilycoveCity_PokemonCenter_1F_MapObjects: @ 8531BCC
	object_event 1, MAP_OBJ_GFX_NURSE, 0, 0, 7, 2, 3, 8, 0, 0, 0, 0, LilycoveCity_PokemonCenter_1F_EventScript_21C5E8, 0, 0, 0
	object_event 2, MAP_OBJ_GFX_BOY_1, 0, 0, 10, 7, 3, 10, 0, 0, 0, 0, LilycoveCity_PokemonCenter_1F_EventScript_21C5F6, 0, 0, 0
	object_event 3, MAP_OBJ_GFX_MANIAC, 0, 0, 11, 8, 3, 7, 0, 0, 0, 0, LilycoveCity_PokemonCenter_1F_EventScript_21C5FF, 0, 0, 0
	object_event 4, MAP_OBJ_GFX_HIPSTER, 0, 0, 1, 3, 3, 8, 0, 0, 0, 0, LilycoveCity_PokemonCenter_1F_EventScript_2A836B, 0, 0, 0
	object_event 5, MAP_OBJ_GFX_TRADER, 0, 0, 2, 3, 3, 8, 0, 0, 0, 0, LilycoveCity_PokemonCenter_1F_EventScript_2A89C7, 993, 0, 0

LilycoveCity_PokemonCenter_1F_MapWarps: @ 8531C44
	warp_def 7, 8, 3, 2, LILYCOVE_CITY
	warp_def 6, 8, 3, 2, LILYCOVE_CITY
	warp_def 1, 6, 4, 0, LILYCOVE_CITY_POKEMON_CENTER_2F

LilycoveCity_PokemonCenter_1F_MapEvents:: @ 8531C5C
	map_events LilycoveCity_PokemonCenter_1F_MapObjects, LilycoveCity_PokemonCenter_1F_MapWarps, 0x0, 0x0

