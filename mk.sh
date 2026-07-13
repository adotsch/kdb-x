# This script builds the zig-builder docker image and runs make inside the container to build everything.
docker build -t zig-builder .
MAKE="docker run --rm -v $PWD:/app --user $(id -u):$(id -g) zig-builder make -C"
$MAKE bit $@
$MAKE selectk $@
$MAKE udp $@
$MAKE flock $@
