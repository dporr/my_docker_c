FROM n0madic/alpine-gcc:9.2.0

RUN apk add --no-cache --upgrade 'curl>=7.66'
RUN apk add --no-cache --upgrade 'curl-dev>=7.66'

# Download docker-explorer
ARG docker_explorer_version=v18
RUN curl --fail -Lo /usr/local/bin/docker-explorer https://github.com/codecrafters-io/docker-explorer/releases/download/${docker_explorer_version}/${docker_explorer_version}_linux_amd64
RUN chmod +x /usr/local/bin/docker-explorer
RUN apk update && apk add gdb

COPY . /app
WORKDIR /app

RUN sed -i -e 's/\r$//' /app/your_docker.sh
RUN chmod +x /app/*.sh
ENTRYPOINT ["/app/debug.sh"]
