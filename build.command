# change directory to that of this .command file
# should be ~/Desktop/Development/gbadev/___
cd `dirname $0`

# if there's a main.gba file already here, delete it,
# this way OpenEmu won't be launched if build fails
# todo: might want to trash this instead or back it up, 
# or check date/time modified instead
[ -e main.gba ] && rm main.gba

# ${PWD##*/} resolves to the current directory name, not its absolute path
# -lm flag is to link math library
"/Applications/VMWare Fusion.app/Contents/Library/vmrun" -T fusion -gu Owner -gp "" \
runScriptInGuest ~/"Documents/Virtual Machines.localized/Windows XP Home Edition.vmwarevm/Windows XP Home Edition.vmx" "" \
"
    pushd \"\\\\vmware-host\\Shared Folders\\gbadev\\${PWD##*/}\"
    path=C:\\devkitadv\\bin
    gcc -o main.elf main.c gfx.c -lm
    objcopy -O binary main.elf main.gba
    del main.elf
    ::pause
"

[ -e main.gba ] && open /Applications/OpenEmu.app main.gba