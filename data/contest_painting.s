@ the third big chunk of data

	.include "asm/macros.inc"
	.include "constants/constants.inc"

	.section .rodata
	.align 2, 0

gUnknown_085A989C:: @ 85A989C
	.incbin "baserom.gba", 0x5a989c, 0x200

gUnknown_085A9A9C:: @ 85A9A9C
	.incbin "baserom.gba", 0x5a9a9c, 0x1084

gUnknown_085AAB20:: @ 85AAB20
	.incbin "baserom.gba", 0x5aab20, 0xc30

gUnknown_085AB750:: @ 85AB750
	.incbin "baserom.gba", 0x5ab750, 0xb38

gUnknown_085AC288:: @ 85AC288
	.incbin "baserom.gba", 0x5ac288, 0xfb8

gUnknown_085AD240:: @ 85AD240
	.incbin "baserom.gba", 0x5ad240, 0x1130

gUnknown_085AE370:: @ 85AE370
	.incbin "baserom.gba", 0x5ae370, 0x604

gUnknown_085AE974:: @ 85AE974
	.incbin "baserom.gba", 0x5ae974, 0x50c

gUnknown_085AEE80:: @ 85AEE80
	.incbin "baserom.gba", 0x5aee80, 0x50c

gUnknown_085AF38C:: @ 85AF38C
	.incbin "baserom.gba", 0x5af38c, 0x50c

gUnknown_085AF898:: @ 85AF898
	.incbin "baserom.gba", 0x5af898, 0x50c

gUnknown_085AFDA4:: @ 85AFDA4
	.incbin "baserom.gba", 0x5afda4, 0x50c

gUnknown_085B02B0:: @ 85B02B0
	.incbin "baserom.gba", 0x5b02b0, 0x524

gContestRankTextPointers:: @ 85B07D4
	.4byte gContestRankNormal
	.4byte gContestRankSuper
	.4byte gContestRankHyper
	.4byte gContestRankMaster
	.4byte gContestLink

gUnknown_085B07E8:: @ 85B07E8
	.incbin "baserom.gba", 0x5b07e8, 0x4

gUnknown_085B07EC:: @ 85B07EC
	.incbin "baserom.gba", 0x5b07ec, 0x8

gContestPaintingDescriptionPointers:: @ 85B07F4
	.4byte gContestPaintingCool1
	.4byte gContestPaintingCool2
	.4byte gContestPaintingCool3
	.4byte gContestPaintingBeauty1
	.4byte gContestPaintingBeauty2
	.4byte gContestPaintingBeauty3
	.4byte gContestPaintingCute1
	.4byte gContestPaintingCute2
	.4byte gContestPaintingCute3
	.4byte gContestPaintingSmart1
	.4byte gContestPaintingSmart2
	.4byte gContestPaintingSmart3
	.4byte gContestPaintingTough1
	.4byte gContestPaintingTough2
	.4byte gContestPaintingTough3

gUnknown_085B0830:: @ 85B0830
	.4byte 0xc0003000, 0x00000000

gUnknown_085B0838:: @ 85B0838
	.incbin "baserom.gba", 0x5b0838, 0x4
