#!/bin/bash

container_id=$(docker ps -lq)
if [ -z "$container_id" ]; then
  container_name="batata"  # Fallback to 'batata' if no container is running
else
  container_name=$(docker inspect --format '{{.Name}}' "$container_id" | sed 's/\///g')
fi

docker run -it --rm irc-client weechat -r "/server add bestserver $(ip -4 addr show docker0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')/6667 -notls; /set irc.server_default.nicks ${container_name},${container_name}2,${container_name}3,${container_name}4,${container_name}5"
#docker run -it --rm irc-client weechat -r "/server add bestserver $(ip -4 addr show docker0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')/6667 -password=123456 -notls; /set irc.server_default.nicks ${container_name},${container_name}2,${container_name}3,${container_name}4,${container_name}5"

