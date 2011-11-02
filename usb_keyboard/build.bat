
@ECHO OFF

IF "%1"=="snes" (
make clean
make GAMEPAD="SNES" PLAYER="MAME_P1"
copy .\keyboard.hex ..\hex\usb_keyboard_snes_p1.hex /y
make clean
make GAMEPAD="SNES" PLAYER="MAME_P2"
copy .\keyboard.hex ..\hex\usb_keyboard_snes_p2.hex /y
)
IF "%1"=="saturn" (
make clean
make GAMEPAD="SATURN" PLAYER="MAME_P1"
copy .\keyboard.hex ..\hex\usb_keyboard_saturn_p1.hex /y
make clean
make GAMEPAD="SATURN" PLAYER="MAME_P2"
copy .\keyboard.hex ..\hex\usb_keyboard_saturn_p2.hex /y
)

