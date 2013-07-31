all: clean init DO_THINGS

DO_THINGS:
	cd src && make;
	@echo -e '\033[44;36;1m All things go to ./output now, Have Fun!!! \033[0m'  
init:
	@echo -e '\033[44;36;1m Starting... \033[0m'  
clean:
	cd src && make clean;
