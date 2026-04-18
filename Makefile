# BlocksDS starter project for an ARM9-only Nintendo DS game.
# Keep all source/assets inside this repo folder. BlocksDS makefiles do not
# support using .. to reach outside the project tree.

NAME        := crash_mage_proto
GAME_TITLE  := Crash Mage Trial
GAME_SUBTITLE := One-level starter build
GAME_AUTHOR := Crash

SOURCEDIRS  := source
INCLUDEDIRS := source
LIBDIRS     :=
LIBS        := -lnds9

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
