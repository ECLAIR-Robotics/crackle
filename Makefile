docker-mac:
	- docker run -it --rm -e DISPLAY=host.docker.internal:0 --privileged -v  /Users/kevinbabu/Documents/ECLAIR/crackle:/app crackle:humble

# edit the IP here for other computers
docker:
	- docker run -it --rm -e DISPLAY=host.docker.internal:0 --privileged -v  /Users/kevinbabu/Documents/ECLAIR/crackle:/app crackle:humble