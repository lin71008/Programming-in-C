/*******************************************************************************
    Reference: https://new-acos.blogspot.com/2010/02/linuxprocstatcpu.html
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>

void stat()
{
    FILE *fp = NULL;
    char buffer[256] = {0};

    static int64_t usr,      sys,      nie,      idle,
                   iow,      irq,      sirq,      stead;
    static int64_t usr_sav,  sys_sav,  nie_sav,  idle_sav,
                   iow_sav,  irq_sav,  sirq_sav,  stead_sav;
    static int64_t usr_frme, sys_frme, nie_frme, idle_frme,
                   iow_frme, irq_frme, sirq_frme, stead_frme;

    if ((fp = fopen("/proc/stat", "r")) == NULL) return;
    while(!feof(fp))
    {
        size_t i = fread(buffer, 1, 255, fp);
        if (i && strstr(buffer, "cpu"))
        {
            sscanf(buffer, "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
                   &usr, &nie, &sys, &idle, &iow, &irq, &sirq, &stead);
            break;
        }
    }
    fclose(fp);

    static int64_t total_frme;
    usr_frme   = usr   - usr_sav;
    sys_frme   = sys   - sys_sav;
    nie_frme   = nie   - nie_sav;
    idle_frme  = idle  - idle_sav;
    iow_frme   = iow   - iow_sav;
    irq_frme   = irq   - irq_sav;
    sirq_frme  = sirq  - sirq_sav;
    stead_frme = stead - stead_sav;
    total_frme = usr_frme + sys_frme + nie_frme + idle_frme + iow_frme + irq_frme + sirq_frme + stead_frme;
    if (total_frme < 1) total_frme = 1;

    float scale = 100.0 / ((float) total_frme);
    printf("CPU usage: %6.2f%%\n", 100.0 - idle_frme * scale);

    usr_sav   = usr;
    sys_sav   = sys;
    nie_sav   = nie;
    idle_sav  = idle;
    iow_sav   = iow;
    irq_sav   = irq;
    sirq_sav  = sirq;
    stead_sav = stead;
}

int main()
{
    signal(SIGALRM, stat);
    struct itimerval tick = {    .it_value.tv_sec = 1,    .it_value.tv_usec = 0,
                              .it_interval.tv_sec = 1, .it_interval.tv_usec = 0};
    while (!!setitimer(ITIMER_REAL, &tick, NULL))
    {
        /* Wait until SIGINT. OwO */
    }
    getchar();
    return 0;
}
