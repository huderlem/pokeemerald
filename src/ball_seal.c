#include "global.h"
#include "ball_seal.h"
#include "battle.h"
#include "battle_anim.h"
#include "decompress.h"
#include "palette.h"
#include "random.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "trig.h"
#include "util.h"
#include "constants/ball_seals.h"
#include "constants/rgb.h"

static void CleanupBallSealAnimation(u8);
static void FreeBallSealGraphics(u8);
static void CreateBallSealParticles_HeartCloudSmallPink(u8, u8, u8, u8, int);
static void CreateBallSealParticles_HeartSpinSmallPink(u8, u8, u8, u8, int);
static void CreateBallSealParticles_HeartCircleSmallPink(u8, u8, u8, u8, int);
static void CreateBallSealParticles_LetterA(u8, u8, u8, u8, int);
static void CreateBallSealParticles_OrbsRainbow(u8, u8, u8, u8, int);
static void BallSeal_HeartCloudSmallPink_Step0(struct Sprite*);
static void BallSeal_HeartSpinSmallPink_Step0(struct Sprite*);
static void BallSeal_HeartCircleSmallPink_Step0(struct Sprite*);
static void BallSeal_StaticLetter_Step0(struct Sprite*);
static void BallSeal_OrbsRainbow_Step0(struct Sprite*);
static void BallSeal_OrbsRainbow_BlendTask(u8 taskId);

#define TAG_HEARTCLOUD_SMALL_PINK  56000
#define TAG_HEARTSPIN_SMALL_PINK   56001
#define TAG_HEARTCIRCLE_SMALL_PINK 56002
#define TAG_LETTER_A               56003
#define TAG_ORBS_RAINBOW           56004

static const u8 sBallSealTypes[] =
{
    0, // BALL_SEAL_NONE
    0, // BALL_SEAL_HEARTCLOUD_SMALL_PINK
    0, // BALL_SEAL_HEARTSPIN_SMALL_PINK
    0, // BALL_SEAL_HEARTCIRCLE_SMALL_PINK
    BALL_SEAL_TYPE_STATIC_ICON, // BALL_SEAL_LETTER_A
    0, // BALL_SEAL_ORBS_RAINBOW
};

static void (*const sCreateBallSealParticleFuncs[])(u8, u8, u8, u8, int) =
{
    NULL, // BALL_SEAL_NONE
    CreateBallSealParticles_HeartCloudSmallPink, // BALL_SEAL_HEARTCLOUD_SMALL_PINK
    CreateBallSealParticles_HeartSpinSmallPink,  // BALL_SEAL_HEARTSPIN_SMALL_PINK
    CreateBallSealParticles_HeartCircleSmallPink,  // BALL_SEAL_HEARTCIRCLE_SMALL_PINK
    CreateBallSealParticles_LetterA,  // BALL_SEAL_LETTER_A
    CreateBallSealParticles_OrbsRainbow, // BALL_SEAL_ORBS_RAINBOW
};

static const u8 sBallSealSpriteSheet_HeartSmallPink[] = INCBIN_U8("graphics/battle_anims/ball_seals/heart_small_pink.4bpp");
static const u16 sBallSealPalette_HeartSmallPink[] = INCBIN_U16("graphics/battle_anims/ball_seals/heart_small_pink.gbapal");
static const u8 sBallSealSpriteSheet_LetterA[] = INCBIN_U8("graphics/battle_anims/ball_seals/letter_a.4bpp");
static const u16 sBallSealPalette_LetterA[] = INCBIN_U16("graphics/battle_anims/ball_seals/letter_a.gbapal");
static const u8 sBallSealSpriteSheet_OrbSmall[] = INCBIN_U8("graphics/battle_anims/ball_seals/orb_small.4bpp");
static const u16 sBallSealPalette_OrbSmall[] = INCBIN_U16("graphics/battle_anims/ball_seals/orb_small.gbapal");

static const struct SpriteSheet sBallSealSpriteSheets[] =
{
    { }, // BALL_SEAL_NONE
    { sBallSealSpriteSheet_HeartSmallPink, sizeof(sBallSealSpriteSheet_HeartSmallPink), TAG_HEARTCLOUD_SMALL_PINK }, // BALL_SEAL_HEARTCLOUD_SMALL_PINK
    { sBallSealSpriteSheet_HeartSmallPink, sizeof(sBallSealSpriteSheet_HeartSmallPink), TAG_HEARTSPIN_SMALL_PINK }, // BALL_SEAL_HEARTSPIN_SMALL_PINK
    { sBallSealSpriteSheet_HeartSmallPink, sizeof(sBallSealSpriteSheet_HeartSmallPink), TAG_HEARTCIRCLE_SMALL_PINK }, // BALL_SEAL_HEARTCIRCLE_SMALL_PINK
    { sBallSealSpriteSheet_LetterA, sizeof(sBallSealSpriteSheet_LetterA), TAG_LETTER_A }, // BALL_SEAL_LETTER_A
    { sBallSealSpriteSheet_OrbSmall, sizeof(sBallSealSpriteSheet_OrbSmall), TAG_ORBS_RAINBOW }, // BALL_SEAL_ORBS_RAINBOW
};

static const struct SpritePalette sBallSealPalettes[] =
{
    { }, // BALL_SEAL_NONE
    { sBallSealPalette_HeartSmallPink, TAG_HEARTCLOUD_SMALL_PINK }, // BALL_SEAL_HEARTCLOUD_SMALL_PINK
    { sBallSealPalette_HeartSmallPink, TAG_HEARTSPIN_SMALL_PINK }, // BALL_SEAL_HEARTSPIN_SMALL_PINK
    { sBallSealPalette_HeartSmallPink, TAG_HEARTCIRCLE_SMALL_PINK }, // BALL_SEAL_HEARTCIRCLE_SMALL_PINK
    { sBallSealPalette_LetterA, TAG_LETTER_A }, // BALL_SEAL_LETTER_A
    { sBallSealPalette_OrbSmall, TAG_ORBS_RAINBOW }, // BALL_SEAL_ORBS_RAINBOW
};

static const struct OamData sOamData_SmallNormal =
{
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = ST_OAM_SQUARE,
    .size = 0,
};

static const struct OamData sOamData_SmallAffine =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = ST_OAM_SQUARE,
    .size = 0,
};

static const struct OamData sOamData_BigBlend =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = ST_OAM_OBJ_NORMAL,
    .shape = ST_OAM_SQUARE,
    .size = 2,
};

static const struct SpriteTemplate gBallSealSpriteTemplate_HeartCloudSmallPink =
{
    .tileTag = TAG_HEARTCLOUD_SMALL_PINK,
    .paletteTag = TAG_HEARTCLOUD_SMALL_PINK,
    .oam = &sOamData_SmallNormal,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = BallSeal_HeartCloudSmallPink_Step0,
};

static const struct SpriteTemplate gBallSealSpriteTemplate_HeartSpinSmallPink =
{
    .tileTag = TAG_HEARTSPIN_SMALL_PINK,
    .paletteTag = TAG_HEARTSPIN_SMALL_PINK,
    .oam = &sOamData_SmallAffine,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = BallSeal_HeartSpinSmallPink_Step0,
};

static const struct SpriteTemplate gBallSealSpriteTemplate_HeartCircleSmallPink =
{
    .tileTag = TAG_HEARTCIRCLE_SMALL_PINK,
    .paletteTag = TAG_HEARTCIRCLE_SMALL_PINK,
    .oam = &sOamData_SmallNormal,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = BallSeal_HeartCircleSmallPink_Step0,
};

static const struct SpriteTemplate gBallSealSpriteTemplate_LetterA =
{
    .tileTag = TAG_LETTER_A,
    .paletteTag = TAG_LETTER_A,
    .oam = &sOamData_BigBlend,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = BallSeal_StaticLetter_Step0,
};

static const struct SpriteTemplate gBallSealSpriteTemplate_OrbsRainbow =
{
    .tileTag = TAG_ORBS_RAINBOW,
    .paletteTag = TAG_ORBS_RAINBOW,
    .oam = &sOamData_SmallNormal,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = BallSeal_OrbsRainbow_Step0,
};


void LoadBallSealGraphics(u8 ballSeal1, u8 ballSeal2, u8 ballSeal3, u8 ballSeal4)
{
    if (ballSeal1)
    {
        LoadSpriteSheet(&sBallSealSpriteSheets[ballSeal1]);
        LoadSpritePalette(&sBallSealPalettes[ballSeal1]);
    }

    if (ballSeal2)
    {
        LoadSpriteSheet(&sBallSealSpriteSheets[ballSeal2]);
        LoadSpritePalette(&sBallSealPalettes[ballSeal2]);
    }

    if (ballSeal3)
    {
        LoadSpriteSheet(&sBallSealSpriteSheets[ballSeal3]);
        LoadSpritePalette(&sBallSealPalettes[ballSeal3]);
    }

    if (ballSeal4)
    {
        LoadSpriteSheet(&sBallSealSpriteSheets[ballSeal4]);
        LoadSpritePalette(&sBallSealPalettes[ballSeal4]);
    }
}

static const u8 sStaticIconSealOffsets_Single[][2] =
{
    { 0, 0 },
};

static const u8 sStaticIconSealOffsets_Double[][2] =
{
    { -18, 0 },
    {  18, 0 },
};

static const u8 sStaticIconSealOffsets_Triple[][2] =
{
    { -18, -32 },
    {  18, -32 },
    {  0, 0 },
};

static const u8 sStaticIconSealOffsets_Quad[][2] =
{
    { -18, -32 },
    {  18, -32 },
    { -18, 0 },
    {  18, 0 },
};

static u8 GetStaticIconSealOffsets(u8 ballSeal1, u8 ballSeal2, u8 ballSeal3, u8 ballSeal4)
{
    int numStaticIcons = 0;
    if (sBallSealTypes[ballSeal1] & BALL_SEAL_TYPE_STATIC_ICON)
        numStaticIcons++;
    if (sBallSealTypes[ballSeal2] & BALL_SEAL_TYPE_STATIC_ICON)
        numStaticIcons++;
    if (sBallSealTypes[ballSeal3] & BALL_SEAL_TYPE_STATIC_ICON)
        numStaticIcons++;
    if (sBallSealTypes[ballSeal4] & BALL_SEAL_TYPE_STATIC_ICON)
        numStaticIcons++;

    return numStaticIcons;
}

void StartBallSealAnimation(u8 ballSeal1, u8 ballSeal2, u8 ballSeal3, u8 ballSeal4, u8 x, u8 y, u8 priority, u8 subpriority, int battlerSide)
{
    u8 taskId;
    int i;
    u8 ballSeals[] = {ballSeal1, ballSeal2, ballSeal3, ballSeal4};
    int numBallSeals = 0;
    int curStaticIcon = 0;
    u8 numStaticIcons = GetStaticIconSealOffsets(ballSeal1, ballSeal2, ballSeal3, ballSeal4);

    for (i = 0; i < 4; i++)
    {
        u8 ballSeal = ballSeals[i];

        if (ballSeal)
        {
            u8 sealX = x;
            u8 sealY = y;
            numBallSeals++;
            if (sBallSealTypes[ballSeal] & BALL_SEAL_TYPE_STATIC_ICON)
            {
                switch (numStaticIcons)
                {
                case 1:
                    sealX += sStaticIconSealOffsets_Single[curStaticIcon][0];
                    sealY += sStaticIconSealOffsets_Single[curStaticIcon][1];
                    break;
                case 2:
                    sealX += sStaticIconSealOffsets_Double[curStaticIcon][0];
                    sealY += sStaticIconSealOffsets_Double[curStaticIcon][1];
                    break;
                case 3:
                    sealX += sStaticIconSealOffsets_Triple[curStaticIcon][0];
                    sealY += sStaticIconSealOffsets_Triple[curStaticIcon][1];
                    break;
                case 4:
                    sealX += sStaticIconSealOffsets_Quad[curStaticIcon][0];
                    sealY += sStaticIconSealOffsets_Quad[curStaticIcon][1];
                    break;
                }

                curStaticIcon++;
            }

            sCreateBallSealParticleFuncs[ballSeal](sealX, sealY, priority, subpriority + i, battlerSide);
        }
    }

    if (numBallSeals > 0)
    {
        taskId = CreateTask(CleanupBallSealAnimation, 5);
        gTasks[taskId].data[0] = ballSeal1;
        gTasks[taskId].data[1] = ballSeal2;
        gTasks[taskId].data[2] = ballSeal3;
        gTasks[taskId].data[3] = ballSeal4;
        gTasks[taskId].data[4] = 0;
    }
}

static void CleanupBallSealAnimation(u8 taskId)
{
    if (gTasks[taskId].data[4]++ == BALL_SEAL_DURATION)
    {
        FreeBallSealGraphics(gTasks[taskId].data[0]);
        FreeBallSealGraphics(gTasks[taskId].data[1]);
        FreeBallSealGraphics(gTasks[taskId].data[2]);
        FreeBallSealGraphics(gTasks[taskId].data[3]);
        DestroyTask(taskId);
    }
}

static void FreeBallSealGraphics(u8 ballSeal)
{
    if (!ballSeal)
        return;

    FreeSpriteTilesByTag(sBallSealSpriteSheets[ballSeal].tag);
    FreeSpritePaletteByTag(sBallSealPalettes[ballSeal].tag);
}

/*
Heart Cloud Small Pink
A simple upward plume of hearts.
*/
static void CreateBallSealParticles_HeartCloudSmallPink(u8 x, u8 y, u8 priority, u8 subpriority, int battlerSide)
{
    int i;
    u8 spriteId;

    for (i = 0; i < 10; i++)
    {
        spriteId = CreateSprite(&gBallSealSpriteTemplate_HeartCloudSmallPink, x, y, subpriority);
        gSprites[spriteId].oam.priority = priority;
        gSprites[spriteId].data[0] = 96 + (Random() % 64);
        gSprites[spriteId].data[1] = 0;
        gSprites[spriteId].data[2] = 0x180 + Random() % 0x100;
    }
}

static void BallSeal_HeartCloudSmallPink_Step0(struct Sprite *sprite)
{
    if (++sprite->data[3] == BALL_SEAL_DURATION)
    {
        DestroySprite(sprite);
    }
    else
    {
        sprite->x2 = Sin(sprite->data[0], (sprite->data[1] >> 8));
        sprite->y2 = Cos(sprite->data[0], (sprite->data[1] >> 8));
        sprite->data[1] += sprite->data[2];
        sprite->data[2] -= 5;
    }
}

/*
Heart Spin Small Pink
3 Hearts move in a circle around the mon. They scale larger
on the way up, and shrink on the way down.
*/
static void CreateBallSealParticles_HeartSpinSmallPink_Delay(u8 taskId)
{
    u8 spriteId;

    if (++gTasks[taskId].data[0] == gTasks[taskId].data[1])
    {
        gTasks[taskId].data[0] = 0;

        spriteId = CreateSprite(&gBallSealSpriteTemplate_HeartSpinSmallPink, gTasks[taskId].data[3], gTasks[taskId].data[4] - 32, gTasks[taskId].data[6]);
        SetSpriteRotScale(spriteId, 0x100, 0x100, 0);
        gSprites[spriteId].oam.priority = gTasks[taskId].data[5];
        gSprites[spriteId].data[0] = 32;
        gSprites[spriteId].data[1] = 0x40;
        gSprites[spriteId].data[2] = 7;
        gSprites[spriteId].data[3] = 0;
        gSprites[spriteId].data[4] = gTasks[taskId].data[7];
        gSprites[spriteId].data[7] = spriteId;
        BallSeal_HeartSpinSmallPink_Step0(&gSprites[spriteId]);

        if (--gTasks[taskId].data[2] == 0)
            DestroyTask(taskId);
    }
}

static void CreateBallSealParticles_HeartSpinSmallPink(u8 x, u8 y, u8 priority, u8 subpriority, int battlerSide)
{
    u8 taskId = CreateTask(CreateBallSealParticles_HeartSpinSmallPink_Delay, 5);
    gTasks[taskId].data[0] = 0; // Delay counter
    gTasks[taskId].data[1] = 5; // Number of frames to delay between sprites
    gTasks[taskId].data[2] = 3; // Number of sprites to create
    gTasks[taskId].data[3] = x;
    gTasks[taskId].data[4] = y;
    gTasks[taskId].data[5] = priority;
    gTasks[taskId].data[6] = subpriority;
    gTasks[taskId].data[7] = battlerSide;
}

static void BallSeal_HeartSpinSmallPink_Step0(struct Sprite *sprite)
{
    int scale;

    if (++sprite->data[3] > 37)
    {
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
    else
    {
        sprite->x2 = Cos(sprite->data[1], sprite->data[0]);
        sprite->y2 = Sin(sprite->data[1], sprite->data[0]);
        scale = 0x80 + abs(0x80 - (sprite->data[1] - 0x40));
        SetSpriteRotScale(sprite->data[7], scale, scale, 0);

        if (sprite->data[4] == B_SIDE_PLAYER)
            sprite->data[1] += sprite->data[2];
        else
            sprite->data[1] -= sprite->data[2];

        sprite->data[1] &= 0xFF;
    }
}

/*
Heart Circle Small Pink
A circle of hearts expands and spins.
*/
static void CreateBallSealParticles_HeartCircleSmallPink(u8 x, u8 y, u8 priority, u8 subpriority, int battlerSide)
{
    int i;
    u8 spriteId;
    int numHearts = 10;

    for (i = 0; i < numHearts; i++)
    {
        spriteId = CreateSprite(&gBallSealSpriteTemplate_HeartCircleSmallPink, x, y, subpriority);
        gSprites[spriteId].oam.priority = priority;
        gSprites[spriteId].data[0] = (256 / numHearts * i) << 8; // Angle
        gSprites[spriteId].data[1] = 0x380; // Angle delta
        gSprites[spriteId].data[2] = 0x000; // Radius
        gSprites[spriteId].data[3] = 0x150; // Radius delta
        gSprites[spriteId].data[4] = 0; // Counter
        gSprites[spriteId].data[5] = battlerSide; // Side
    }
}

static void BallSeal_HeartCircleSmallPink_Step0(struct Sprite *sprite)
{
    if (++sprite->data[4] == BALL_SEAL_DURATION)
    {
        DestroySprite(sprite);
    }
    else
    {
        u16 angle = (u16)sprite->data[0] >> 8;
        u16 radius = sprite->data[2] >> 8;
        sprite->x2 = Sin(angle, radius);
        sprite->y2 = Cos(angle, radius);

        if (sprite->data[5] == B_SIDE_PLAYER)
            sprite->data[0] += sprite->data[1];
        else
            sprite->data[0] -= sprite->data[1];

        sprite->data[2] += sprite->data[3];
    }
}

/*
Letter A
The letter "A". Slightly moves upwards and changes opacity.
*/
static void CreateBallSealParticles_LetterA(u8 x, u8 y, u8 priority, u8 subpriority, int battlerSide)
{
    u8 spriteId = CreateSprite(&gBallSealSpriteTemplate_LetterA, x, y, subpriority);
    gSprites[spriteId].oam.priority = priority;
    gSprites[spriteId].data[0] = 0;
    gSprites[spriteId].data[1] = 0; // y offset
    gSprites[spriteId].data[2] = 0;
    gSprites[spriteId].data[3] = 0x880; // scale
    gSprites[spriteId].data[7] = spriteId;
}

static void BallSeal_StaticLetter_Step0(struct Sprite *sprite)
{
    if (++sprite->data[0] == BALL_SEAL_DURATION - 1)
    {
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
    else
    {
        int slideDuration = 8;
        int slidePixels = 20;
        if (sprite->data[0] < slideDuration)
        {
            sprite->data[1] += (slidePixels << 8) / slideDuration;
            sprite->data[2] += 2;
            sprite->data[3] -= 0x100;
        }
        else if (sprite->data[0] < BALL_SEAL_DURATION - slideDuration)
        {
            sprite->data[1] += 0x020;
            sprite->data[2] = 16;
            sprite->data[3] = 0x100;
        }
        else
        {
            sprite->data[1] += (slidePixels << 8) / slideDuration;
            sprite->data[2] -= 2;
            sprite->data[3] += 0x100;
        }

        SetSpriteRotScale(sprite->data[7], sprite->data[3], sprite->data[3], 0);
        sprite->y2 = -((u16)sprite->data[1] >> 8);
    }
}

/*
Orbs Rainbow
A burst of orbs that change colors over time.
*/
static void CreateBallSealParticles_OrbsRainbow(u8 x, u8 y, u8 priority, u8 subpriority, int battlerSide)
{
    int i;
    u8 spriteId;
    u8 taskId = 0xFF;
    int numOrbs = 5;

    for (i = 0; i < numOrbs; i++)
    {
        spriteId = CreateSprite(&gBallSealSpriteTemplate_OrbsRainbow, x, y, subpriority);
        if (spriteId != MAX_SPRITES)
        {
            gSprites[spriteId].oam.priority = priority;
            gSprites[spriteId].data[0] = (256 / numOrbs * i) << 8; // Angle
            gSprites[spriteId].data[1] = 0; // x
            gSprites[spriteId].data[2] = (Random() & 0x1FF) - 0x100; // x delta
            gSprites[spriteId].data[3] = 0; // y
            gSprites[spriteId].data[4] = (Random() % 0x200) + 0xFC00; // y delta
            gSprites[spriteId].data[5] = 0x20; // y acceleration
            gSprites[spriteId].data[6] = 0; // Counter

            if (taskId == 0xFF)
            {
                taskId = CreateTask(BallSeal_OrbsRainbow_BlendTask, 3);
                gTasks[taskId].data[0] = 0;
                gTasks[taskId].data[1] = gSprites[spriteId].oam.paletteNum;
            }
        }
    }
}

static u16 HsvToRgb(u8 hue, u8 saturation, u8 value)
{
    u8 remainder, region;
    u8 p, q, t;
    u32 red, green, blue;

    if (saturation == 0)
        return RGB(value, value, value);

    region = hue / 43;
    remainder = (hue - (region * 43)) * 6;

    p = (value * (255 - saturation)) >> 8;
    q = (value * (255 - ((saturation * remainder) >> 8))) >> 8;
    t = (value * (255 - ((saturation * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
    case 0:
        red = value;
        green = t;
        blue = p;
        break;
    case 1:
        red = q;
        green = value;
        blue = p;
        break;
    case 2:
        red = p;
        green = value;
        blue = t;
        break;
    case 3:
        red = p;
        green = q;
        blue = value;
        break;
    case 4:
        red = t;
        green = p;
        blue = value;
        break;
    default:
        red = value;
        green = p;
        blue = q;
        break;
    }

    return RGB(red >> 3, green >> 3, blue >> 3);
}

static u16 BallSeal_OrbsRainbow_GetBlendColor(int index)
{
    return HsvToRgb((index - 1) * 5, 255, 255);
}

static void BallSeal_OrbsRainbow_Step0(struct Sprite *sprite)
{
    if (++sprite->data[6] == BALL_SEAL_DURATION)
    {
        DestroySprite(sprite);
    }
    else
    {
        sprite->x2 = (s8)(sprite->data[1] >> 8);
        sprite->y2 = (s8)(sprite->data[3] >> 8);
        sprite->data[1] += sprite->data[2];
        sprite->data[3] += sprite->data[4];
        sprite->data[4] += sprite->data[5];
    }
}

static void BallSeal_OrbsRainbow_BlendTask(u8 taskId)
{
    if (++gTasks[taskId].data[0] == BALL_SEAL_DURATION)
    {
        DestroyTask(taskId);
    }
    else
    {
        u16 blendColor = BallSeal_OrbsRainbow_GetBlendColor(gTasks[taskId].data[0]);
        BlendPalette(0x100 + 16 * gTasks[taskId].data[1], 16, 8, blendColor);
    }
}
