

              CC = arm-linux-gnueabi-gcc

          CFLAGS = -D_POSIX_C_SOURCE=199309L -std=gnu99 -D_REENTRANT -D_POSIX_TIMERS

              LD = arm-linux-gnueabi-gcc

         LDFLAGS = -lrt


mssd:		matrix_status_monitor_deamon.o
		$(LD) $(LDFLAGS) matrix_status_monitor_deamon.o -o mssd


clean:
		rm mssd matrix_status_monitor_deamon.o
