
@ECHO OFF

make clean

IF "%1"=="snes" (
make GAMEPAD="SNES"
copy .\gamepad.hex ..\hex\ps3_gamepad_snes.hex /y
)
IF "%1"=="saturn" (
make GAMEPAD="SATURN"
copy .\gamepad.hex ..\hex\ps3_gamepad_saturn.hex /y
)

