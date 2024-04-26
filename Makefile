# edit the IP here for other computers
docker:
	- docker run -it --rm -e DISPLAY=host.docker.internal:0 --privileged -v  C:\eclair_projects\crackle:/app crackle:humble