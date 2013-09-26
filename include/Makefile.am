AUTOMAKE_OPTIONS = foreign 1.10

include_HEADERS = ypspur.h ypspur-md.h ypparam.h
#pkginclude_HEADERS = yp-spur.h

noinst_HEADERS = ypspur-coordinator.h control.h param.h shvel-param.h \
                 command.h serial.h ypprotocol.h odometry.h utility.h \
		 ssm_spur_handler.h communication.h adinput.h \
		 ipcommunication.h yprintf.h msq.win32.h

nobase_include_HEADERS = ssmtype/pws-motor.h ssmtype/spur-odometry.h ssmtype/ypspur-ad.h


# make symbolic link after "make install"
install-data-hook : 
	if [ -f ${includedir}/yp-spur.h ]; then rm ${includedir}/yp-spur.h; fi;
	ln -s ${includedir}/ypspur.h ${includedir}/yp-spur.h

uninstall-hook :
	if [ -L ${includedir}/yp-spur.h ]; then rm ${includedir}/yp-spur.h; fi;

