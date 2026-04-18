# Crash Mage Trial — BlocksDS starter repo

This is a tiny **ARM9-only Nintendo DS starter project** for BlocksDS.
It builds a playable one-level prototype with:

- D-pad movement
- A button spell casting
- B button restart
- a portal goal (`P`)
- a chasing rune enemy (`R`)
- no art pipeline yet, so it is easy to edit from an iPhone

## Why this structure

BlocksDS officially recommends starting from a template or example, and its `rom_arm9` template is the one that lets you focus on the ARM9 side while using the default ARM7 core. BlocksDS also notes that project paths referenced by the Makefile must stay **inside the project folder**, so this repo keeps everything self-contained. citeturn942638view1turn306488view2

This Makefile uses BlocksDS' current default ARM9 makefile include style:

```make
include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
```

That pattern is in current public BlocksDS projects, and BlocksDS' current docs still describe starting from templates/examples and customizing the title/icon in the Makefile. citeturn719334search7turn306488view2

## Files

- `.devcontainer/devcontainer.json` — lets GitHub Codespaces boot into the official BlocksDS container image
- `Makefile` — starter build config
- `source/main.c` — one-level mage prototype
- `.gitignore`

## iPhone workflow

1. Create a new GitHub repo.
2. Upload these files.
3. Open the repo in Safari.
4. Start a GitHub Codespace.
5. In the Codespace terminal, run:

```bash
make
```

GitHub Codespaces supports a `.devcontainer/devcontainer.json` file in the repo root's `.devcontainer` directory, and it can reference an existing image directly with the `image` property. citeturn942638view2

## Result

A successful build should produce an `.nds` ROM you can download and test in a DS emulator. BlocksDS' docs describe `make` producing an NDS ROM, and recommend melonDS, no$gba, and DeSmuMe for testing. citeturn942638view0turn306488view3

## Next upgrades

After this first build works, the clean progression is:

1. swap ASCII map tiles for sprite graphics
2. put UI on one screen and gameplay on the other
3. add touch controls
4. add XP, enemies, and a boss room
5. move from console rendering to proper tile/sprite rendering

BlocksDS' input docs note that `scanKeys()` is used before `keysDown()` / `keysHeld()`, and before reading touch input, which is the pattern used in this starter. citeturn737359search0turn737359search3
