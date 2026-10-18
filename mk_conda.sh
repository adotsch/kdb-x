docker build -t mmamba -f Dockerfile.mmamba .
docker run --rm --user $(id -u):$(id -g) -v $PWD:/app -w /app/recipe mmamba rattler-build build "$@"
