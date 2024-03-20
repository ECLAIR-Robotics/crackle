docker:
	- docker run -it --rm --expose 5920 --device=/dev/video0 --privileged -v  C:\eclair_projects\crackle:/app crackle:humble