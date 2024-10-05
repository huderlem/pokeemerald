#include "global.h"
#include "comfy_anim_screen.h"
#include "bg.h"
#include "comfy_anim.h"
#include "gpu_regs.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "random.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"

static void Task_ComfyAnimScreenFadeIn(u8 taskId);
static void Task_ComfyAnimScreenProcessInput(u8 taskId);
static void Task_Exit(u8 taskId);
static void Task_ExitFadeOut(u8 taskId);
static void DrawHeaderText(void);
static void DrawBgWindowFrame(void);
static void SetAffineData(struct Sprite *sprite, s16 xScale, s16 yScale, u16 rotation);

static const struct BgTemplate sComfyAnimScreenBgTemplates[] =
{
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

enum
{
    WIN_HEADER,
};

static const struct WindowTemplate sComfyAnimScreenWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    DUMMY_WIN_TEMPLATE
};

static const u16 sComfyAnimScreenBg_Pal[] = {RGB(31, 31, 31)};
static const u16 sComfyAnimText_Pal[] = INCBIN_U16("graphics/interface/comfy_anim_screen_text.gbapal");

struct AnimDemo;

typedef void (*AnimSetupFunc)(struct AnimDemo *self);
typedef void (*AnimStartFunc)(struct AnimDemo *self);
typedef void (*AnimProcessFunc)(struct AnimDemo *self);


struct AnimDemo {
    AnimSetupFunc setup;
    AnimStartFunc start;
    AnimProcessFunc process;
    u8 spriteId;
    u32 animIds[4];
};

EWRAM_DATA static struct AnimDemo sAnimDemo = {0};
EWRAM_DATA static bool8 sIsRunningAnim = 0;
EWRAM_DATA static u8 sNextAnimDirection = 0;
EWRAM_DATA static u32 sTargetValue0 = 0;
EWRAM_DATA static u32 sTargetValue1 = 0;
EWRAM_DATA static u32 sTargetValue2 = 0;
EWRAM_DATA static u32 sTargetValue3 = 0;
EWRAM_DATA static u32 sTargetValue4 = 0;

enum {
    TAG_SQUARE = 1,
};

const u32 gComfyAnimScreenSquareGfx[] = INCBIN_U32("graphics/comfy_anim_screen/square.4bpp");
const u16 gComfyAnimScreenSquarePalette[] = INCBIN_U16("graphics/comfy_anim_screen/square.gbapal");
static const struct SpriteSheet sSpriteSheet_Square = { gComfyAnimScreenSquareGfx, 0x800, TAG_SQUARE };
static const struct SpritePalette sSpritePal_Square = { gComfyAnimScreenSquarePalette, TAG_SQUARE };

static const struct OamData sOam_Square =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x64),
    .x = 0,
    .size = SPRITE_SIZE(64x64),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
};

static const struct SpriteTemplate sSpriteTemplate_Square =
{
    .tileTag = TAG_SQUARE,
    .paletteTag = TAG_SQUARE,
    .oam = &sOam_Square,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

#define SQUARE_START_X 56
#define SQUARE_START_Y 96
#define SQUARE_END_X 184
#define SQUARE_END_Y 96
#define SQUARE_MIN_Y 64
#define SQUARE_MAX_Y 128

static void Demo_TranslateSquare_Setup(struct AnimDemo *self)
{
    LoadSpriteSheet(&sSpriteSheet_Square);
    LoadSpritePalette(&sSpritePal_Square);
    self->spriteId = CreateSprite(&sSpriteTemplate_Square, SQUARE_START_X, SQUARE_START_Y, 3);
}

void Demo_TranslateSquare_Start(struct AnimDemo *self)
{
    struct ComfyAnimEasingConfig animConfig = {
        .durationFrames = 60,
        .easingFunc = ComfyAnimEasing_Linear,
        // .easingFunc = ComfyAnimEasing_EaseInQuad,
        // .easingFunc = ComfyAnimEasing_EaseOutQuad,
        // .easingFunc = ComfyAnimEasing_EaseOutQuad,
        // .easingFunc = ComfyAnimEasing_EaseInOutQuad,
        // .easingFunc = ComfyAnimEasing_EaseInCubic,
        // .easingFunc = ComfyAnimEasing_EaseOutCubic,
        // .easingFunc = ComfyAnimEasing_EaseInOutCubic,
        // .easingFunc = ComfyAnimEasing_EaseInOutBack,
    };

    if (sNextAnimDirection == 0)
    {
        animConfig.from = Q_24_8(SQUARE_START_X);
        animConfig.to = Q_24_8(SQUARE_END_X);
    }
    else
    {
        animConfig.from = Q_24_8(SQUARE_END_X);
        animConfig.to = Q_24_8(SQUARE_START_X);
    }

    self->animIds[0] = CreateComfyAnim_Easing(&animConfig);

    sIsRunningAnim = TRUE;
    sNextAnimDirection = sNextAnimDirection ^ 1;
}

void Demo_TranslateSquare_Process(struct AnimDemo *self)
{
    gSprites[self->spriteId].x = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[0]]);
    if (gComfyAnims[self->animIds[0]].completed)
    {
        ReleaseComfyAnim(self->animIds[0]);
        sIsRunningAnim = FALSE;
    }
}

static const struct AnimDemo sAnimDemo_TranslateSquare = {
    .setup = Demo_TranslateSquare_Setup,
    .start = Demo_TranslateSquare_Start,
    .process = Demo_TranslateSquare_Process,
};

static void Demo_TranslateSquare_Spring_Setup(struct AnimDemo *self)
{
    LoadSpriteSheet(&sSpriteSheet_Square);
    LoadSpritePalette(&sSpritePal_Square);
    self->spriteId = CreateSprite(&sSpriteTemplate_Square, SQUARE_START_X, SQUARE_START_Y, 3);
}

void Demo_TranslateSquare_Spring_Start(struct AnimDemo *self)
{
    struct ComfyAnimSpringConfig animConfig = {
        .mass = Q_24_8(50),
        // COMFY_ANIM_SPRING_DEFAULT,
        COMFY_ANIM_SPRING_GENTLE,
        // COMFY_ANIM_SPRING_WOBBLY,
        // COMFY_ANIM_SPRING_STIFF,
        // COMFY_ANIM_SPRING_SLOW,
        // COMFY_ANIM_SPRING_MOLASSES,
    };

    if (sNextAnimDirection == 0)
    {
        animConfig.from = Q_24_8(SQUARE_START_X);
        animConfig.to = Q_24_8(SQUARE_END_X);
    }
    else
    {
        animConfig.from = Q_24_8(SQUARE_END_X);
        animConfig.to = Q_24_8(SQUARE_START_X);
    }

    self->animIds[0] = CreateComfyAnim_Spring(&animConfig);

    sIsRunningAnim = TRUE;
    sNextAnimDirection = sNextAnimDirection ^ 1;
}

void Demo_TranslateSquare_Spring_Process(struct AnimDemo *self)
{
    gSprites[self->spriteId].x = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[0]]);
    if (gComfyAnims[self->animIds[0]].completed)
    {
        ReleaseComfyAnim(self->animIds[0]);
        sIsRunningAnim = FALSE;
    }
}

static const struct AnimDemo sAnimDemo_TranslateSquare_Spring = {
    .setup = Demo_TranslateSquare_Spring_Setup,
    .start = Demo_TranslateSquare_Spring_Start,
    .process = Demo_TranslateSquare_Spring_Process,
};

#define SQUARE_START_SCALE 0x100
#define SQUARE_END_SCALE 0x200

static void Demo_ScaleSquare_Setup(struct AnimDemo *self)
{
    LoadSpriteSheet(&sSpriteSheet_Square);
    LoadSpritePalette(&sSpritePal_Square);
    self->spriteId = CreateSprite(&sSpriteTemplate_Square, 120, SQUARE_START_Y, 3);
}

void Demo_ScaleSquare_Start(struct AnimDemo *self)
{
    struct ComfyAnimEasingConfig animConfig = {
        .durationFrames = 60,
        // .easingFunc = ComfyAnimEasing_Linear,
        // .easingFunc = ComfyAnimEasing_EaseInQuad,
        // .easingFunc = ComfyAnimEasing_EaseOutQuad,
        // .easingFunc = ComfyAnimEasing_EaseOutQuad,
        // .easingFunc = ComfyAnimEasing_EaseInOutQuad,
        // .easingFunc = ComfyAnimEasing_EaseInCubic,
        // .easingFunc = ComfyAnimEasing_EaseOutCubic,
        // .easingFunc = ComfyAnimEasing_EaseInOutCubic,
        .easingFunc = ComfyAnimEasing_EaseInOutBack,
    };

    if (sNextAnimDirection == 0)
    {
        animConfig.from = Q_24_8(SQUARE_START_SCALE);
        animConfig.to = Q_24_8(SQUARE_END_SCALE);
    }
    else
    {
        animConfig.from = Q_24_8(SQUARE_END_SCALE);
        animConfig.to = Q_24_8(SQUARE_START_SCALE);
    }

    self->animIds[0] = CreateComfyAnim_Easing(&animConfig);

    sIsRunningAnim = TRUE;
    sNextAnimDirection = sNextAnimDirection ^ 1;
}

void Demo_ScaleSquare_Process(struct AnimDemo *self)
{
    u32 scale = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[0]]);
    SetAffineData(&gSprites[self->spriteId], scale, scale, 0);
    if (gComfyAnims[self->animIds[0]].completed)
    {
        ReleaseComfyAnim(self->animIds[0]);
        sIsRunningAnim = FALSE;
    }
}

static const struct AnimDemo sAnimDemo_ScaleSquare = {
    .setup = Demo_ScaleSquare_Setup,
    .start = Demo_ScaleSquare_Start,
    .process = Demo_ScaleSquare_Process,
};

static void Demo_ScaleSquare_Spring_Setup(struct AnimDemo *self)
{
    LoadSpriteSheet(&sSpriteSheet_Square);
    LoadSpritePalette(&sSpritePal_Square);
    self->spriteId = CreateSprite(&sSpriteTemplate_Square, 120, SQUARE_START_Y, 3);
}

void Demo_ScaleSquare_Spring_Start(struct AnimDemo *self)
{
    struct ComfyAnimSpringConfig animConfig = {
        .mass = Q_24_8(100),
        // COMFY_ANIM_SPRING_DEFAULT,
        // COMFY_ANIM_SPRING_GENTLE,
        // COMFY_ANIM_SPRING_WOBBLY,
        // COMFY_ANIM_SPRING_STIFF,
        // COMFY_ANIM_SPRING_SLOW,
        // COMFY_ANIM_SPRING_MOLASSES,
         .tension = Q_24_8(200), .friction = Q_24_8(650),
    };

    if (sNextAnimDirection == 0)
    {
        animConfig.from = Q_24_8(SQUARE_START_SCALE);
        animConfig.to = Q_24_8(SQUARE_END_SCALE);
    }
    else
    {
        animConfig.from = Q_24_8(SQUARE_END_SCALE);
        animConfig.to = Q_24_8(SQUARE_START_SCALE);
    }

    self->animIds[0] = CreateComfyAnim_Spring(&animConfig);

    sIsRunningAnim = TRUE;
    sNextAnimDirection = sNextAnimDirection ^ 1;
}

void Demo_ScaleSquare_Spring_Process(struct AnimDemo *self)
{
    u32 scale = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[0]]);
    SetAffineData(&gSprites[self->spriteId], scale, scale, 0);
    if (gComfyAnims[self->animIds[0]].completed)
    {
        ReleaseComfyAnim(self->animIds[0]);
        sIsRunningAnim = FALSE;
    }
}

static const struct AnimDemo sAnimDemo_ScaleSquare_Spring = {
    .setup = Demo_ScaleSquare_Spring_Setup,
    .start = Demo_ScaleSquare_Spring_Start,
    .process = Demo_ScaleSquare_Spring_Process,
};

#define SQUARE_START_ROTATION 0x0
#define SQUARE_END_ROTATION -0x80 * 0x100

static void Demo_TumbleSquare_Setup(struct AnimDemo *self)
{
    LoadSpriteSheet(&sSpriteSheet_Square);
    LoadSpritePalette(&sSpritePal_Square);
    self->spriteId = CreateSprite(&sSpriteTemplate_Square, SQUARE_START_X, SQUARE_START_Y, 3);
}

void Demo_TumbleSquare_Start(struct AnimDemo *self)
{
    struct ComfyAnimEasingConfig translateConfig = {
        .durationFrames = 50,
        .easingFunc = ComfyAnimEasing_EaseInOutCubic,
    };
    struct ComfyAnimEasingConfig scaleConfig = {
        .durationFrames = 50,
        .easingFunc = ComfyAnimEasing_EaseInOutCubic,
    };
    struct ComfyAnimEasingConfig rotationConfig = {
        .durationFrames = 50,
        .easingFunc = ComfyAnimEasing_EaseInOutCubic,
    };

    if (sNextAnimDirection == 0)
    {
        translateConfig.from = Q_24_8(SQUARE_START_X);
        translateConfig.to = Q_24_8(SQUARE_END_X);
        scaleConfig.from = Q_24_8(SQUARE_START_SCALE);
        scaleConfig.to = Q_24_8(SQUARE_END_SCALE);
        rotationConfig.from = Q_24_8(SQUARE_START_ROTATION);
        rotationConfig.to = Q_24_8(SQUARE_END_ROTATION);
    }
    else
    {
        translateConfig.from = Q_24_8(SQUARE_END_X);
        translateConfig.to = Q_24_8(SQUARE_START_X);
        scaleConfig.from = Q_24_8(SQUARE_END_SCALE);
        scaleConfig.to = Q_24_8(SQUARE_START_SCALE);
        rotationConfig.from = Q_24_8(SQUARE_END_ROTATION);
        rotationConfig.to = Q_24_8(SQUARE_START_ROTATION);
    }

    self->animIds[0] = CreateComfyAnim_Easing(&translateConfig);
    self->animIds[1] = CreateComfyAnim_Easing(&scaleConfig);
    self->animIds[2] = CreateComfyAnim_Easing(&rotationConfig);

    sIsRunningAnim = TRUE;
    sNextAnimDirection = sNextAnimDirection ^ 1;
}

void Demo_TumbleSquare_Process(struct AnimDemo *self)
{
    int scale = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[1]]);
    int rotation = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[2]]);
    gSprites[self->spriteId].x = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[0]]);
    SetAffineData(&gSprites[self->spriteId], scale, scale, rotation);
    if (gComfyAnims[self->animIds[0]].completed && gComfyAnims[self->animIds[1]].completed && gComfyAnims[self->animIds[2]].completed)
    {
        ReleaseComfyAnim(self->animIds[0]);
        ReleaseComfyAnim(self->animIds[1]);
        ReleaseComfyAnim(self->animIds[2]);
        sIsRunningAnim = FALSE;
    }
}

static const struct AnimDemo sAnimDemo_TumbleSquare = {
    .setup = Demo_TumbleSquare_Setup,
    .start = Demo_TumbleSquare_Start,
    .process = Demo_TumbleSquare_Process,
};

static void Demo_TumbleSquare_Spring_Setup(struct AnimDemo *self)
{
    LoadSpriteSheet(&sSpriteSheet_Square);
    LoadSpritePalette(&sSpritePal_Square);
    self->spriteId = CreateSprite(&sSpriteTemplate_Square, SQUARE_START_X, SQUARE_START_Y, 3);
}

void Demo_TumbleSquare_Spring_Start(struct AnimDemo *self)
{
    struct ComfyAnimSpringConfig translateConfig = {
        .mass = Q_24_8(150),
        .tension = Q_24_8(150), .friction = Q_24_8(1250),
        .clampAfter = 1,
    };
    struct ComfyAnimSpringConfig scaleConfig = {
        .mass = Q_24_8(150),
        .tension = Q_24_8(150), .friction = Q_24_8(1250),
        .clampAfter = 1,
    };
    struct ComfyAnimSpringConfig rotationConfig = {
        .mass = Q_24_8(150),
        .tension = Q_24_8(150), .friction = Q_24_8(1250),
        .clampAfter = 1,
    };

    if (sNextAnimDirection == 0)
    {
        translateConfig.from = Q_24_8(SQUARE_START_X);
        translateConfig.to = Q_24_8(SQUARE_END_X);
        scaleConfig.from = Q_24_8(SQUARE_START_SCALE);
        scaleConfig.to = Q_24_8(SQUARE_END_SCALE);
        rotationConfig.from = Q_24_8(SQUARE_START_ROTATION);
        rotationConfig.to = Q_24_8(SQUARE_END_ROTATION);
    }
    else
    {
        translateConfig.from = Q_24_8(SQUARE_END_X);
        translateConfig.to = Q_24_8(SQUARE_START_X);
        scaleConfig.from = Q_24_8(SQUARE_END_SCALE);
        scaleConfig.to = Q_24_8(SQUARE_START_SCALE);
        rotationConfig.from = Q_24_8(SQUARE_END_ROTATION);
        rotationConfig.to = Q_24_8(SQUARE_START_ROTATION);
    }

    self->animIds[0] = CreateComfyAnim_Spring(&translateConfig);
    self->animIds[1] = CreateComfyAnim_Spring(&scaleConfig);
    self->animIds[2] = CreateComfyAnim_Spring(&rotationConfig);

    sIsRunningAnim = TRUE;
    sNextAnimDirection = sNextAnimDirection ^ 1;
}

void Demo_TumbleSquare_Spring_Process(struct AnimDemo *self)
{
    int scale = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[1]]);
    int rotation = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[2]]);
    gSprites[self->spriteId].x = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[0]]);
    SetAffineData(&gSprites[self->spriteId], scale, scale, rotation);
    if (gComfyAnims[self->animIds[0]].completed && gComfyAnims[self->animIds[1]].completed && gComfyAnims[self->animIds[2]].completed)
    {
        ReleaseComfyAnim(self->animIds[0]);
        ReleaseComfyAnim(self->animIds[1]);
        ReleaseComfyAnim(self->animIds[2]);
        sIsRunningAnim = FALSE;
    }
}

static const struct AnimDemo sAnimDemo_TumbleSquare_Spring = {
    .setup = Demo_TumbleSquare_Spring_Setup,
    .start = Demo_TumbleSquare_Spring_Start,
    .process = Demo_TumbleSquare_Spring_Process,
};

static void Demo_TumbleSquare_Chase_Setup(struct AnimDemo *self)
{
    LoadSpriteSheet(&sSpriteSheet_Square);
    LoadSpritePalette(&sSpritePal_Square);
    sTargetValue0 = SQUARE_START_X;
    sTargetValue1 = SQUARE_START_Y;
    sTargetValue2 = SQUARE_START_SCALE;
    sTargetValue3 = SQUARE_START_ROTATION;
    self->spriteId = CreateSprite(&sSpriteTemplate_Square, SQUARE_START_X, SQUARE_START_Y, 3);
}

void Demo_TumbleSquare_Chase_Start(struct AnimDemo *self)
{
    struct ComfyAnimSpringConfig xTranslateConfig = {
        .mass = Q_24_8(250),
        .tension = Q_24_8(150), .friction = Q_24_8(1250),
        .clampAfter = 0,
        .from = Q_24_8(sTargetValue0),
        .to = Q_24_8(sTargetValue0),
    };
    struct ComfyAnimSpringConfig yTranslateConfig = {
        .mass = Q_24_8(250),
        .tension = Q_24_8(150), .friction = Q_24_8(1250),
        .clampAfter = 0,
        .from = Q_24_8(sTargetValue1),
        .to = Q_24_8(sTargetValue1),
    };
    struct ComfyAnimSpringConfig scaleConfig = {
        .mass = Q_24_8(250),
        .tension = Q_24_8(150), .friction = Q_24_8(1250),
        .clampAfter = 0,
        .from = Q_24_8(sTargetValue2),
        .to = Q_24_8(sTargetValue2),
    };
    struct ComfyAnimSpringConfig rotationConfig = {
        .mass = Q_24_8(250),
        .tension = Q_24_8(150), .friction = Q_24_8(1250),
        .clampAfter = 0,
        .from = Q_24_8(sTargetValue3),
        .to = Q_24_8(sTargetValue3),
    };

    self->animIds[0] = CreateComfyAnim_Spring(&xTranslateConfig);
    self->animIds[1] = CreateComfyAnim_Spring(&yTranslateConfig);
    self->animIds[2] = CreateComfyAnim_Spring(&scaleConfig);
    self->animIds[3] = CreateComfyAnim_Spring(&rotationConfig);

    sIsRunningAnim = TRUE;
}

void Demo_TumbleSquare_Chase_Process(struct AnimDemo *self)
{
    int scale = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[2]]);
    int rotation = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[3]]);
    gSprites[self->spriteId].x = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[0]]);
    gSprites[self->spriteId].y = ReadComfyAnimValueSmooth(&gComfyAnims[self->animIds[1]]);
    SetAffineData(&gSprites[self->spriteId], scale, scale, rotation);

    gComfyAnims[self->animIds[0]].config.data.spring.to = Q_24_8(sTargetValue0);
    gComfyAnims[self->animIds[1]].config.data.spring.to = Q_24_8(sTargetValue1);
    gComfyAnims[self->animIds[2]].config.data.spring.to = Q_24_8(sTargetValue2);
    gComfyAnims[self->animIds[3]].config.data.spring.to = Q_24_8(sTargetValue3);
}

static const struct AnimDemo sAnimDemo_TumbleSquare_Chase = {
    .setup = Demo_TumbleSquare_Chase_Setup,
    .start = Demo_TumbleSquare_Chase_Start,
    .process = Demo_TumbleSquare_Chase_Process,
};

static void ChooseRandomTargetValues(void)
{
    sTargetValue0 = SQUARE_START_X + Random() % (SQUARE_END_X - SQUARE_START_X);
    sTargetValue1 = SQUARE_MIN_Y + Random() % (SQUARE_MAX_Y - SQUARE_MIN_Y);
    sTargetValue2 = SQUARE_START_SCALE + Random() % (SQUARE_END_SCALE - SQUARE_START_SCALE);
    sTargetValue3 = SQUARE_END_ROTATION + -(Random() % (SQUARE_START_ROTATION - SQUARE_END_ROTATION));
}


static void MainCB2(void)
{
    RunTasks();
    AdvanceComfyAnimations();
    AnimateSprites();
    if (sIsRunningAnim)
        sAnimDemo.process(&sAnimDemo);
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    ScanlineEffect_InitHBlankDmaTransfer();
    TransferPlttBuffer();
}

void CB2_InitComfyAnimScreen(void)
{
    switch (gMain.state)
    {
    default:
    case 0:
        sAnimDemo = sAnimDemo_TumbleSquare_Chase;
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sComfyAnimScreenBgTemplates, ARRAY_COUNT(sComfyAnimScreenBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);
        InitWindows(sComfyAnimScreenWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sComfyAnimScreenBg_Pal, BG_PLTT_ID(0), sizeof(sComfyAnimScreenBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sComfyAnimText_Pal, BG_PLTT_ID(1), sizeof(sComfyAnimText_Pal));
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_HEADER);
        DrawHeaderText();
        gMain.state++;
        break;
    case 7:
        DrawBgWindowFrame();
        sAnimDemo.setup(&sAnimDemo);
        gMain.state++;
        break;
    case 8:
    {
        CreateTask(Task_ComfyAnimScreenFadeIn, 0);
        gMain.state++;
        break;
    }
    case 9:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_ComfyAnimScreenFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_ComfyAnimScreenProcessInput;
}

static void Task_ComfyAnimScreenProcessInput(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        if (sIsRunningAnim)
            return;

        sAnimDemo.start(&sAnimDemo);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        sIsRunningAnim = FALSE;
        gTasks[taskId].func = Task_Exit;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        ChooseRandomTargetValues();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {

    }
}

static void Task_Exit(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_ExitFadeOut;
}

static void Task_ExitFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        ReleaseComfyAnims();
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void DrawHeaderText(void)
{
    FillWindowPixelBuffer(WIN_HEADER, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_ComfyAnims, 8, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
}

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrame(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  0,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      1,  0, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 29,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     0,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   29,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  0,  3,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      1,  3, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 29,  3,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  0,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      1,  4, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 29,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     0,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   29,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  0, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      1, 19, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 29, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}

static void SetAffineData(struct Sprite *sprite, s16 xScale, s16 yScale, u16 rotation)
{
    u8 matrixNum;
    struct ObjAffineSrcData affineSrcData;
    struct OamMatrix dest;

    affineSrcData.xScale = xScale;
    affineSrcData.yScale = yScale;
    affineSrcData.rotation = rotation;

    matrixNum = sprite->oam.matrixNum;

    ObjAffineSet(&affineSrcData, &dest, 1, 2);
    gOamMatrices[matrixNum].a = dest.a;
    gOamMatrices[matrixNum].b = dest.b;
    gOamMatrices[matrixNum].c = dest.c;
    gOamMatrices[matrixNum].d = dest.d;
}
