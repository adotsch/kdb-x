docker build -t mmamba -f Dockerfile.mmamba .
docker run -it --rm --user $(id -u):$(id -g) -v $PWD:/app mmamba
