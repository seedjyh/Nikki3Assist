awk -F[\'] '/^ /{printf "ADD-ITEM-INFO: NAME=(\"%s\"|\"%s\"), ID_IN_GAME=%d;\n", $2, $4, $6}'  wardrobe.js  > item_info.mml
