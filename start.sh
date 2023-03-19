NAME=$(pwd | grep -o "[^\/]*$")
make
st-flash --reset write build/$NAME.bin 0x08000000
