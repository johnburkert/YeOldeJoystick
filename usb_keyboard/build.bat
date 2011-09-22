
@ECHO OFF

make clean

IF "%1"=="snes" (
make GAMEPAD="SNES"
copy .\keyboard.hex ..\hex\usb_keyboard_snes.hex /y
)
IF "%1"=="saturn" (
make GAMEPAD="SATURN"
copy .\keyboard.hex ..\hex\usb_keyboard_saturn.hex /y
)

