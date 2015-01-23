/*
     Author : Hans Kramer

       Date : Jan 2015

 */

#include <stdio.h>
#include <syslog.h>
#include <time.h>

#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/statvfs.h>


void log_load_avg() {
    FILE *fp_up = fopen("/proc/loadavg", "r");
    
    if (fp_up) {
        char buffer[2048];
        if (fgets(buffer, 2048, fp_up)) 
            syslog(LOG_INFO, "%s", buffer);
        fclose(fp_up);
    }
}


void log_uptime() {
    FILE *fp_up = fopen("/proc/uptime", "r");
    
    if (fp_up) {
        char buffer[2048];
        if (fgets(buffer, 2048, fp_up)) 
            syslog(LOG_INFO, "%s", buffer);
        fclose(fp_up);
    }
}


void log_df() {
    struct statvfs buf;
    statvfs("/media/mmc", &buf);

    syslog(LOG_INFO, "/media/mmc %ld %ld", buf.f_bavail, buf.f_ffree);

    statvfs("/", &buf);
    syslog(LOG_INFO, "/ %ld %ld", buf.f_bavail, buf.f_ffree);
}


void handler(union sigval sigval) {
    syslog(LOG_INFO, "timer_nr=%02d  pid=%d  pthread_self=%ld\n", sigval.sival_int, getpid(), pthread_self());
    log_load_avg();
    log_uptime();
    log_df();
}


int main() {
    struct sigevent  event;
    timer_t          timerid;

    memset(&event, 0, sizeof (struct sigevent));
    event.sigev_notify            = SIGEV_THREAD;
    event.sigev_notify_attributes = NULL;
    event.sigev_notify_function   = handler;
    
    timer_create(CLOCK_REALTIME, &event, &timerid);
    // error checking

    struct itimerspec itimer = { { 10, 0 }, { 10, 0 } };

    timer_settime(timerid, 0, &itimer, NULL);
    // error checking

    while (1)
        sleep(10000000);
}

