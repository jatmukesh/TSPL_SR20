echo on
cls
cd  %~dp0
del SR20_2kjm.bin
del SR20.bin
del SR20_ALL.bin
::生成boot bin
hex2bin.exe %~dp0boot\SR20_Bootloader.hex 1000200

::生成app bin
hex2bin.exe %~dp0app\SR20.hex 1010000

::拷贝app bin
copy  %~dp0app\SR20.bin  %~dp0SR20.bin 

::合并boot与app
binJoin.exe %~dp0boot\SR20_Bootloader.bin 1000200 %~dp0app\SR20.bin 1010000 SR20_ALL.bin