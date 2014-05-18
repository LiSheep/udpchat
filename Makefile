server:
	cd src/server && $(MAKE) $@
client:
	cd src/client && $(MAKE) $@
.PHONY : clean
clean:
	cd src/server && $(MAKE) clean
	cd src/client && $(MAKE) clean
