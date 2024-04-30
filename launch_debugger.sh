export CONTAINER_NAME="gdb_container:latest"
docker build -f debug.dockerfile -t $CONTAINER_NAME . 
docker run --name="debugger" --rm -it $CONTAINER_NAME