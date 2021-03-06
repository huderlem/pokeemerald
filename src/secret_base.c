
// Includes
#include "global.h"
#include "constants/decorations.h"
#include "malloc.h"
#include "task.h"
#include "palette.h"
#include "list_menu.h"
#include "window.h"
#include "menu.h"
#include "menu_helpers.h"
#include "new_menu_helpers.h"
#include "menu_indicators.h"
#include "constants/maps.h"
#include "constants/songs.h"
#include "sound.h"
#include "overworld.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "field_screen.h"
#include "field_weather.h"
#include "field_map_obj.h"
#include "field_effect.h"
#include "fldeff_80F9BCC.h"
#include "metatile_behavior.h"
#include "map_name_popup.h"
#include "string_util.h"
#include "script.h"
#include "event_scripts.h"
#include "strings.h"
#include "international_string_util.h"
#include "event_data.h"
#include "battle.h"
#include "rom6.h"
#include "decoration.h"
#include "link.h"
#include "tv.h"
#include "secret_base.h"

// Static type declarations

struct SecretBaseListMenuBuffer {
    struct ListMenuItem items[11];
    u8 names[11][32];
};

struct SecretBaseRecordMixer {
    struct SecretBaseRecord *records;
    u32 version;
    u32 language;
};

// Static RAM declarations
EWRAM_DATA u8 sCurSecretBaseId = 0;
EWRAM_DATA u8 gUnknown_0203A01D = 0;
EWRAM_DATA struct SecretBaseListMenuBuffer *gUnknown_0203A020 = NULL;

// Static ROM declarations

void sub_80E9C9C(u8 taskId);
void game_continue(u8 taskId);
void sub_80E9DEC(u32 a0, bool8 flag, struct ListMenu *menu);
void sub_80E9E00(u8 taskId);
void sub_80E9E44(u8 taskId);
void sub_80E9E90(u8 taskId);
void sub_80E9F20(u8 taskId);
void sub_80E9FB0(u8 taskId);
void sub_80E9FFC(u8 taskId);
void sub_80EA06C(u8 taskId);
void sub_80EA120(u8 taskId);
void sub_80EA13C(u8 taskId);
void sub_80EA18C(u8 taskId);
void task_pc_turn_off(u8 taskId);
u8 sub_80EA20C(u8 secretBaseRecordId);

// .rodata

const struct {
    u16 tile1;
    u16 tile2;
} gUnknown_0858CFCC[] = {
    {0x0026, 0x0036},
    {0x0027, 0x0037},
    {0x01a0, 0x01a1},
    {0x01a8, 0x01a9},
    {0x01b0, 0x01b1},
    {0x0208, 0x0210},
    {0x0271, 0x0278}
};

const u8 gUnknown_0858CFE8[] = {
    MAP_NUM(SECRET_BASE_RED_CAVE1),    0x00, 0x01, 0x03,
    MAP_NUM(SECRET_BASE_RED_CAVE2),    0x00, 0x05, 0x09,
    MAP_NUM(SECRET_BASE_RED_CAVE3),    0x00, 0x01, 0x03,
    MAP_NUM(SECRET_BASE_RED_CAVE4),    0x00, 0x07, 0x0d,
    MAP_NUM(SECRET_BASE_BROWN_CAVE1),  0x00, 0x02, 0x03,
    MAP_NUM(SECRET_BASE_BROWN_CAVE2),  0x00, 0x09, 0x02,
    MAP_NUM(SECRET_BASE_BROWN_CAVE3),  0x00, 0x0d, 0x04,
    MAP_NUM(SECRET_BASE_BROWN_CAVE4),  0x00, 0x01, 0x02,
    MAP_NUM(SECRET_BASE_BLUE_CAVE1),   0x00, 0x01, 0x03,
    MAP_NUM(SECRET_BASE_BLUE_CAVE2),   0x00, 0x01, 0x02,
    MAP_NUM(SECRET_BASE_BLUE_CAVE3),   0x00, 0x03, 0x0f,
    MAP_NUM(SECRET_BASE_BLUE_CAVE4),   0x00, 0x03, 0x0e,
    MAP_NUM(SECRET_BASE_YELLOW_CAVE1), 0x00, 0x09, 0x03,
    MAP_NUM(SECRET_BASE_YELLOW_CAVE2), 0x00, 0x08, 0x07,
    MAP_NUM(SECRET_BASE_YELLOW_CAVE3), 0x00, 0x03, 0x06,
    MAP_NUM(SECRET_BASE_YELLOW_CAVE4), 0x00, 0x05, 0x09,
    MAP_NUM(SECRET_BASE_TREE1),        0x00, 0x02, 0x03,
    MAP_NUM(SECRET_BASE_TREE2),        0x00, 0x05, 0x06,
    MAP_NUM(SECRET_BASE_TREE3),        0x00, 0x0f, 0x03,
    MAP_NUM(SECRET_BASE_TREE4),        0x00, 0x04, 0x0a,
    MAP_NUM(SECRET_BASE_SHRUB1),       0x00, 0x03, 0x03,
    MAP_NUM(SECRET_BASE_SHRUB2),       0x00, 0x01, 0x02,
    MAP_NUM(SECRET_BASE_SHRUB3),       0x00, 0x07, 0x08,
    MAP_NUM(SECRET_BASE_SHRUB4),       0x00, 0x09, 0x06
};

const struct MenuAction gUnknown_0858D048[] = {
    {gUnknown_085EA79D, {.void_u8 = sub_80E9FFC}},
    {gText_Cancel, {.void_u8 = sub_80EA18C}}
};

const struct YesNoFuncTable gUnknown_0858D058 = {
    sub_80EA120, sub_80EA13C
};

const u8 gUnknown_0858D060[10] = {
    0x23, 0x24, 0x0f, 0x1f, 0x21,
    0x2f, 0x0e, 0x14, 0x20, 0x22
};

const struct WindowTemplate gUnknown_0858D06C[] = {
    { 0, 18, 1, 11, 18, 15, 0x01 },
    { 0,  2, 1, 28,  4, 15, 0xc7 }
};

const struct ListMenuTemplate gUnknown_0858D07C = {
    NULL,
    sub_80E9DEC,
    NULL,
    0, 0,
    0, 0, 8, 0,
    9, 2, 1, 3, FALSE, 0, FALSE, 1
};

// .text

void ClearSecretBase(struct SecretBaseRecord *sbr)
{
    u16 i;

    CpuFastFill16(0, sbr, sizeof(struct SecretBaseRecord));
    for (i = 0; i < 7; i ++)
    {
        sbr->trainerName[i] = EOS;
    }
}

void ResetSecretBases(void)
{
    u16 i;

    for (i = 0; i < 20; i ++)
    {
        ClearSecretBase(&gSaveBlock1Ptr->secretBases[i]);
    }
}

void sub_80E8B58(void)
{
    sCurSecretBaseId = gSpecialVar_0x8004;
}

void sub_80E8B6C(void)
{
    u16 i;

    gSpecialVar_Result = FALSE;
    for (i = 0; i < 20; i ++)
    {
        if (sCurSecretBaseId != gSaveBlock1Ptr->secretBases[i].secretBaseId)
        {
            continue;
        }
        gSpecialVar_Result = TRUE;
        VarSet(VAR_0x4054, i);
        break;
    }
}

void sub_80E8BC8(void)
{
    if (gSaveBlock1Ptr->secretBases[0].secretBaseId != 0)
    {
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

u8 sub_80E8BF8(void)
{
    s16 x;
    s16 y;
    s16 behavior;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    behavior = MapGridGetMetatileBehaviorAt(x, y) & 0xFFF;
    if (behavior == 0x90 || behavior == 0x91)
    {
        return 1;
    }
    if (behavior == 0x92 || behavior == 0x93)
    {
        return 2;
    }
    if (behavior == 0x9a || behavior == 0x9b)
    {
        return 3;
    }
    if (behavior == 0x94 || behavior == 0x95)
    {
        return 4;
    }
    if (behavior == 0x96 || behavior == 0x97 || behavior == 0x9c || behavior == 0x9d)
    {
        return 5;
    }
    if (behavior == 0x98 || behavior == 0x99)
    {
        return 6;
    }
    return 0;
}

void sub_80E8C98(void)
{
    gSpecialVar_0x8007 = sub_80E8BF8();
}

void sub_80E8CB0(s16 *xPtr, s16 *yPtr, u16 tile)
{
    struct MapData *mapData;
    s16 x;
    s16 y;

    mapData = gMapHeader.mapData;
    for (y = 0; y < mapData->height; y ++)
    {
        for (x = 0; x < mapData->width; x ++)
        {
            if ((mapData->map[y * mapData->width + x] & 0x3ff) == tile)
            {
                *xPtr = x;
                *yPtr = y;
                return;
            }
        }
    }
}

void sub_80E8D4C(void)
{
    s16 x;
    s16 y;
    s16 tile;
    u16 i;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    tile = MapGridGetMetatileIdAt(x, y);
    for (i = 0; i < 7; i ++)
    {
        if (gUnknown_0858CFCC[i].tile1 == tile)
        {
            MapGridSetMetatileIdAt(x, y, gUnknown_0858CFCC[i].tile2 | 0xC00);
            CurrentMapDrawMetatileAt(x, y);
            return;
        }
    }
    for (i = 0; i < 7; i ++)
    {
        if (gUnknown_0858CFCC[i].tile2 == tile)
        {
            MapGridSetMetatileIdAt(x, y, gUnknown_0858CFCC[i].tile1 | 0xC00);
            CurrentMapDrawMetatileAt(x, y);
            return;
        }
    }
}

u8 sub_80E8DF4(const u8 *src)
{
    u8 i;

    for (i = 0; i < 7; i ++)
    {
        if (src[i] == EOS)
        {
            return i;
        }
    }
    return 7;
}

void sub_80E8E18(void)
{
    u16 i;

    gSaveBlock1Ptr->secretBases[0].secretBaseId = sCurSecretBaseId;
    for (i = 0; i < 4; i ++)
    {
        gSaveBlock1Ptr->secretBases[0].trainerId[i] = gSaveBlock2Ptr->playerTrainerId[i];
    }
    VarSet(VAR_0x4054, 0);
    StringCopyN(gSaveBlock1Ptr->secretBases[0].trainerName, gSaveBlock2Ptr->playerName, sub_80E8DF4(gSaveBlock2Ptr->playerName));
    gSaveBlock1Ptr->secretBases[0].gender = gSaveBlock2Ptr->playerGender;
    gSaveBlock1Ptr->secretBases[0].language = GAME_LANGUAGE;
    VarSet(VAR_SECRET_BASE_MAP, gMapHeader.regionMapSectionId);
}

void sub_80E8EE0(struct MapEvents *events)
{
    u16 bgEventIndex;
    u16 i;
    u16 j;
    s16 tile_id;
    s16 x;
    s16 y;

    for (bgEventIndex = 0; bgEventIndex < events->bgEventCount; bgEventIndex ++)
    {
        if (events->bgEvents[bgEventIndex].kind == 8)
        {
            for (j = 0; j < 20; j ++)
            {
                if (gSaveBlock1Ptr->secretBases[j].secretBaseId == events->bgEvents[bgEventIndex].bgUnion.secretBaseId)
                {
                    x = events->bgEvents[bgEventIndex].x + 7;
                    y = events->bgEvents[bgEventIndex].y + 7;
                    tile_id = MapGridGetMetatileIdAt(x, y);
                    for (i = 0; i < 7; i ++)
                    {
                        if (gUnknown_0858CFCC[i].tile1 == tile_id)
                        {
                            MapGridSetMetatileIdAt(x, y, gUnknown_0858CFCC[i].tile2 | 0xc00);
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}

void sub_80E8F9C(void)
{
    s8 idx;

    idx = sCurSecretBaseId / 10 * 4;
    warp1_set_2(MAP_GROUP(SECRET_BASE_RED_CAVE1), gUnknown_0858CFE8[idx], gUnknown_0858CFE8[idx + 1]);
}

void sub_80E8FD0(u8 taskId)
{
    u16 secretBaseRecordId;

    switch (gTasks[taskId].data[0])
    {
        case 0:
            if (!gPaletteFade.active)
            {
                gTasks[taskId].data[0] = 1;
            }
            break;
        case 1:
            secretBaseRecordId = VarGet(VAR_0x4054);
            if (gSaveBlock1Ptr->secretBases[secretBaseRecordId].sbr_field_10 < 255)
            {
                gSaveBlock1Ptr->secretBases[secretBaseRecordId].sbr_field_10 ++;
            }
            sub_80E8F9C();
            warp_in();
            gFieldCallback = sub_80AF168;
            SetMainCallback2(c2_load_new_map);
            DestroyTask(taskId);
            break;
    }
}

void sub_80E9068(void)
{
    CreateTask(sub_80E8FD0, 0);
    fade_screen(1, 0);
    saved_warp2_set(0, gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, -1);
}

bool8 sub_80E909C(void)
{
    if (gMapHeader.mapType == MAP_TYPE_SECRET_BASE && VarGet(VAR_0x4097) == 0)
    {
        return FALSE;
    }
    return TRUE;
}

void sub_80E90C8(u8 taskId)
{
    FieldObjectTurn(&gMapObjects[gPlayerAvatar.mapObjectId], DIR_NORTH);
    if (sub_80ABDFC() == TRUE)
    {
        EnableBothScriptContexts();
        DestroyTask(taskId);
    }
}

void sub_80E9108(void)
{
    s16 x;
    s16 y;

    ScriptContext2_Enable();
    HideMapNamePopUpWindow();
    sub_80E8CB0(&x, &y, 0x220);
    x += 7;
    y += 7;
    MapGridSetMetatileIdAt(x, y, 0x220 | 0xC00);
    CurrentMapDrawMetatileAt(x, y);
    pal_fill_black();
    CreateTask(sub_80E90C8, 0);
}

void sub_80E916C(u8 taskId)
{
    s8 idx;

    if (!gPaletteFade.active)
    {
        idx = sCurSecretBaseId / 10 * 4;
        Overworld_SetWarpDestination(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum, -1, gUnknown_0858CFE8[idx + 2], gUnknown_0858CFE8[idx + 3]);
        warp_in();
        gFieldCallback = sub_80E9108;
        SetMainCallback2(c2_load_new_map);
        DestroyTask(taskId);
    }
}

void sub_80E91F8(void)
{
    CreateTask(sub_80E916C, 0);
    fade_screen(1, 0);
}

bool8 CurrentMapIsSecretBase(void)
{
    if (gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(SECRET_BASE_RED_CAVE1) && (u8)gSaveBlock1Ptr->location.mapNum <= MAP_NUM(SECRET_BASE_SHRUB4))
    {
        return TRUE;
    }
    return FALSE;
}

void sub_80E9238(u8 flagIn)
{
    u16 curBaseId;
    u16 x;
    u16 y;
    u8 *decorations;
    u8 *decorPos;

    if (CurrentMapIsSecretBase())
    {
        curBaseId = VarGet(VAR_0x4054);
        decorations = gSaveBlock1Ptr->secretBases[curBaseId].decorations;
        decorPos = gSaveBlock1Ptr->secretBases[curBaseId].decorationPos;
        for (x = 0; x < 16; x ++)
        {
            if (decorations[x] > 0 && decorations[x] <= 0x78 && gDecorations[decorations[x]].permission != DECORPERM_SOLID_MAT) {
                sub_8127D38((decorPos[x] >> 4) + 7, (decorPos[x] & 0xF) + 7, decorations[x]);
            }
        }
        if (curBaseId != 0)
        {
            sub_80E8CB0(&x, &y, 0x220);
            MapGridSetMetatileIdAt(x + 7, y + 7, 0x221 | 0xc00);
        }
        else if (flagIn == 1 && VarGet(VAR_0x4089) == 1)
        {
            sub_80E8CB0(&x, &y, 0x220);
            MapGridSetMetatileIdAt(x + 7, y + 7, 0x20a | 0xc00);
        }
    }
}

void sub_80E933C(void)
{
    u8 *roomDecor;
    u8 *roomDecorPos;
    u8 decorIdx;
    u8 objIdx;
    u8 metatile;
    u8 category;
    u8 permission;
    u8 nDecor;
    u16 curBase;

    objIdx = 0;
    if (!CurrentMapIsSecretBase())
    {
        roomDecor = gSaveBlock1Ptr->playerRoomDecor;
        roomDecorPos = gSaveBlock1Ptr->playerRoomDecorPos;
        nDecor = 12;
    }
    else
    {
        curBase = VarGet(VAR_0x4054);
        roomDecor = gSaveBlock1Ptr->secretBases[curBase].decorations;
        roomDecorPos = gSaveBlock1Ptr->secretBases[curBase].decorationPos;
        nDecor = 16;
    }
    for (decorIdx = 0; decorIdx < nDecor; decorIdx ++)
    {
        if (roomDecor[decorIdx] != DECOR_NONE)
        {
            permission = gDecorations[roomDecor[decorIdx]].permission;
            category = gDecorations[roomDecor[decorIdx]].category;
            if (permission == DECORPERM_SOLID_MAT)
            {
                for (objIdx = 0; objIdx < gMapHeader.events->mapObjectCount; objIdx ++)
                {
                    if (gMapHeader.events->mapObjects[objIdx].flagId == gSpecialVar_0x8004 + 0xAE)
                    {
                        break;
                    }
                }
                if (objIdx == gMapHeader.events->mapObjectCount)
                {
                    continue;
                }
                gSpecialVar_0x8006 = roomDecorPos[decorIdx] >> 4;
                gSpecialVar_0x8007 = roomDecorPos[decorIdx] & 0xF;
                metatile = MapGridGetMetatileBehaviorAt(gSpecialVar_0x8006 + 7, gSpecialVar_0x8007 + 7);
                if (MetatileBehavior_IsMB_B5(metatile) == TRUE || MetatileBehavior_IsMB_C3(metatile) == TRUE)
                {
                    gSpecialVar_Result = gMapHeader.events->mapObjects[objIdx].graphicsId + VAR_0x3F20;
                    VarSet(gSpecialVar_Result, gDecorations[roomDecor[decorIdx]].tiles[0]);
                    gSpecialVar_Result = gMapHeader.events->mapObjects[objIdx].localId;
                    FlagClear(gSpecialVar_0x8004 + 0xAE);
                    show_sprite(gSpecialVar_Result, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
                    sub_808EBA8(gSpecialVar_Result, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, gSpecialVar_0x8006, gSpecialVar_0x8007);
                    sub_808F254(gSpecialVar_Result, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
                    if (CurrentMapIsSecretBase() == TRUE && VarGet(VAR_0x4054) != 0)
                    {
                        if (category == DECORCAT_DOLL)
                        {
                            sub_808F28C(gSpecialVar_Result, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, DECORCAT_DOLL);
                        }
                        else if (category == DECORCAT_CUSHION)
                        {
                            sub_808F28C(gSpecialVar_Result, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, DECORCAT_CUSHION);
                        }
                    }
                    gSpecialVar_0x8004 ++;
                }
            }
        }
    }
}

void sub_80E9578(void)
{
    u8 objectEventIdx;
    u16 flagId;

    for (objectEventIdx = 0; objectEventIdx < gMapHeader.events->mapObjectCount; objectEventIdx ++)
    {
        flagId = gMapHeader.events->mapObjects[objectEventIdx].flagId;
        if (flagId >= 0xAE && flagId <= 0xBB)
        {
            RemoveFieldObjectByLocalIdAndMap(gMapHeader.events->mapObjects[objectEventIdx].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
            FlagSet(flagId);
        }
    }
}

void sub_80E95D4(void)
{
    VarSet(VAR_0x401F, gUnknown_0858D060[sub_80EA20C(VarGet(VAR_0x4054))]);
}

void sub_80E9608(struct Coords16 *coords, struct MapEvents *events)
{
    s16 bgEventIdx;

    for (bgEventIdx = 0; bgEventIdx < events->bgEventCount; bgEventIdx ++)
    {
        if (events->bgEvents[bgEventIdx].kind == 8 && coords->x == events->bgEvents[bgEventIdx].x + 7 && coords->y == events->bgEvents[bgEventIdx].y + 7)
        {
            sCurSecretBaseId = events->bgEvents[bgEventIdx].bgUnion.secretBaseId;
            break;
        }
    }
}

void sub_80E9668(struct Coords16 *coords, struct MapEvents *events)
{
    sub_80E9608(coords, events);
    sub_80E8B6C();
    ScriptContext1_SetupScript(EventScript_275BB7);
}

bool8 sub_80E9680(void)
{
    sub_80E8B58();
    sub_80E8B6C();
    if (gSpecialVar_Result == TRUE)
    {
        return FALSE;
    }
    return TRUE;
}

void sub_80E96A4(u8 taskId)
{
    switch (gTasks[taskId].data[0])
    {
        case 0:
            ScriptContext2_Enable();
            gTasks[taskId].data[0] = 1;
            break;
        case 1:
            if (!gPaletteFade.active)
            {
                gTasks[taskId].data[0] = 2;
            }
            break;
        case 2:
            copy_saved_warp2_bank_and_enter_x_to_warp1(0x7e);
            warp_in();
            gFieldCallback = mapldr_default;
            SetMainCallback2(c2_load_new_map);
            ScriptContext2_Disable();
            DestroyTask(taskId);
            break;
    }
}

void sub_80E9728(void)
{
    CreateTask(sub_80E96A4, 0);
    fade_screen(1, 0);
}

void sub_80E9744(void)
{
    if (gSaveBlock1Ptr->secretBases[0].secretBaseId != sCurSecretBaseId)
    {
        gSpecialVar_Result = TRUE;
    }
    else
    {
        gSpecialVar_Result = FALSE;
    }
}

u8 *sub_80E9780(u8 *dest, u8 secretBaseRecordId)
{
    *StringCopyN(dest, gSaveBlock1Ptr->secretBases[secretBaseRecordId].trainerName, sub_80E8DF4(gSaveBlock1Ptr->secretBases[secretBaseRecordId].trainerName)) = EOS;
    ConvertInternationalString(dest, gSaveBlock1Ptr->secretBases[secretBaseRecordId].language);
    return StringAppend(dest, gText_ApostropheSBase);
}

u8 *GetSecretBaseMapName(u8 *dest)
{
    return sub_80E9780(dest, VarGet(VAR_0x4054));
}

void sub_80E980C(void)
{
    u8 secretBaseRecordId;
    const u8 *src;

    secretBaseRecordId = VarGet(VAR_0x4054);
    src = gSaveBlock1Ptr->secretBases[secretBaseRecordId].trainerName;
    *StringCopyN(gStringVar1, src, sub_80E8DF4(src)) = EOS;
    ConvertInternationalString(gStringVar1, gSaveBlock1Ptr->secretBases[secretBaseRecordId].language);
}

bool8 sub_80E9878(u8 secretBaseRecordId)
{
    if (gSaveBlock1Ptr->secretBases[secretBaseRecordId].sbr_field_1_6 != 0)
    {
        return TRUE;
    }
    return FALSE;
}

u8 sub_80E98AC(struct Pokemon *pokemon)
{
    u16 evTotal;

    evTotal  = GetMonData(pokemon, MON_DATA_HP_EV);
    evTotal += GetMonData(pokemon, MON_DATA_ATK_EV);
    evTotal += GetMonData(pokemon, MON_DATA_DEF_EV);
    evTotal += GetMonData(pokemon, MON_DATA_SPEED_EV);
    evTotal += GetMonData(pokemon, MON_DATA_SPATK_EV);
    evTotal += GetMonData(pokemon, MON_DATA_SPDEF_EV);
    return evTotal / 6;
}

#ifdef NONMATCHING
// This function is a meme
void sub_80E9914(void)
{
    u32 zero;
    u32 *personality;
    u16 partyIdx;
    u16 moveIdx;
    u16 sbPartyIdx;
    u16 *species;
    u16 *items;
    u16 *moves;
    u8 *levels;
    u8 *evs;

    sbPartyIdx = 0;
    personality = gSaveBlock1Ptr->secretBases[0].partyPersonality;
    if (gSaveBlock1Ptr->secretBases[0].secretBaseId != 0)
    {
        partyIdx = 0;
        moves = gSaveBlock1Ptr->secretBases[0].partyMoves;
        species = gSaveBlock1Ptr->secretBases[0].partySpecies;
        items = gSaveBlock1Ptr->secretBases[0].partyHeldItems;
        levels = gSaveBlock1Ptr->secretBases[0].partyLevels;
        evs = gSaveBlock1Ptr->secretBases[0].partyEVs;
        zero = 0;
        for (partyIdx = 0; partyIdx < PARTY_SIZE; partyIdx ++)
        {
            for (moveIdx = 0; moveIdx < 4; moveIdx ++)
            {
                moves[partyIdx * 4 + moveIdx] = zero;
            }
            species[partyIdx] = zero;
            items[partyIdx] = zero;
            levels[partyIdx] = zero;
            personality[partyIdx] = zero;
            evs[partyIdx] = zero;
            if (GetMonData(&gPlayerParty[partyIdx], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&gPlayerParty[partyIdx], MON_DATA_IS_EGG))
            {
                for (moveIdx = 0; moveIdx < 4; moveIdx ++)
                {
                    moves[sbPartyIdx * 4 + moveIdx] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_MOVE1 + moveIdx);
                }
                species[sbPartyIdx] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_SPECIES);
                items[sbPartyIdx] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_HELD_ITEM);
                levels[sbPartyIdx] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_LEVEL);
                personality[sbPartyIdx] = GetMonData(&gPlayerParty[partyIdx], MON_DATA_PERSONALITY);
                evs[sbPartyIdx] = sub_80E98AC(&gPlayerParty[partyIdx]);
                sbPartyIdx ++;
            }
        }
    }
}
#else
__attribute__((naked)) void sub_80E9914(void)
{
    asm_unified("\tpush {r4-r7,lr}\n"
                    "\tmov r7, r10\n"
                    "\tmov r6, r9\n"
                    "\tmov r5, r8\n"
                    "\tpush {r5-r7}\n"
                    "\tsub sp, 0x24\n"
                    "\tmovs r0, 0\n"
                    "\tmov r10, r0\n"
                    "\tldr r0, =gSaveBlock1Ptr\n"
                    "\tldr r1, [r0]\n"
                    "\tldr r2, =0x00001ad0\n"
                    "\tadds r2, r1, r2\n"
                    "\tstr r2, [sp]\n"
                    "\tldr r3, =0x00001a9c\n"
                    "\tadds r0, r1, r3\n"
                    "\tldrb r0, [r0]\n"
                    "\tcmp r0, 0\n"
                    "\tbne _080E993A\n"
                    "\tb _080E9A60\n"
                    "_080E993A:\n"
                    "\tmovs r6, 0\n"
                    "\tldr r7, =0x00001ae8\n"
                    "\tadds r7, r1, r7\n"
                    "\tstr r7, [sp, 0x14]\n"
                    "\tldr r0, =0x00001b18\n"
                    "\tadds r0, r1, r0\n"
                    "\tstr r0, [sp, 0xC]\n"
                    "\tldr r2, =0x00001b24\n"
                    "\tadds r2, r1, r2\n"
                    "\tstr r2, [sp, 0x10]\n"
                    "\tadds r3, 0x94\n"
                    "\tadds r3, r1, r3\n"
                    "\tstr r3, [sp, 0x18]\n"
                    "\tldr r7, =0x00001b36\n"
                    "\tadds r7, r1, r7\n"
                    "\tstr r7, [sp, 0x1C]\n"
                    "\tmov r9, r6\n"
                    "_080E995C:\n"
                    "\tmovs r4, 0\n"
                    "\tlsls r5, r6, 2\n"
                    "\tlsls r3, r6, 1\n"
                    "\tldr r0, =gPlayerParty\n"
                    "\tmov r8, r0\n"
                    "\tadds r1, r6, 0x1\n"
                    "\tstr r1, [sp, 0x4]\n"
                    "\tadds r2, r5, 0\n"
                    "\tldr r1, [sp, 0x14]\n"
                    "_080E996E:\n"
                    "\tadds r0, r2, r4\n"
                    "\tlsls r0, 1\n"
                    "\tadds r0, r1, r0\n"
                    "\tmov r7, r9\n"
                    "\tstrh r7, [r0]\n"
                    "\tadds r0, r4, 0x1\n"
                    "\tlsls r0, 16\n"
                    "\tlsrs r4, r0, 16\n"
                    "\tcmp r4, 0x3\n"
                    "\tbls _080E996E\n"
                    "\tldr r1, [sp, 0xC]\n"
                    "\tadds r0, r1, r3\n"
                    "\tmov r2, r9\n"
                    "\tstrh r2, [r0]\n"
                    "\tldr r7, [sp, 0x10]\n"
                    "\tadds r0, r7, r3\n"
                    "\tstrh r2, [r0]\n"
                    "\tldr r1, [sp, 0x18]\n"
                    "\tadds r0, r1, r6\n"
                    "\tmov r2, r9\n"
                    "\tstrb r2, [r0]\n"
                    "\tldr r3, [sp]\n"
                    "\tadds r0, r3, r5\n"
                    "\tmov r7, r9\n"
                    "\tstr r7, [r0]\n"
                    "\tldr r1, [sp, 0x1C]\n"
                    "\tadds r0, r1, r6\n"
                    "\tstrb r7, [r0]\n"
                    "\tmovs r2, 0x64\n"
                    "\tadds r5, r6, 0\n"
                    "\tmuls r5, r2\n"
                    "\tmov r3, r8\n"
                    "\tadds r4, r5, r3\n"
                    "\tadds r0, r4, 0\n"
                    "\tmovs r1, 0xB\n"
                    "\tbl GetMonData\n"
                    "\tcmp r0, 0\n"
                    "\tbeq _080E9A54\n"
                    "\tadds r0, r4, 0\n"
                    "\tmovs r1, 0x2D\n"
                    "\tbl GetMonData\n"
                    "\tcmp r0, 0\n"
                    "\tbne _080E9A54\n"
                    "\tmovs r4, 0\n"
                    "\tmov r7, r10\n"
                    "\tlsls r7, 2\n"
                    "\tmov r8, r7\n"
                    "\tmov r0, r10\n"
                    "\tlsls r7, r0, 1\n"
                    "\tadds r0, 0x1\n"
                    "\tstr r0, [sp, 0x8]\n"
                    "\tldr r2, =gPlayerParty\n"
                    "_080E99DA:\n"
                    "\tadds r1, r4, 0\n"
                    "\tadds r1, 0xD\n"
                    "\tadds r0, r5, r2\n"
                    "\tstr r2, [sp, 0x20]\n"
                    "\tbl GetMonData\n"
                    "\tmov r3, r8\n"
                    "\tadds r1, r3, r4\n"
                    "\tlsls r1, 1\n"
                    "\tldr r3, [sp, 0x14]\n"
                    "\tadds r1, r3, r1\n"
                    "\tstrh r0, [r1]\n"
                    "\tadds r0, r4, 0x1\n"
                    "\tlsls r0, 16\n"
                    "\tlsrs r4, r0, 16\n"
                    "\tldr r2, [sp, 0x20]\n"
                    "\tcmp r4, 0x3\n"
                    "\tbls _080E99DA\n"
                    "\tmovs r0, 0x64\n"
                    "\tadds r4, r6, 0\n"
                    "\tmuls r4, r0\n"
                    "\tldr r0, =gPlayerParty\n"
                    "\tadds r4, r0\n"
                    "\tadds r0, r4, 0\n"
                    "\tmovs r1, 0xB\n"
                    "\tbl GetMonData\n"
                    "\tldr r2, [sp, 0xC]\n"
                    "\tadds r1, r2, r7\n"
                    "\tstrh r0, [r1]\n"
                    "\tadds r0, r4, 0\n"
                    "\tmovs r1, 0xC\n"
                    "\tbl GetMonData\n"
                    "\tldr r3, [sp, 0x10]\n"
                    "\tadds r1, r3, r7\n"
                    "\tstrh r0, [r1]\n"
                    "\tadds r0, r4, 0\n"
                    "\tmovs r1, 0x38\n"
                    "\tbl GetMonData\n"
                    "\tldr r1, [sp, 0x18]\n"
                    "\tadd r1, r10\n"
                    "\tstrb r0, [r1]\n"
                    "\tadds r0, r4, 0\n"
                    "\tmovs r1, 0\n"
                    "\tbl GetMonData\n"
                    "\tldr r1, [sp]\n"
                    "\tadd r1, r8\n"
                    "\tstr r0, [r1]\n"
                    "\tadds r0, r4, 0\n"
                    "\tbl sub_80E98AC\n"
                    "\tldr r1, [sp, 0x1C]\n"
                    "\tadd r1, r10\n"
                    "\tstrb r0, [r1]\n"
                    "\tldr r7, [sp, 0x8]\n"
                    "\tlsls r0, r7, 16\n"
                    "\tlsrs r0, 16\n"
                    "\tmov r10, r0\n"
                    "_080E9A54:\n"
                    "\tldr r1, [sp, 0x4]\n"
                    "\tlsls r0, r1, 16\n"
                    "\tlsrs r6, r0, 16\n"
                    "\tcmp r6, 0x5\n"
                    "\tbhi _080E9A60\n"
                    "\tb _080E995C\n"
                    "_080E9A60:\n"
                    "\tadd sp, 0x24\n"
                    "\tpop {r3-r5}\n"
                    "\tmov r8, r3\n"
                    "\tmov r9, r4\n"
                    "\tmov r10, r5\n"
                    "\tpop {r4-r7}\n"
                    "\tpop {r0}\n"
                    "\tbx r0\n"
                    "\t.pool");
}
#endif

void sub_80E9A90(void)
{
    u16 sbr_e;

    sbr_e = gSaveBlock1Ptr->secretBases[0].sbr_field_e;
    ClearSecretBase(&gSaveBlock1Ptr->secretBases[0]);
    gSaveBlock1Ptr->secretBases[0].sbr_field_e = sbr_e;
    sub_80E9728();
}

void sub_80E9AC0(void)
{
    IncrementGameStat(GAME_STAT_MOVED_SECRET_BASE);
    sub_80E9A90();
}

void sub_80E9AD0(void)
{
    u16 i;
    u16 j;
    s16 tile;
    struct MapEvents *events;

    events = gMapHeader.events;
    for (i = 0; i < events->bgEventCount; i ++)
    {
        if (events->bgEvents[i].kind == 8 && gSaveBlock1Ptr->secretBases[0].secretBaseId == events->bgEvents[i].bgUnion.secretBaseId)
        {
            tile = MapGridGetMetatileIdAt(events->bgEvents[i].x + 7, events->bgEvents[i].y + 7);
            for (j = 0; j < 7; j ++)
            {
                if (gUnknown_0858CFCC[j].tile2 == tile)
                {
                    MapGridSetMetatileIdAt(events->bgEvents[i].x + 7, events->bgEvents[i].y + 7, gUnknown_0858CFCC[j].tile1 | 0xc00);
                    break;
                }
            }
            DrawWholeMapView();
            break;
        }
    }
}

void sub_80E9B70(void)
{
    u16 sbr_e;

    sub_80E9AD0();
    IncrementGameStat(GAME_STAT_MOVED_SECRET_BASE);
    sbr_e = gSaveBlock1Ptr->secretBases[0].sbr_field_e;
    ClearSecretBase(&gSaveBlock1Ptr->secretBases[0]);
    gSaveBlock1Ptr->secretBases[0].sbr_field_e = sbr_e;
}

u8 sub_80E9BA8(void)
{
    u8 sum;
    s16 i;

    sum = 0;
    for (i = 1; i < 20; i ++)
    {
        if (sub_80E9878(i) == TRUE)
        {
            sum ++;
        }
    }
    return sum;
}

void sub_80E9BDC(void)
{
    if (sub_80E9878(VarGet(VAR_0x4054)) == TRUE)
    {
        gSpecialVar_Result = 1;
    }
    else if (sub_80E9BA8() > 9)
    {
        gSpecialVar_Result = 2;
    }
    else
    {
        gSpecialVar_Result = 0;
    }
}

void sub_80E9C2C(void)
{
    gSaveBlock1Ptr->secretBases[VarGet(VAR_0x4054)].sbr_field_1_6 ^= 1;
    FlagSet(0x10C);
}

void sub_80E9C74(void)
{
    CreateTask(sub_8126AD8, 0);
}

void sub_80E9C88(void)
{
    CreateTask(sub_80E9C9C, 0);
}

void sub_80E9C9C(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    ScriptContext2_Enable();
    data[0] = sub_80E9BA8();
    if (data[0] != 0)
    {
        data[1] = 0;
        data[2] = 0;
        sub_8197434(0, 0);
        gUnknown_0203A020 = calloc(1, sizeof(struct SecretBaseListMenuBuffer));
        data[6] = AddWindow(&gUnknown_0858D06C[0]);
        game_continue(taskId);
        sub_80E9E00(taskId);
        gTasks[taskId].func = sub_80E9E90;
    }
    else
    {
        DisplayItemMessageOnField(taskId, gText_NoRegistry, task_pc_turn_off);
    }
}

void game_continue(u8 taskId)
{
    s16 *data;
    u8 i;
    u8 count;

    data = gTasks[taskId].data;
    count = 0;
    for (i = 1; i < 20; i ++)
    {
        if (sub_80E9878(i))
        {
            sub_80E9780(gUnknown_0203A020->names[count], i);
            gUnknown_0203A020->items[count].unk_00 = gUnknown_0203A020->names[count];
            gUnknown_0203A020->items[count].unk_04 = i;
            count ++;
        }
    }
    gUnknown_0203A020->items[count].unk_00 = gText_Cancel;
    gUnknown_0203A020->items[count].unk_04 = -2;
    data[0] = count + 1;
    if (data[0] < 8)
    {
        data[3] = data[0];
    }
    else
    {
        data[3] = 8;
    }
    gUnknown_03006310 = gUnknown_0858D07C;
    gUnknown_03006310.unk_10 = data[6];
    gUnknown_03006310.totalItems = data[0];
    gUnknown_03006310.items = gUnknown_0203A020->items;
    gUnknown_03006310.maxShowed = data[3];
}

void sub_80E9DEC(u32 a0, bool8 flag, struct ListMenu *menu)
{
    if (flag != TRUE)
    {
        PlaySE(SE_SELECT);
    }
}

void sub_80E9E00(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    SetStandardWindowBorderStyle(data[6], 0);
    data[5] = ListMenuInit(&gUnknown_03006310, data[2], data[1]);
    sub_80E9E44(taskId);
    schedule_bg_copy_tilemap_to_vram(0);
}

void sub_80E9E44(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    data[8] = AddScrollIndicatorArrowPairParametrized(0x02, 0xbc, 0x0c, 0x94, data[0] - data[3], 0x13f8, 0x13f8, &data[2]);
}

void sub_80E9E90(u8 taskId)
{
    s16 *data;
    s32 input;

    data = gTasks[taskId].data;
    input = ListMenuHandleInput(data[5]);
    get_coro_args_x18_x1A(data[5], &data[2], &data[1]);
    switch (input)
    {
        case -1:
            break;
        case -2:
            PlaySE(SE_SELECT);
            sub_81AE6C8(data[5], NULL, NULL);
            RemoveScrollIndicatorArrowPair(data[8]);
            sub_819746C(data[6], 0);
            ClearWindowTilemap(data[6]);
            RemoveWindow(data[6]);
            schedule_bg_copy_tilemap_to_vram(0);
            free(gUnknown_0203A020);
            task_pc_turn_off(taskId);
            break;
        default:
            PlaySE(SE_SELECT);
            data[4] = input;
            sub_80E9F20(taskId);
            break;
    }
}

void sub_80E9F20(u8 taskId)
{
    struct WindowTemplate template;
    s16 *data;

    data = gTasks[taskId].data;
    RemoveScrollIndicatorArrowPair(data[8]);
    template = gUnknown_0858D06C[1];
    template.width = GetMaxWidthInMenuTable(gUnknown_0858D048, 2);
    data[7] = AddWindow(&template);
    SetStandardWindowBorderStyle(data[7], 0);
    PrintMenuTable(data[7], 2, gUnknown_0858D048);
    InitMenuInUpperLeftCornerPlaySoundWhenAPressed(data[7], 2, 0);
    schedule_bg_copy_tilemap_to_vram(0);
    gTasks[taskId].func = sub_80E9FB0;
}

void sub_80E9FB0(u8 taskId)
{
    s8 input;

    input = ProcessMenuInputNoWrapAround();
    switch (input)
    {
        case -1:
            PlaySE(SE_SELECT);
            sub_80EA18C(taskId);
            break;
        case -2:
            break;
        default:
            PlaySE(SE_SELECT);
            gUnknown_0858D048[input].func.void_u8(taskId);
            break;
    }
}

void sub_80E9FFC(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    sub_819746C(data[6], FALSE);
    sub_819746C(data[7], FALSE);
    ClearWindowTilemap(data[6]);
    ClearWindowTilemap(data[7]);
    RemoveWindow(data[7]);
    schedule_bg_copy_tilemap_to_vram(0);
    sub_80E9780(gStringVar1, data[4]);
    StringExpandPlaceholders(gStringVar4, gText_OkayToDeleteFromRegistry);
    DisplayItemMessageOnField(taskId, gStringVar4, sub_80EA06C);
}

void sub_80EA06C(u8 taskId)
{
    sub_8197930();
    sub_8121F68(taskId, &gUnknown_0858D058);
}

void sub_80EA08C(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    sub_8197434(0, 0);
    sub_81AE6C8(data[5], &data[2], &data[1]);
    gSaveBlock1Ptr->secretBases[data[4]].sbr_field_1_6 = 0;
    game_continue(taskId);
    sub_812225C(&data[2], &data[1], data[3], data[0]);
    sub_80E9E00(taskId);
    gTasks[taskId].func = sub_80E9E90;
}

void sub_80EA120(u8 taskId)
{
    DisplayItemMessageOnField(taskId, gText_RegisteredDataDeleted, sub_80EA08C);
}

void sub_80EA13C(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    sub_8197434(0, 0);
    sub_81AE6C8(data[5], &data[2], &data[1]);
    sub_80E9E00(taskId);
    gTasks[taskId].func = sub_80E9E90;
}

void sub_80EA18C(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    sub_80E9E44(taskId);
    sub_819746C(data[7], 0);
    ClearWindowTilemap(data[7]);
    RemoveWindow(data[7]);
    schedule_bg_copy_tilemap_to_vram(0);
    gTasks[taskId].func = sub_80E9E90;
}

void task_pc_turn_off(u8 taskId)
{
    if (VarGet(VAR_0x4054) == 0)
    {
        ScriptContext1_SetupScript(gUnknown_0823B4E8);
    }
    else
    {
        ScriptContext1_SetupScript(gUnknown_0823B5E9);
    }
    DestroyTask(taskId);
}

u8 sub_80EA20C(u8 secretBaseRecordId)
{
    return (gSaveBlock1Ptr->secretBases[secretBaseRecordId].trainerId[0] % 5) + (gSaveBlock1Ptr->secretBases[secretBaseRecordId].gender * 5);
}

const u8 *GetSecretBaseTrainerLoseText(void)
{
    u8 param;

    param = sub_80EA20C(VarGet(VAR_0x4054));
    if (param == 0)
    {
        return SecretBase_RedCave1_Text_274966;
    }
    if (param == 1)
    {
        return SecretBase_RedCave1_Text_274D13;
    }
    if (param == 2)
    {
        return SecretBase_RedCave1_Text_274FFE;
    }
    if (param == 3)
    {
        return SecretBase_RedCave1_Text_275367;
    }
    if (param == 4)
    {
        return SecretBase_RedCave1_Text_2756C7;
    }
    if (param == 5)
    {
        return SecretBase_RedCave1_Text_274B24;
    }
    if (param == 6)
    {
        return SecretBase_RedCave1_Text_274E75;
    }
    if (param == 7)
    {
        return SecretBase_RedCave1_Text_2751E1;
    }
    if (param == 8)
    {
        return SecretBase_RedCave1_Text_2754F6;
    }
    return SecretBase_RedCave1_Text_2758CC;
}

void sub_80EA2E4(void)
{
    sub_813BADC(TRUE);
    gTrainerBattleOpponent_A = 0x400;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_SECRET_BASE;
}

void sub_80EA30C(void)
{
    gSaveBlock1Ptr->secretBases[VarGet(VAR_0x4054)].sbr_field_1_5 = gSpecialVar_Result;
}

void sub_80EA354(void)
{
    u16 secretBaseRecordId;
    u8 i;

    secretBaseRecordId = VarGet(VAR_0x4054);
    if (!FlagGet(0x922))
    {
        for (i = 0; i < 20; i ++)
        {
            gSaveBlock1Ptr->secretBases[i].sbr_field_1_5 = FALSE;
        }
        FlagSet(0x922);
    }
    gSpecialVar_0x8004 = sub_80EA20C(secretBaseRecordId);
    gSpecialVar_Result = gSaveBlock1Ptr->secretBases[secretBaseRecordId].sbr_field_1_5;
}


void sub_80EA3E4(u8 taskId)
{
    s16 x;
    s16 y;
    u8 behavior;
    u16 tileId;
    s16 *data;

    data = gTasks[taskId].data;
    switch (data[1])
    {
        case 0:
            if (VarGet(VAR_0x4054) != 0)
            {
                gUnknown_0203A01D = TRUE;
            }
            else
            {
                gUnknown_0203A01D = FALSE;
            }
            PlayerGetDestCoords(&data[2], &data[3]);
            data[1] = 1;
            break;
        case 1:
            PlayerGetDestCoords(&x, &y);
            if (x != data[2] || y != data[3])
            {
                data[2] = x;
                data[3] = y;
                VarSet(VAR_0x40EC, VarGet(VAR_0x40EC) + 1);
                behavior = MapGridGetMetatileBehaviorAt(x, y);
                tileId = MapGridGetMetatileIdAt(x, y);
                if (tileId == 0x234 || tileId == 0x23C)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x20);
                    }
                }
                else if (tileId == 0x2b8 || tileId == 0x2b9 || tileId == 0x2ba || tileId == 0x2c0 || tileId == 0x2c1 || tileId == 0x2c2 || tileId == 0x2c8 || tileId == 0x2c9 || tileId == 0x2ca)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x01);
                    }
                }
                else if (tileId == 0x239 || tileId == 0x241 || tileId == 0x251 || tileId == 0x259)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x04);
                    }
                }
                else if ((behavior == 0x34 && tileId == 0x26d) || (behavior == 0x35 && MapGridGetMetatileIdAt(x, y) == 0x26a))
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x200);
                    }
                }
                else if (behavior == 0xc1 && tileId == 0x23d)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) ^ 0x1000);
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x2000);
                    }
                }
                else if (behavior == 0x47 && tileId == 0x23e)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x1000);
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) ^ 0x2000);
                    }
                }
                else if (MetatileBehavior_IsSecretBaseGlitterMat(behavior) == TRUE)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x80);
                    }
                }
                else if (MetatileBehavior_IsSecretBaseBalloon(behavior) == TRUE)
                {
                    sub_80FA5E4(MapGridGetMetatileIdAt(x, y), x, y);
                    if (gUnknown_0203A01D == TRUE)
                    {
                        switch ((int)MapGridGetMetatileIdAt(x, y))
                        {
                            case 0x338:
                            case 0x33c:
                            case 0x340:
                                VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x02);
                                break;
                            case 0x228:
                                VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x100);
                                break;
                        }
                    }
                }
                else if (MetatileBehavior_IsMB_BE(behavior) == TRUE)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x400);
                    }
                    sub_80FA794(x, y);
                }
                else if (MetatileBehavior_IsSecretBaseSoundMat(behavior) == TRUE){
                    if (gUnknown_0203A01D == TRUE) {
                        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x8000);
                    }
                }
                else if (MetatileBehavior_IsSecretBaseJumpMat(behavior) == TRUE)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x4000);
                    }
                }
                else if (MetatileBehavior_IsSecretBaseSpinMat(behavior) == TRUE)
                {
                    if (gUnknown_0203A01D == TRUE)
                    {
                        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x02);
                    }
                }
            }
            break;
        case 2:
            if (!FieldEffectActiveListContains(data[4]))
            {
                data[1] = 1;
            }
            break;
    }
}

void sub_80EA828(u8 secretBaseRecordId, struct SecretBaseRecord *base, u32 version, u32 language)
{
    int stringLength;
    u8 *name;

    gSaveBlock1Ptr->secretBases[secretBaseRecordId] = *base;
    gSaveBlock1Ptr->secretBases[secretBaseRecordId].sbr_field_1_6 = 2;
    if (version == VERSION_SAPPHIRE || version == VERSION_RUBY)
    {
        gSaveBlock1Ptr->secretBases[secretBaseRecordId].language = LANGUAGE_ENGLISH;
    }
    if (version == VERSION_EMERALD && language == LANGUAGE_JAPANESE)
    {
        name = gSaveBlock1Ptr->secretBases[secretBaseRecordId].trainerName;
        for (stringLength = 0; stringLength < 7; stringLength ++)
        {
            if (name[stringLength] == EOS)
            {
                break;
            }
        }
        if (stringLength > 5)
        {
            gSaveBlock1Ptr->secretBases[secretBaseRecordId].language = LANGUAGE_ENGLISH;
        }
    }
}

bool8 sub_80EA8D4(struct SecretBaseRecord *sbr1, struct SecretBaseRecord *sbr2)
{
    u8 i;
    for (i = 0; i < 4; i ++)
    {
        if (sbr1->trainerId[i] != sbr2->trainerId[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

bool8 sub_80EA904(struct SecretBaseRecord *sbr1, struct SecretBaseRecord *sbr2)
{
    u8 i;

    for (i = 0; i < OT_NAME_LENGTH && (sbr1->trainerName[i] != EOS || sbr2->trainerName[i] != EOS); i++)
    {
        if (sbr1->trainerName[i] != sbr2->trainerName[i])
        {
            return FALSE;
        }
    }
    return TRUE;
}

bool8 sub_80EA950(struct SecretBaseRecord *sbr1, struct SecretBaseRecord *sbr2)
{
    if (sbr1->gender == sbr2->gender && sub_80EA8D4(sbr1, sbr2) && sub_80EA904(sbr1, sbr2))
    {
        return TRUE;
    }
    return FALSE;
}

s16 sub_80EA990(u8 secretBaseRecordId)
{
    s16 i;

    for (i = 0; i < 20; i ++)
    {
        if (gSaveBlock1Ptr->secretBases[i].secretBaseId == secretBaseRecordId)
        {
            return i;
        }
    }
    return -1;
}

u8 sub_80EA9D8(void)
{
    s16 i;

    for (i = 1; i < 20; i ++)
    {
        if (gSaveBlock1Ptr->secretBases[i].secretBaseId == 0)
        {
            return i;
        }
    }
    return 0;
}

u8 sub_80EAA18(void)
{
    s16 i;

    for (i = 1; i < 20; i ++)
    {
        if (gSaveBlock1Ptr->secretBases[i].sbr_field_1_6 == 0 && gSaveBlock1Ptr->secretBases[i].sbr_field_1_0 == 0)
        {
            return i;
        }
    }
    return 0;
}

u8 sub_80EAA64(struct SecretBaseRecord *base, u32 version, u32 language)
{
    s16 secretBaseRecordId;

    if (base->secretBaseId == 0)
    {
        return 0;
    }
    secretBaseRecordId = sub_80EA990(base->secretBaseId);
    if (secretBaseRecordId != 0)
    {
        if (secretBaseRecordId != -1)
        {
            if (gSaveBlock1Ptr->secretBases[secretBaseRecordId].sbr_field_1_0 == 1)
            {
                return 0;
            }
            if (gSaveBlock1Ptr->secretBases[secretBaseRecordId].sbr_field_1_6 != 2 || base->sbr_field_1_0 == 1)
            {
                sub_80EA828(secretBaseRecordId, base, version, language);
                return secretBaseRecordId;
            }
        }
        else
        {
            secretBaseRecordId = sub_80EA9D8();
            if (secretBaseRecordId != 0)
            {
                sub_80EA828(secretBaseRecordId, base, version, language);
                return secretBaseRecordId;
            }
            secretBaseRecordId = sub_80EAA18();
            if (secretBaseRecordId != 0)
            {
                sub_80EA828(secretBaseRecordId, base, version, language);
                return secretBaseRecordId;
            }
        }
    }
    return 0;
}

void sub_80EAAF4(void)
{
    u8 i;
    u8 j;
    struct SecretBaseRecord base;
    struct SecretBaseRecord *secretBases;

    secretBases = gSaveBlock1Ptr->secretBases;
    for (i = 1; i < 19; i ++)
    {
        for (j = i + 1; j < 20; j ++)
        {
            if ((secretBases[i].sbr_field_1_6 == 0 && secretBases[j].sbr_field_1_6 == 1) || (secretBases[i].sbr_field_1_6 == 2 && secretBases[j].sbr_field_1_6 != 2))
            {
                base = secretBases[i];
                secretBases[i] = secretBases[j];
                secretBases[j] = base;
            }
        }
    }
}

void sub_80EABA4(struct SecretBaseRecordMixer *mixer, u8 b)
{
    u16 i;

    for (i = 1; i < 20; i ++)
    {
        if (mixer->records[i].sbr_field_1_6 == b)
        {
            sub_80EAA64(&mixer->records[i], mixer->version, mixer->language);
        }
    }
}

bool8 DoesSecretBaseBelongToPlayer(struct SecretBaseRecord *secretBase)
{
    u8 i;

    if (secretBase->secretBaseId == 0)
        return FALSE;

    if (secretBase->secretBaseId && secretBase->gender != gSaveBlock2Ptr->playerGender)
        return FALSE;

    // Check if the player's trainer Id matches the secret base's id.
    for (i = 0; i < 4; i ++)
    {
        if (secretBase->trainerId[i] != gSaveBlock2Ptr->playerTrainerId[i])
            return FALSE;
    }

    for (i = 0; i < OT_NAME_LENGTH && (secretBase->trainerName[i] != EOS || gSaveBlock2Ptr->playerName[i] != EOS); i ++)
    {
        if (secretBase->trainerName[i] != gSaveBlock2Ptr->playerName[i])
            return FALSE;
    }

    return TRUE;
}

void DeleteFirstOldBaseFromPlayerInRecordMixingFriendsRecords(struct SecretBaseRecord *basesA, struct SecretBaseRecord *basesB, struct SecretBaseRecord *basesC)
{
    u8 i;
    u8 sbFlags = 0x0;

    for (i = 0; i < 20; i ++)
    {
        if (!(sbFlags & 0x1)) // 001
        {
            if (DoesSecretBaseBelongToPlayer(&basesA[i]) == TRUE)
            {
                ClearSecretBase(&basesA[i]);
                sbFlags |= 1;
            }
        }

        if (!(sbFlags & 0x2)) // 010
        {
            if (DoesSecretBaseBelongToPlayer(&basesB[i]) == TRUE)
            {
                ClearSecretBase(&basesB[i]);
                sbFlags |= 2;
            }
        }

        if (!(sbFlags & 0x4)) // 100
        {
            if (DoesSecretBaseBelongToPlayer(&basesC[i]) == TRUE)
            {
                ClearSecretBase(&basesC[i]);
                sbFlags |= 4;
            }
        }

        if (sbFlags == 0x7) // 111
        {
            break;
        }
    }
}

bool8 sub_80EAD14(struct SecretBaseRecord *base, struct SecretBaseRecord *secretBases, u8 c)
{
    u8 i;

    for (i = 0; i < 20; i ++)
    {
        if (secretBases[i].secretBaseId != 0)
        {
            if (sub_80EA950(base, &secretBases[i]) == TRUE)
            {
                if (c == 0)
                {
                    ClearSecretBase(&secretBases[i]);
                    return FALSE;
                }

                if (base->sbr_field_e > secretBases[i].sbr_field_e)
                {
                    ClearSecretBase(&secretBases[i]);
                    return FALSE;
                }

                secretBases[i].sbr_field_1_0 = base->sbr_field_1_0;

                ClearSecretBase(base);
                return TRUE;
            }
        }
    }

    return FALSE;
}

void sub_80EAD94(struct SecretBaseRecord *basesA, struct SecretBaseRecord *basesB, struct SecretBaseRecord *basesC, struct SecretBaseRecord *basesD)
{
    u8 i;

    for (i = 1; i < 20; i ++)
    {
        if (basesA[i].secretBaseId)
        {
            if (basesA[i].sbr_field_1_6 == 1)
            {
                basesA[i].sbr_field_1_0 = 1;
            }
            if (!sub_80EAD14(&basesA[i], basesB, i))
            {
                if (!sub_80EAD14(&basesA[i], basesC, i))
                {
                    sub_80EAD14(&basesA[i], basesD, i);
                }
            }
        }
    }
    for (i = 0; i < 20; i ++)
    {
        if (basesB[i].secretBaseId)
        {
            basesB[i].sbr_field_1_5 = 0;
            if (!sub_80EAD14(&basesB[i], basesC, i))
            {
                sub_80EAD14(&basesB[i], basesD, i);
            }
        }
    }
    for (i = 0; i < 20; i ++)
    {
        if (basesC[i].secretBaseId)
        {
            basesC[i].sbr_field_1_5 = 0;
            sub_80EAD14(&basesC[i], basesD, i);
        }
        if (basesD[i].secretBaseId)
        {
            basesD[i].sbr_field_1_5 = 0;
        }
    }
}

void sub_80EAE90(struct SecretBaseRecord *base, u32 version, u32 language)
{
    if (base->sbr_field_1_0 == 1)
    {
        sub_80EAA64(base, version, language);
        ClearSecretBase(base);
    }
}

void sub_80EAEB4(struct SecretBaseRecordMixer *mixers)
{
    u16 i;

    for (i = 0; i < 20; i ++)
    {
        sub_80EAE90(&mixers[0].records[i], mixers[0].version, mixers[0].language);
        sub_80EAE90(&mixers[1].records[i], mixers[1].version, mixers[1].language);
        sub_80EAE90(&mixers[2].records[i], mixers[2].version, mixers[2].language);
    }
}

void sub_80EAEF4(struct SecretBaseRecordMixer *mixers)
{
    DeleteFirstOldBaseFromPlayerInRecordMixingFriendsRecords(mixers[0].records, mixers[1].records, mixers[2].records);
    sub_80EAD94(gSaveBlock1Ptr->secretBases, mixers[0].records, mixers[1].records, mixers[2].records);
    sub_80EAEB4(mixers);
    sub_80EAA64(mixers[0].records, mixers[0].version, mixers[0].language);
    sub_80EAA64(mixers[1].records, mixers[1].version, mixers[1].language);
    sub_80EAA64(mixers[2].records, mixers[2].version, mixers[2].language);
    sub_80EABA4(&mixers[0], 1);
    sub_80EABA4(&mixers[1], 1);
    sub_80EABA4(&mixers[2], 1);
    sub_80EABA4(&mixers[0], 0);
    sub_80EABA4(&mixers[1], 0);
    sub_80EABA4(&mixers[2], 0);
}

void sub_80EAF80(void *records, size_t recordSize, u8 linkIdx)
{
    struct SecretBaseRecordMixer mixers[3];
    u16 i;

    if (FlagGet(0x60))
    {
        switch (GetLinkPlayerCount())
        {
            case 2:
                memset(records + 2 * recordSize, 0, recordSize);
                memset(records + 3 * recordSize, 0, recordSize);
                break;
            case 3:
                memset(records + 3 * recordSize, 0, recordSize);
                break;
        }
        switch (linkIdx)
        {
            case 0:
                mixers[0].records = records + 1 * recordSize;
                mixers[0].version = gLinkPlayers[1].version & 0xFF;
                mixers[0].language = gLinkPlayers[1].language;
                mixers[1].records = records + 2 * recordSize;
                mixers[1].version = gLinkPlayers[2].version & 0xFF;
                mixers[1].language = gLinkPlayers[2].language;
                mixers[2].records = records + 3 * recordSize;
                mixers[2].version = gLinkPlayers[3].version & 0xFF;
                mixers[2].language = gLinkPlayers[3].language;
                break;
            case 1:
                mixers[0].records = records + 2 * recordSize;
                mixers[0].version = gLinkPlayers[2].version & 0xFF;
                mixers[0].language = gLinkPlayers[2].language;
                mixers[1].records = records + 3 * recordSize;
                mixers[1].version = gLinkPlayers[3].version & 0xFF;
                mixers[1].language = gLinkPlayers[3].language;
                mixers[2].records = records + 0 * recordSize;
                mixers[2].version = gLinkPlayers[0].version & 0xFF;
                mixers[2].language = gLinkPlayers[0].language;
                break;
            case 2:
                mixers[0].records = records + 3 * recordSize;
                mixers[0].version = gLinkPlayers[3].version & 0xFF;
                mixers[0].language = gLinkPlayers[3].language;
                mixers[1].records = records + 0 * recordSize;
                mixers[1].version = gLinkPlayers[0].version & 0xFF;
                mixers[1].language = gLinkPlayers[0].language;
                mixers[2].records = records + 1 * recordSize;
                mixers[2].version = gLinkPlayers[1].version & 0xFF;
                mixers[2].language = gLinkPlayers[1].language;
                break;
            case 3:
                mixers[0].records = records + 0 * recordSize;
                mixers[0].version = gLinkPlayers[0].version & 0xFF;
                mixers[0].language = gLinkPlayers[0].language;
                mixers[1].records = records + 1 * recordSize;
                mixers[1].version = gLinkPlayers[1].version & 0xFF;
                mixers[1].language = gLinkPlayers[1].language;
                mixers[2].records = records + 2 * recordSize;
                mixers[2].version = gLinkPlayers[2].version & 0xFF;
                mixers[2].language = gLinkPlayers[2].language;
                break;
        }
        sub_80EAEF4(mixers);
        for (i = 1; i < 20; i ++)
        {
            if (gSaveBlock1Ptr->secretBases[i].sbr_field_1_0 == 1)
            {
                gSaveBlock1Ptr->secretBases[i].sbr_field_1_6 = 1;
                gSaveBlock1Ptr->secretBases[i].sbr_field_1_0 = 0;
            }
        }
        sub_80EAAF4();
        for (i = 1; i < 20; i ++)
        {
            if (gSaveBlock1Ptr->secretBases[i].sbr_field_1_6 == 2)
            {
                gSaveBlock1Ptr->secretBases[i].sbr_field_1_6 = 0;
            }
        }
        if (gSaveBlock1Ptr->secretBases[0].secretBaseId != 0 && gSaveBlock1Ptr->secretBases[0].sbr_field_e != 0xFFFF)
        {
            gSaveBlock1Ptr->secretBases[0].sbr_field_e ++;
        }
    }
}

void sub_80EB18C(struct SecretBaseRecord *bases)
{
    u32 i;

    for (i = 0; i < 20; i ++)
    {
        if (bases[i].language == LANGUAGE_JAPANESE)
        {
            ClearSecretBase(&bases[i]);
        }
    }
}

void sub_80EB1AC(void)
{
    VarSet(VAR_0x40EC, 0);
    VarSet(VAR_0x40ED, 0);
    VarSet(VAR_0x40EE, 0);
    VarSet(VAR_0x40EF, 0);
    if (VarGet(VAR_0x4054) != 0)
    {
        VarSet(VAR_0x40F0, TRUE);
    }
    else
    {
        VarSet(VAR_0x40F0, FALSE);
    }
    gUnknown_0203A01D = FALSE;
}

void sub_80EB218(void)
{
    if (VarGet(VAR_0x40F0) && gUnknown_0203A01D == TRUE && !CurrentMapIsSecretBase())
    {
        VarSet(VAR_0x40F0, FALSE);
        gUnknown_0203A01D = FALSE;
        sub_80EEA70();
        VarSet(VAR_0x40EC, 0);
        VarSet(VAR_0x40ED, 0);
        VarSet(VAR_0x40EE, 0);
        VarSet(VAR_0x40EF, 0);
        VarSet(VAR_0x40F0, FALSE);
    }
}

void sub_80EB290(void)
{
    if (VarGet(VAR_0x4054) != 0)
    {
        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x800);
    }
}

void sub_80EB2C8(void)
{
    if (VarGet(VAR_0x4054) != 0)
    {
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x400);
    }
}

void sub_80EB300(void)
{
    if (VarGet(VAR_0x4054) != 0)
    {
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) & ~0x3800);
        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) & ~0x001);
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x2000);
    }
}

void sub_80EB368(void)
{
    if (VarGet(VAR_0x4054) != 0)
    {
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) & ~0x3800);
        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) & ~0x001);
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x800);
    }
}

void sub_80EB3D0(void)
{
    if (VarGet(VAR_0x4054) != 0)
    {
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) & ~0x3800);
        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) & ~0x001);
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x1000);
    }
}

void sub_80EB438(void)
{
    if (VarGet(VAR_0x4054) != 0)
    {
        VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) & ~0x3800);
        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) & ~0x001);
        VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x001);
    }
}

void sub_80EB498(void)
{
    s16 x;
    s16 y;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    switch (MapGridGetMetatileIdAt(x, y))
    {
        case 0x31C:
        case 0x31D:
        case 0x31E:
        case 0x31F:
        case 0x324:
        case 0x325:
        case 0x326:
        case 0x327:
        case 0x32C:
        case 0x32D:
        case 0x330:
        case 0x331:
        case 0x332:
        case 0x333:
        case 0x334:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x4000);
            }
            break;
    }
}

void sub_80EB56C(void)
{
    s16 x;
    s16 y;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    switch (MapGridGetMetatileIdAt(x, y))
    {
        case 0x28a:
        case 0x28b:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x40);
            }
            break;
        case 0x2d8:
        case 0x2d9:
        case 0x2da:
        case 0x2db:
        case 0x2dc:
        case 0x2dd:
        case 0x2e8:
        case 0x2e9:
        case 0x2ea:
        case 0x2eb:
        case 0x2ec:
        case 0x2ed:
        case 0x2ee:
        case 0x2ef:
        case 0x2f8:
        case 0x2f9:
        case 0x2fa:
        case 0x2fb:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EE, VarGet(VAR_0x40EE) | 0x8);
            }
            break;
        case 0x22c:
        case 0x233:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x40);
            }
            break;
        case 0x288:
        case 0x289:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x100);
            }
            break;
        case 0x22d:
        case 0x22e:
        case 0x22f:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x10);
            }
            break;
        case 0x287:
        case 0x28f:
        case 0x298:
        case 0x299:
        case 0x29a:
        case 0x29b:
        case 0x29c:
        case 0x29d:
        case 0x29e:
        case 0x29f:
        case 0x2ab:
        case 0x2b0:
        case 0x2b1:
        case 0x2b2:
        case 0x2b4:
        case 0x2b5:
        case 0x2b6:
        case 0x2b7:
        case 0x2cb:
        case 0x2cc:
        case 0x2cd:
        case 0x2ce:
        case 0x2cf:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x8);
            }
            break;
    }
}

void sub_80EB9E0(void)
{
    s16 x;
    s16 y;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    switch (MapGridGetMetatileIdAt(x, y))
    {
        case 0x291:
        case 0x294:
        case 0x297:
        case 0x2a1:
        case 0x2a5:
        case 0x2a9:
        case 0x2ad:
        case 0x2bb:
        case 0x2be:
        case 0x2c3:
        case 0x2c6:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x8);
            }
            break;
    }
}

void sub_80EBB28(void)
{
    s16 x;
    s16 y;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    switch (MapGridGetMetatileIdAt(x, y))
    {
        case 0x290:
        case 0x292:
        case 0x293:
        case 0x295:
        case 0x296:
        case 0x2a0:
        case 0x2a2:
        case 0x2a3:
        case 0x2a4:
        case 0x2a6:
        case 0x2a7:
        case 0x2a8:
        case 0x2aa:
        case 0x2ac:
        case 0x2ae:
        case 0x2af:
        case 0x2bc:
        case 0x2bd:
        case 0x2bf:
        case 0x2c4:
        case 0x2c5:
        case 0x2c7:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x8);
            }
            break;
        case 0x280:
        case 0x281:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x100);
            }
            break;
        case 0x225:
        case 0x226:
        case 0x227:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x10);
            }
            break;
    }
}

void sub_80EBE7C(void)
{
    s16 x;
    s16 y;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    switch ((int)MapGridGetMetatileIdAt(x, y))
    {
        case 0x28d:
        case 0x28e:
            if (VarGet(VAR_0x4054) != 0)
            {
                VarSet(VAR_0x40EF, VarGet(VAR_0x40EF) | 0x4);
            }
            break;
    }
}
