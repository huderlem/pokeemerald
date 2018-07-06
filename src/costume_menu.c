#include "global.h"
#include "main.h"
#include "bg.h"
#include "costume_menu.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/rgb.h"
#include "constants/songs.h"

static EWRAM_DATA struct SpriteTemplate sSpriteTemplateBase = {};
u8 gCostumeSpriteId[8];
u16 selection;
u8 sScrollBarState;

#define NONE 0
#define SCROLL_LEFT 1
#define SCROLL_RIGHT 2

// static declarations

static void CreateOverworldScrollBar(void);
static void UpdateSpritePositions(void);
static void HandleKeyPresses(void);
static void UpdateBackgroundGraphics(void);
static void UpdateSlotNumbers(struct Sprite *sprite);
static void SpriteCallback_UpdateSpritePosition(struct Sprite *sprite);
static void TrainerSpriteCallback(struct Sprite *sprite);
static void CreateTrainerSprite(void);
static void SetSpriteTemplateParameters(void);
static void FreeSpriteTilesIfUnused(u16);
static void FreeSpritePaletteIfUnused(u8);

//.rodata

static const u8 gGraphics_CostumeScreenBackground[] = INCBIN_U8("graphics/costume_screen/background.4bpp.lz");
static const u16 gPalette_CostumeScreenBackground[] = INCBIN_U16("graphics/costume_screen/background.gbapal");
static const u16 gTilemap_CostumeScreenBackground[] = INCBIN_U16("graphics/costume_screen/background_tilemap.bin.lz");

const struct BgTemplate gBgTemplates_CostumeScreen[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 1,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

static const struct OamData sOamData_CostumeSlots =
{
    .y = 0,                        //top of sprite
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,                    // .4bpp if 0, .8bpp if 1?
    .shape = 0,                    // square, wide, tall
    .x = 0,                        // left of sprite
    .matrixNum = 0,
    .size = 2,                    //0 = 8x8, 1 = 16x16, 2 = 32x32, 3 = 64x64? (if shape is square)
    .tileNum = 0,
    .priority = 0,                //draw order, higher = drawn earlier
    .paletteNum = 0,
    .affineParam = 0,
};

static const struct OamData sOamData_TrainerSprite =
{
    .y = 0,                      //top of sprite
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,                    // .4bpp if 0, .8bpp if 1?
    .shape = 0,                  // square, wide, tall
    .x = 0,                      // left of sprite
    .matrixNum = 0,
    .size = 3,                   //0 = 8x8, 1 = 16x16, 2 = 32x32, 3 = 64x64? (if shape is square)
    .tileNum = 0,
    .priority = 0,               //draw order, higher = drawn earlier
    .paletteNum = 0,
    .affineParam = 0,
};

const u8 gGraphics_Costume_Red[] =     INCBIN_U8("graphics/costume_screen/overworlds/red.4bpp.lz");
const u8 gPalette_Costume_Red[] =      INCBIN_U8("graphics/costume_screen/overworlds/red.gbapal.lz");
const u8 gGraphics_Costume_Lyra[] =    INCBIN_U8("graphics/costume_screen/overworlds/lyra.4bpp.lz");
const u8 gPalette_Costume_Lyra[] =     INCBIN_U8("graphics/costume_screen/overworlds/lyra.gbapal.lz");
const u8 gGraphics_Costume_Lance[] =   INCBIN_U8("graphics/costume_screen/overworlds/lance.4bpp.lz");
const u8 gPalette_Costume_Lance[] =    INCBIN_U8("graphics/costume_screen/overworlds/lance.gbapal.lz");
const u8 gGraphics_Costume_Misty[] =   INCBIN_U8("graphics/costume_screen/overworlds/misty.4bpp.lz");
const u8 gPalette_Costume_Misty[] =    INCBIN_U8("graphics/costume_screen/overworlds/misty.gbapal.lz");
const u8 gGraphics_Costume_RocketM[] = INCBIN_U8("graphics/costume_screen/overworlds/rocket_m.4bpp.lz");
const u8 gPalette_Costume_RocketM[] =  INCBIN_U8("graphics/costume_screen/overworlds/rocket_m.gbapal.lz");
const u8 gGraphics_Costume_RocketF[] = INCBIN_U8("graphics/costume_screen/overworlds/rocket_f.4bpp.lz");
const u8 gPalette_Costume_RocketF[] =  INCBIN_U8("graphics/costume_screen/overworlds/rocket_f.gbapal.lz");
const u8 gGraphics_Costume_ProfOak[] = INCBIN_U8("graphics/costume_screen/overworlds/prof_oak.4bpp.lz");
const u8 gPalette_Costume_ProfOak[] =  INCBIN_U8("graphics/costume_screen/overworlds/prof_oak.gbapal.lz");

const u8 gTrainerFrontSprite_Red[] =   INCBIN_U8("graphics/costume_screen/trainers/red.4bpp.lz");
const u8 gPalette_Trainer_Red[] =      INCBIN_U8("graphics/costume_screen/trainers/red.gbapal.lz");
const u8 gTrainerFrontSprite_Lyra[] =  INCBIN_U8("graphics/costume_screen/trainers/lyra.4bpp.lz");
const u8 gPalette_Trainer_Lyra[] =     INCBIN_U8("graphics/costume_screen/trainers/lyra.gbapal.lz");
const u8 gTrainerFrontSprite_Lance[] = INCBIN_U8("graphics/costume_screen/trainers/lance.4bpp.lz");
const u8 gPalette_Trainer_Lance[] =    INCBIN_U8("graphics/costume_screen/trainers/lance.gbapal.lz");
const u8 gTrainerFrontSprite_Misty[] = INCBIN_U8("graphics/costume_screen/trainers/misty.4bpp.lz");
const u8 gPalette_Trainer_Misty[] =    INCBIN_U8("graphics/costume_screen/trainers/misty.gbapal.lz");
const u8 gTrainerFrontSprite_RocketGruntM[] = INCBIN_U8("graphics/costume_screen/trainers/rocket_m.4bpp.lz");
const u8 gPalette_Trainer_RocketM[] =  INCBIN_U8("graphics/costume_screen/trainers/rocket_m.gbapal.lz");
const u8 gTrainerFrontSprite_RocketGruntF[] = INCBIN_U8("graphics/costume_screen/trainers/rocket_f.4bpp.lz");
const u8 gPalette_Trainer_RocketF[] =  INCBIN_U8("graphics/costume_screen/trainers/rocket_f.gbapal.lz");
const u8 gTrainerFrontSprite_ProfessorOak[] = INCBIN_U8("graphics/costume_screen/trainers/prof_oak.4bpp.lz");
const u8 gPalette_Trainer_ProfOak[] =  INCBIN_U8("graphics/costume_screen/trainers/prof_oak.gbapal.lz");

#include "data/costume_menu_tables.h"

// .text

void LoadBackgroundGraphics(void)
{
    LZ77UnCompVram(gGraphics_CostumeScreenBackground, (void *)VRAM);
    LoadPalette(gPalette_CostumeScreenBackground, 0x00, 0x20);
    LZ77UnCompVram(gTilemap_CostumeScreenBackground, (u16 *)BG_SCREEN_ADDR(1));
    
    InitBgsFromTemplates(0, gBgTemplates_CostumeScreen, 1);
    ShowBg(0);
    HideBg(3);
    HideBg(1);
    HideBg(2);
}

void CB2_CostumeMenu(void)
{
    switch (gMain.state)
    {
        case 0:
        default:
            ScanlineEffect_Stop();
            ResetTasks();
            ResetSpriteData();
            ResetPaletteFade();
            FreeAllSpritePalettes();
            DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000)
            gMain.state++;
            break;
        case 1:
            selection = 0;            //selection = gSaveBlock2Ptr->costume
            sScrollBarState = NONE;
            LoadBackgroundGraphics();
            CreateOverworldScrollBar();
            SetSpriteTemplateParameters();
            CreateTrainerSprite();
            BuildOamBuffer();
            gMain.state++;
            break;
        case 2:
            SetMainCallback2(HandleKeyPresses);
            break;
    }
}

u8 slotSize = 32;
s16 xPos_initial = 24;
s16 yPos = 136;

static void CreateNewScrollBarSlot(s8 slot, u8 scrollDirection)
{
    u8 spriteId;

    // Only load the sprite pic into VRAM if it isn't already there.
    if (GetSpriteTileStartByTag(sCostumeOverworldSpriteTable[selection + slot].tag) == 0xFFFF)
    {
        LoadCompressedObjectPic(&sCostumeOverworldSpriteTable[selection + slot]);
    }
    LoadCompressedObjectPalette(&sCostumeOverworldPaletteTable[selection + slot]);
    spriteId = CreateSprite(&sSpriteTemplate_Costumes[selection + slot], xPos_initial + slot*slotSize, yPos, 0);
    gSprites[spriteId].data[0] = slot;
    sScrollBarState = scrollDirection;
}

static void HandleKeyPresses(void)
{
    if (sScrollBarState == NONE)
    {
        if ((gMain.newKeys & DPAD_LEFT) && (selection != NUMBER_OF_COSTUMES))
        {
            CreateNewScrollBarSlot(7, SCROLL_LEFT);
            selection++;
            PlaySE(SE_Z_SCROLL);
        }
        if ((gMain.newKeys & DPAD_RIGHT) && (selection != 0))
        {
            CreateNewScrollBarSlot(-1, SCROLL_RIGHT);
            selection--;
            PlaySE(SE_Z_SCROLL);
        }
        if (gMain.newKeys & A_BUTTON) // && (GetFlag(CostumeFlags(selection)) == TRUE)
        {
            //pop-up confirmation menu first
            PlaySE(SE_SELECT);
            //gSaveBlock2Ptr->costume = selection;
            //gSaveBlock2Ptr->gender = gCostumes[selection].gender;
        }
        if (gMain.newKeys & (B_BUTTON | START_BUTTON))
        {
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
            PlaySE(SE_PC_OFF);
            SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        }
        else if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT)) // | A_BUTTON))
        {
            PlaySE(SE_HAZURE);
        }
    }

    AnimateSprites();
    BuildOamBuffer();
}


static void CreateOverworldScrollBar(void)
{
    s8 slot;
    u8 xPos;
    u16 displayedCostume;

    for (slot = 0; slot <= 6; slot++)
    {
        xPos = xPos_initial + (slot * slotSize);
        displayedCostume = selection + slot;
        LoadCompressedObjectPic(&sCostumeOverworldSpriteTable[displayedCostume]);
        LoadCompressedObjectPalette(&sCostumeOverworldPaletteTable[displayedCostume]);
        gCostumeSpriteId[slot] = CreateSprite(&sSpriteTemplate_Costumes[displayedCostume], xPos, yPos, 0);
        gSprites[gCostumeSpriteId[slot]].data[0] = slot;
    }
}

static void UpdateSpritePositions(void)
{
    s8 slot;
    
    for (slot = 0; slot <=6; slot++)
    {
        DestroySprite(&gSprites[gCostumeSpriteId[slot]]);
    }

    CreateOverworldScrollBar();
}

static void UpdateSlotNumbers(struct Sprite *sprite)
{
    if (sScrollBarState == SCROLL_LEFT)
    {
        sprite->data[0]--;
        sprite->callback = SpriteCallback_UpdateSpritePosition;
    }
    if (sScrollBarState == SCROLL_RIGHT)
    {
        sprite->data[0]++;
        sprite->callback = SpriteCallback_UpdateSpritePosition;
    }
}

static void SpriteCallback_UpdateSpritePosition(struct Sprite *sprite)
{
    // data[0] represents what slot within the scroll bar the sprite is in.
    // UpdateSlotNumbers() increments/decrements data[0] depending on the 
    // button press in order for this callback to compare the current
    // position of the sprite to its next slot position

    if (sprite->pos1.x > (xPos_initial + sprite->data[0]*slotSize))        //DPAD_LEFT
    {
        sprite->pos1.x -= 2;
    }
    if (sprite->pos1.x < (xPos_initial + sprite->data[0]*slotSize))        //DPAD_RIGHT
    {
        sprite->pos1.x += 2;
    }
    if (sprite->pos1.x <= xPos_initial - slotSize || sprite->pos1.x >= xPos_initial + (7*slotSize))
	// Need to add a check to see if other members of the gSprites array have the same paletteTag.
    {
        u8 paletteNum = sprite->oam.paletteNum; 
        u16 sheetTileStart = sprite->sheetTileStart;
        DestroySprite(sprite);
        FreeSpritePaletteIfUnused(paletteNum);
        FreeSpriteTilesIfUnused(sheetTileStart);
    }
    else if (sprite->pos1.x == xPos_initial + sprite->data[0]*slotSize)
    {
        sScrollBarState = NONE;
        sprite->callback = UpdateSlotNumbers;
    }
}

static void SetSpriteTemplateParameters(void)
{
    sSpriteTemplateBase.anims = gDummySpriteAnimTable;
    sSpriteTemplateBase.images = NULL;
    sSpriteTemplateBase.affineAnims = gDummySpriteAffineAnimTable;
    sSpriteTemplateBase.oam = &sOamData_TrainerSprite;
    sSpriteTemplateBase.callback = TrainerSpriteCallback;
}

static void CreateTrainerSprite(void)
{
    u8 spriteId;

    sSpriteTemplateBase.tileTag = sCostumeTrainerSpriteTable[selection].tag;
    sSpriteTemplateBase.paletteTag = sCostumeTrainerPaletteTable[selection].tag;
    LoadCompressedObjectPic(&sCostumeTrainerSpriteTable[selection]);
    LoadCompressedObjectPalette(&sCostumeTrainerPaletteTable[selection]);
    spriteId = CreateSprite(&sSpriteTemplateBase, 200, 48, 0);
    gSprites[spriteId].data[1] = selection;
}

static void TrainerSpriteCallback(struct Sprite *sprite)
{
    if (sprite->data[1] != selection)
    {
        FreeSpritePalette(sprite);
        FreeSpriteTiles(sprite);
        DestroySprite(sprite);
        CreateTrainerSprite();
    }
}

static void FreeSpriteTilesIfUnused(u16 tileStart)
{
    u8 i;
    u16 tag = GetSpriteTileTagByTileStart(tileStart);

    if (tag != 0xFFFF)
    {
        for (i = 0; i < MAX_SPRITES; i++)
            if (gSprites[i].inUse && gSprites[i].usingSheet && tileStart == gSprites[i].sheetTileStart)
                return;
        FreeSpriteTilesByTag(tag);
    }
}

static void FreeSpritePaletteIfUnused(u8 paletteNum)
{
    u8 i;
    u16 tag = GetSpritePaletteTagByPaletteNum(paletteNum);

    if (tag != 0xFFFF)
    {
        for (i = 0; i < MAX_SPRITES; i++)
            if (gSprites[i].inUse && gSprites[i].oam.paletteNum == paletteNum)
                return;
        FreeSpritePaletteByTag(tag);
    }
}
