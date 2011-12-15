AUTOMAKE_OPTIONS = foreign dist-bzip2 no-dist-gzip 1.10
ACLOCAL_AMFLAGS = -I m4

#SUBDIRS = include auxlib src utilities target-sh samples robot-params
SUBDIRS = include auxlib src target-sh samples

#EXTRA_DIST = README.ja

# do "ldconfig" after "make install"
install-exec-hook : 
	@LDCONFIG@ $(libdir)

