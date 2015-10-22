CCGI_DIST_URL=http://skylink.dl.sourceforge.net/project/libccgi/libccgi/libccgi%201.2/ccgi-1.2.tgz

.PHONY: ccgi

ccgi: libccgi.a

libccgi.a: src/Makefile $(wildcard src/*.c) $(wildcard/*.h)
	make -C src
	cp src/$(@) src/*.h .

src/Makefile: ccgi.tar.gz
	mkdir -p src
	cd src && tar --strip-components=1 -zxvf $(abspath $<)

ccgi.tar.gz:
	wget $(CCGI_DIST_URL) -O ccgi.tar.gz
