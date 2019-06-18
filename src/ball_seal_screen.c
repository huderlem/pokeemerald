#include "global.h"
#include "malloc.h"
#include "ball_seal.h"
#include "ball_seal_screen.h"
#include "battle.h"
#include "bg.h"
#include "battle_anim.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "list_menu.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "trainer_pokemon_sprites.h"
#include "random.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "trig.h"
#include "util.h"
#include "window.h"
#include "constants/ball_seals.h"
#include "constants/rgb.h"
#include "constants/songs.h"

enum
{
    BALL_SEAL_SCREEN_STATE_INIT,
    BALL_SEAL_SCREEN_STATE_SELECTING_SLOT,
    BALL_SEAL_SCREEN_STATE_SELECTING_SEAL,
};

struct BallSealScreenData
{
    void (*returnCallback)(void);
    struct ListMenuItem *slotItems;
    u8 slotItemsListMenuTaskId;
    u16 slotScrollOffset;
    u16 slotSelectedItem;
    struct ListMenuItem *sealItems;
    u8 sealItemsListMenuTaskId;
    u16 sealScrollOffset;
    u16 sealSelectedItem;
    u8 scrollArrowsTaskId;
    s8 partyIndex;
    u8 monSpriteId;
    u16 monPicTileStart;
    u16 monPicPaletteNum;
    int numSealListItems;
    u8 ballSealAnimationTaskId;
    u8 state;
};

static EWRAM_DATA struct BallSealScreenData *sBallSealScreenData = NULL;

static void InitBallSealScreen(void);
static void HandleSelectSlotInput(u8 taskId);
static void HandleSelectSealInput(u8 taskId);
static void DrawBallSealScreenText(void);
static void BallSealScreenMain(u8 taskId);
static void ExitBallSealScreen(u8 taskId);
static void ExitBallSealScreen_WaitFade(u8 taskId);
static void LoadSlotListItems(void);
static int LoadSealListItems();
static void DestroySlotAndSealListMenus(void);
static void DrawSealListMenu(bool32 isActive, int numItems);
static void DrawSlotListMenu(bool32 selected);
static void DrawMonBackSprite(void);
static void RunBallSealAnimation(u8 taskId);
static void ChangeMonBallSeal(int sealIndex, int slot);
static void DrawTitle(void);

#define BG_TEXT 0
#define BG_MENU 1

#define WIN_SLOTS 0
#define WIN_SEALS 1
#define WIN_TITLE 2

#define TAG_MON_BACKPIC 2000
#define TAG_ARROWS      2001

#define MAX_SHOWN_ITEMS 9

static const struct BgTemplate sBallSealScreenBgTemplates[] = {
    {
       .bg = BG_TEXT,
       .charBaseIndex = 0,
       .mapBaseIndex = 28,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0,
   },
   {
       .bg = BG_MENU,
       .charBaseIndex = 1,
       .mapBaseIndex = 29,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0,
   },
};

static const struct WindowTemplate sBallSealScreenWinTemplates[] = {
    {
        .bg = BG_TEXT,
        .tilemapLeft = 1,
        .tilemapTop = 11,
        .width = 13,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 0x1,
    },
    {
        .bg = BG_TEXT,
        .tilemapLeft = 16,
        .tilemapTop = 1,
        .width = 13,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 0x69,
    },
    {
        .bg = BG_TEXT,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 15,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x153,
    },
    DUMMY_WIN_TEMPLATE,
};


static const struct ListMenuTemplate sSlotsListMenuTemplate =
{
    .items = NULL,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = 4,
    .maxShowed = 4,
    .windowId = WIN_SLOTS,
    .header_X = 0,
    .item_X = 0,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 0,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = 1,
    .cursorKind = 0,
    .itemAlign = LIST_ITEM_ALIGN_CENTER,
    .highlightSelection = 0,
};

static const struct ListMenuTemplate sSealCaseListMenuTemplate =
{
    .items = NULL,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = 0,
    .maxShowed = MAX_SHOWN_ITEMS,
    .windowId = WIN_SEALS,
    .header_X = 0,
    .item_X = 0,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 0,
    .cursorShadowPal = 3,
    .lettersSpacing = 1,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = 1,
    .cursorKind = 0,
    .itemAlign = LIST_ITEM_ALIGN_CENTER,
    .highlightSelection = 0,
};

static const u8 sBallSealScreen_Gfx[] = INCBIN_U8("graphics/ball_seal_screen/ball_seal_screen.4bpp.lz");
static const u8 sBallSealScreen_Pal[] = INCBIN_U8("graphics/ball_seal_screen/ball_seal_screen.gbapal");
static const u8 sBallSealScreen_Tilemap[] = INCBIN_U8("graphics/ball_seal_screen/ball_seal_screen.bin.lz");

static const u8 sScreenTitle[] = _("BALL SEALS");

static const u8 sBallSealNames[][13] =
{
    [BALL_SEAL_NONE]                   = _("- NONE -"),
    [BALL_SEAL_HEARTCLOUD_SMALL_PINK]  = _("HEART A"),
    [BALL_SEAL_HEARTSPIN_SMALL_PINK]   = _("HEART B"),
    [BALL_SEAL_HEARTCIRCLE_SMALL_PINK] = _("HEART C"),
    [BALL_SEAL_LETTER_A]               = _("GLYPH A"),
    [BALL_SEAL_ORBS_RAINBOW]           = _("ORBS RAINBOW"),
    [BALL_SEAL_6]                      = _("SEAL 6"),
    [BALL_SEAL_7]                      = _("SEAL 7"),
    [BALL_SEAL_8]                      = _("SEAL 8"),
    [BALL_SEAL_9]                      = _("SEAL 9"),
    [BALL_SEAL_A]                      = _("SEAL A"),
    [BALL_SEAL_B]                      = _("SEAL B"),
    [BALL_SEAL_C]                      = _("SEAL C"),
    [BALL_SEAL_D]                      = _("SEAL D"),
    [BALL_SEAL_E]                      = _("SEAL E"),
    [BALL_SEAL_F]                      = _("SEAL F"),
    [BALL_SEAL_10]                     = _("SEAL 10"),
    [BALL_SEAL_11]                     = _("SEAL 11"),
    [BALL_SEAL_12]                     = _("SEAL 12"),
    [BALL_SEAL_13]                     = _("SEAL 13"),
    [BALL_SEAL_14]                     = _("SEAL 14"),
    [BALL_SEAL_15]                     = _("SEAL 15"),
};

static void MainCallback_BallSealScreen(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    RunTextPrinters();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlank_BallSealScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void OpenBallSealScreen(void (*returnCallback)(void))
{
    sBallSealScreenData = Alloc(sizeof(*sBallSealScreenData));
    sBallSealScreenData->returnCallback = returnCallback;
    sBallSealScreenData->slotItemsListMenuTaskId = 0xFF;
    sBallSealScreenData->slotScrollOffset = 0;
    sBallSealScreenData->slotSelectedItem = 0;
    sBallSealScreenData->sealItemsListMenuTaskId = 0xFF;
    sBallSealScreenData->scrollArrowsTaskId = 0xFF;
    sBallSealScreenData->sealScrollOffset = 0;
    sBallSealScreenData->sealSelectedItem = 0;
    sBallSealScreenData->partyIndex = 0;
    sBallSealScreenData->monSpriteId = MAX_SPRITES;
    sBallSealScreenData->monPicTileStart = 0;
    sBallSealScreenData->monPicPaletteNum = 0;
    sBallSealScreenData->ballSealAnimationTaskId = 0;
    sBallSealScreenData->state = BALL_SEAL_SCREEN_STATE_INIT;

    SetMainCallback2(InitBallSealScreen);
}

static void InitBallSealScreen(void)
{
    SetVBlankCallback(NULL);
    ResetAllBgsCoordinates();
    ResetVramOamAndBgCntRegs();
    ResetBgsAndClearDma3BusyFlags(0);
    ScanlineEffect_Stop();

    InitBgsFromTemplates(0, sBallSealScreenBgTemplates, ARRAY_COUNT(sBallSealScreenBgTemplates));
    SetBgTilemapBuffer(BG_MENU, AllocZeroed(BG_SCREEN_SIZE));
    InitWindows(sBallSealScreenWinTemplates);
    DecompressAndLoadBgGfxUsingHeap(BG_MENU, sBallSealScreen_Gfx, 0x260, 0, 0);
    CopyToBgTilemapBuffer(BG_MENU, sBallSealScreen_Tilemap, 0, 0);
    ResetPaletteFade();
    LoadPalette(sBallSealScreen_Pal, 0, sizeof(sBallSealScreen_Pal));
    DeactivateAllTextPrinters();

    ResetSpriteData();
    FreeAllSpritePalettes();

    CopyBgTilemapBufferToVram(BG_MENU);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    ShowBg(BG_TEXT);
    ShowBg(BG_MENU);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);

    DrawTitle();
    DrawMonBackSprite();
    sBallSealScreenData->numSealListItems = LoadSealListItems();
    DrawSealListMenu(FALSE, sBallSealScreenData->numSealListItems);
    DrawSlotListMenu(FALSE);

    ScheduleBgCopyTilemapToVram(BG_TEXT);

    SetVBlankCallback(VBlank_BallSealScreen);
    SetMainCallback2(MainCallback_BallSealScreen);
    CreateTask(BallSealScreenMain, 0);
}

static void BallSealScreenMain(u8 taskId)
{
    int i;
    struct Pokemon *mon;

    switch (sBallSealScreenData->state)
    {
    case BALL_SEAL_SCREEN_STATE_INIT:
        if (!gPaletteFade.active)
        {
            sBallSealScreenData->state = BALL_SEAL_SCREEN_STATE_SELECTING_SLOT;
            sBallSealScreenData->ballSealAnimationTaskId = CreateTask(RunBallSealAnimation, 3);
        }
        break;
    case BALL_SEAL_SCREEN_STATE_SELECTING_SLOT:
        HandleSelectSlotInput(taskId);
        break;
    case BALL_SEAL_SCREEN_STATE_SELECTING_SEAL:
        HandleSelectSealInput(taskId);
        break;
    }
}

static void HandleSelectSlotInput(u8 taskId)
{
    int input;
    int i;
    struct Pokemon *mon;

    input = ListMenu_ProcessInput(sBallSealScreenData->slotItemsListMenuTaskId);
    ListMenuGetScrollAndRow(sBallSealScreenData->slotItemsListMenuTaskId, &sBallSealScreenData->slotScrollOffset, &sBallSealScreenData->slotSelectedItem);
    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        ExitBallSealScreen(taskId);
        return;
    default:
        PlaySE(SE_SELECT);
        DestroySlotAndSealListMenus();
        DrawSlotListMenu(TRUE);
        DrawSealListMenu(TRUE, sBallSealScreenData->numSealListItems);
        sBallSealScreenData->state = BALL_SEAL_SCREEN_STATE_SELECTING_SEAL;
        return;
    }

    if (gMain.newAndRepeatedKeys & DPAD_LEFT)
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (--sBallSealScreenData->partyIndex < 0)
                sBallSealScreenData->partyIndex = gPlayerPartyCount - 1;

            mon = &gPlayerParty[sBallSealScreenData->partyIndex];
            if (GetMonData(mon, MON_DATA_SPECIES) && !GetMonData(mon, MON_DATA_IS_EGG))
                break;
        }

        PlaySE(SE_SELECT);
        DrawMonBackSprite();
        DrawSlotListMenu(FALSE);
    }
    else if (gMain.newAndRepeatedKeys & DPAD_RIGHT)
    {
        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (++sBallSealScreenData->partyIndex >= gPlayerPartyCount)
                sBallSealScreenData->partyIndex = 0;

            mon = &gPlayerParty[sBallSealScreenData->partyIndex];
            if (GetMonData(mon, MON_DATA_SPECIES) && !GetMonData(mon, MON_DATA_IS_EGG))
                break;
        }

        PlaySE(SE_SELECT);
        DrawMonBackSprite();
        DrawSlotListMenu(FALSE);
    }
}

static void HandleSelectSealInput(u8 taskId)
{
    int input;
    int i;
    struct Pokemon *mon;

    input = ListMenu_ProcessInput(sBallSealScreenData->sealItemsListMenuTaskId);
    ListMenuGetScrollAndRow(sBallSealScreenData->sealItemsListMenuTaskId, &sBallSealScreenData->sealScrollOffset, &sBallSealScreenData->sealSelectedItem);
    switch (input)
    {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        PlaySE(SE_SELECT);
        DestroySlotAndSealListMenus();
        DrawSlotListMenu(FALSE);
        DrawSealListMenu(FALSE, sBallSealScreenData->numSealListItems);
        sBallSealScreenData->state = BALL_SEAL_SCREEN_STATE_SELECTING_SLOT;
        return;
    default:
        PlaySE(SE_SELECT);
        ChangeMonBallSeal(
            sBallSealScreenData->sealScrollOffset + sBallSealScreenData->sealSelectedItem,
            sBallSealScreenData->slotScrollOffset + sBallSealScreenData->slotSelectedItem);
        DestroySlotAndSealListMenus();
        DrawSlotListMenu(FALSE);
        DrawSealListMenu(FALSE, sBallSealScreenData->numSealListItems);
        sBallSealScreenData->state = BALL_SEAL_SCREEN_STATE_SELECTING_SLOT;
        return;
    }
}

static void ChangeMonBallSeal(int sealIndex, int slot)
{
    int ballSeal = sBallSealScreenData->sealItems[sealIndex].id;
    u8 sealDataIds[] = {
        MON_DATA_BALL_SEAL_1,
        MON_DATA_BALL_SEAL_2,
        MON_DATA_BALL_SEAL_3,
        MON_DATA_BALL_SEAL_4,
    };

    SetMonData(&gPlayerParty[sBallSealScreenData->partyIndex], sealDataIds[slot], &ballSeal);
}

static void DrawTitle(void)
{
    u8 colors[3];
    colors[0] = 0;
    colors[1] = 2;
    colors[2] = 3;
    AddTextPrinterParameterized3(WIN_TITLE, 1, GetStringCenterAlignXOffset(1, sScreenTitle, 120), 0, colors, TEXT_SKIP_DRAW, sScreenTitle);
    PutWindowTilemap(WIN_TITLE);
    CopyWindowToVram(WIN_TITLE, 3);
}

u8 GetNumOwnedSeals(void)
{
    int i;
    int byteOffset = 0;
    int bitOffset = 0;
    int numOwned = 0;

    for (i = 0; i < NUM_BALL_SEALS; i++)
    {
        if (gSaveBlock1Ptr->ownedBallSeals[byteOffset] & (1 << bitOffset))
            numOwned++;

        if (++bitOffset == 8)
        {
            bitOffset = 0;
            byteOffset++;
        }
    }

    return numOwned;
}

static void LoadSlotListItems(void)
{
    int i;
    int menuItemY;
    struct Pokemon *mon = &gPlayerParty[sBallSealScreenData->partyIndex];
    u8 sealDataIds[] = {
        MON_DATA_BALL_SEAL_1,
        MON_DATA_BALL_SEAL_2,
        MON_DATA_BALL_SEAL_3,
        MON_DATA_BALL_SEAL_4,
    };

    if (sBallSealScreenData->slotItems)
        Free(sBallSealScreenData->slotItems);

    sBallSealScreenData->slotItems = Alloc(4 * sizeof(struct ListMenuItem));
    for (i = 0; i < 4; i++)
    {
        u32 ballSeal = GetMonData(mon, sealDataIds[i]);
        sBallSealScreenData->slotItems[i].name = sBallSealNames[ballSeal];
        sBallSealScreenData->slotItems[i].id = i;
    }
}

static int LoadSealListItems(void)
{
    int i;
    int byteOffset = 0;
    int bitOffset = 0;
    int numItems = GetNumOwnedSeals() + 1;
    int curItem = 0;

    if (sBallSealScreenData->sealItems)
        Free(sBallSealScreenData->sealItems);

    sBallSealScreenData->sealItems = Alloc(numItems * sizeof(struct ListMenuItem));
    for (i = 0; i < NUM_BALL_SEALS; i++)
    {
        if ((gSaveBlock1Ptr->ownedBallSeals[byteOffset] & (1 << bitOffset)) || (byteOffset == 0 && bitOffset == 0))
        {
            int ballSeal = byteOffset * 8 + bitOffset;
            sBallSealScreenData->sealItems[curItem].name = sBallSealNames[ballSeal];
            sBallSealScreenData->sealItems[curItem].id = ballSeal;
            curItem++;
        }

        if (++bitOffset == 8)
        {
            bitOffset = 0;
            byteOffset++;
        }
    }

    return numItems;
}

static void DestroySlotAndSealListMenus(void)
{
    if (sBallSealScreenData->sealItemsListMenuTaskId != 0xFF)
    {
        DestroyListMenuTask(sBallSealScreenData->sealItemsListMenuTaskId, &sBallSealScreenData->sealScrollOffset, &sBallSealScreenData->sealSelectedItem);
        sBallSealScreenData->sealItemsListMenuTaskId = 0xFF;
    }

    if (sBallSealScreenData->slotItemsListMenuTaskId != 0xFF)
    {
        DestroyListMenuTask(sBallSealScreenData->slotItemsListMenuTaskId, &sBallSealScreenData->slotScrollOffset, &sBallSealScreenData->slotSelectedItem);
        sBallSealScreenData->slotItemsListMenuTaskId = 0xFF;
    }

    if (sBallSealScreenData->scrollArrowsTaskId != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sBallSealScreenData->scrollArrowsTaskId);
        sBallSealScreenData->scrollArrowsTaskId = 0xFF;
    }
}

static void DrawSealListMenu(bool32 isActive, int numItems)
{
    struct ListMenuTemplate menuTemplate;
    if (sBallSealScreenData->sealItemsListMenuTaskId != 0xFF)
        DestroyListMenuTask(sBallSealScreenData->sealItemsListMenuTaskId, &sBallSealScreenData->sealScrollOffset, &sBallSealScreenData->sealSelectedItem);
    if (sBallSealScreenData->scrollArrowsTaskId != 0xFF)
        RemoveScrollIndicatorArrowPair(sBallSealScreenData->scrollArrowsTaskId);

    menuTemplate = sSealCaseListMenuTemplate;
    menuTemplate.items = sBallSealScreenData->sealItems;
    menuTemplate.totalItems = numItems;
    menuTemplate.cursorKind = isActive ? 0 : 1;
    sBallSealScreenData->sealItemsListMenuTaskId = ListMenuInit(&menuTemplate, sBallSealScreenData->sealScrollOffset, sBallSealScreenData->sealSelectedItem);
    if (isActive && numItems > MAX_SHOWN_ITEMS)
    {
        sBallSealScreenData->scrollArrowsTaskId = 
            AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 184, 8, 152, numItems - MAX_SHOWN_ITEMS, TAG_ARROWS, TAG_ARROWS, &sBallSealScreenData->sealScrollOffset);
    }
}

static void DrawSlotListMenu(bool32 selected)
{
    struct ListMenuTemplate menuTemplate;
    if (sBallSealScreenData->slotItemsListMenuTaskId != 0xFF)
        DestroyListMenuTask(sBallSealScreenData->slotItemsListMenuTaskId, &sBallSealScreenData->slotScrollOffset, &sBallSealScreenData->slotSelectedItem);

    LoadSlotListItems();
    menuTemplate = sSlotsListMenuTemplate;
    menuTemplate.items = sBallSealScreenData->slotItems;
    menuTemplate.cursorKind = selected ? 1 : 0;
    menuTemplate.highlightSelection = selected ? 1 : 0;
    sBallSealScreenData->slotItemsListMenuTaskId = ListMenuInit(&menuTemplate, sBallSealScreenData->slotScrollOffset, sBallSealScreenData->slotSelectedItem);
}

static void DrawMonBackSprite(void)
{
    u8 taskId;
    struct Pokemon *mon = &gPlayerParty[sBallSealScreenData->partyIndex];
    u16 species = GetMonData(mon, MON_DATA_SPECIES);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);

    if (sBallSealScreenData->monSpriteId != MAX_SPRITES)
    {
        FreeAndDestroyMonPicSprite(sBallSealScreenData->monSpriteId);
        sBallSealScreenData->monSpriteId = MAX_SPRITES;
    }

    sBallSealScreenData->monSpriteId = CreateMonPicSprite_HandleDeoxys(species, otId, personality, FALSE, 64, 48 + gMonBackPicCoords[species].y_offset, 0, TAG_MON_BACKPIC);
    gSprites[sBallSealScreenData->monSpriteId].oam.priority = 3;
}

static void RunBallSealAnimation(u8 taskId)
{
    u8 ballSeal1, ballSeal2, ballSeal3, ballSeal4;

    if (++gTasks[taskId].data[0] >= 120)
    {
        gTasks[taskId].data[0] = 0;
        ballSeal1 = GetMonData(&gPlayerParty[sBallSealScreenData->partyIndex], MON_DATA_BALL_SEAL_1);
        ballSeal2 = GetMonData(&gPlayerParty[sBallSealScreenData->partyIndex], MON_DATA_BALL_SEAL_2);
        ballSeal3 = GetMonData(&gPlayerParty[sBallSealScreenData->partyIndex], MON_DATA_BALL_SEAL_3);
        ballSeal4 = GetMonData(&gPlayerParty[sBallSealScreenData->partyIndex], MON_DATA_BALL_SEAL_4);
        LoadBallSealGraphics(ballSeal1, ballSeal2, ballSeal3, ballSeal4);
        StartBallSealAnimation(ballSeal1, ballSeal2, ballSeal3, ballSeal4, 60, 80, 2, 0, B_SIDE_PLAYER);
    }
}

static void ExitBallSealScreen(u8 taskId)
{
    DestroyTask(sBallSealScreenData->ballSealAnimationTaskId);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = ExitBallSealScreen_WaitFade;
}

static void ExitBallSealScreen_WaitFade(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        Free(sBallSealScreenData->slotItems);
        Free(sBallSealScreenData->sealItems);
        ResetSpriteData();
        SetMainCallback2(sBallSealScreenData->returnCallback);
        DestroyTask(taskId);
    }
}
