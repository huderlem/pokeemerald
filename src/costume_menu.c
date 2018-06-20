#include "global.h"
#include "main.h"
#include "bg.h"
//#include "costume_menu.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "constants/songs.h"

static const u8 gGraphics_CostumeScreenBackground[] = INCBIN_U8("graphics/costume_screen/background.4bpp");
static const u16 gPalette_CostumeScreenBackground[] = INCBIN_U16("graphics/costume_screen/background.gbapal");
