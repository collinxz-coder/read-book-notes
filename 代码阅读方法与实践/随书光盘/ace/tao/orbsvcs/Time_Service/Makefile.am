##---------------------------------------------------------------------------
##       Makefile.am,v 1.2 1999/04/07 20:53:27 othman Exp
##
##       Makefile for TAO's Time Service
##
##---------------------------------------------------------------------------

##
##  Process this file with automake to create Makefile.in
##

## The number in AUTOMAKE_OPTIONS is the minimum required version automake
## needed to process this file.
AUTOMAKE_OPTIONS = 1.4

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

bin_PROGRAMS = Time_Service_Server Time_Service_Clerk

Time_Service_Server_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

Time_Service_Server_SOURCES = \
        IR_Helper.cpp \
        Server_i.cpp \
        Time_Service_Server.cpp

Time_Service_Clerk_LDADD = \
	$(top_builddir)/ace/libACE.la $(top_builddir)/TAO/tao/libTAO.la

Time_Service_Clerk_SOURCES = \
        Clerk_i.cpp \
        Time_Service_Clerk.cpp

noinst_HEADERS = \
        Clerk_i.h \
        IR_Helper.h \
        Server_i.h

EXTRA_DIST = README

## Clean up some additional files/directories possibly created during
## the configure script tests.
clean-local:
	-rm -f *.bak *.rpo *.sym lib*.*_pure_* Makefile.old core
	-rm -rf ptrepository Templates.DB gcctemp.c gcctemp so_locations

