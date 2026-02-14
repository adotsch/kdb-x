for p in linux arm64 win64; do \
    docker run -it --rm --user $(id -u):$(id -g) -v $(pwd):/workdir -e CROSS_TRIPLE=$p multiarch/crossbuild make -C bit; \
done
