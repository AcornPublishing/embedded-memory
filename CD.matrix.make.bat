@ECHO OFF
ECHO === packing CD files ===
IF	EXIST cd_optimize_I.zip	DEL	cd_optimize_I.zip

pkzip	-add	-maximum	-directories	cd_optimize_I.zip *.*
