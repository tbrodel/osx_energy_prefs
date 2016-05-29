#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ext.h"
#include "ext_obex.h"

#define HOURS     23
#define MINUTES   59
#define NULL_TERM 1
#define NUM_TIMES 2
#define TIME_LEN  8

typedef struct _osx_energy_prefs {
        t_object ob;
        long start_hour;
        long start_min;
        long end_hour;
        long end_min;
} t_osx_energy_prefs;

int check_time(long, long);
void ext_main(void *);
void *osx_energy_prefs_new();
int power(t_osx_energy_prefs *);
void set_end_hour(t_osx_energy_prefs *, long);
void set_end_min(t_osx_energy_prefs *, long);
void set_start_hour(t_osx_energy_prefs *, long);
void set_start_min(t_osx_energy_prefs *, long);

static t_class *s_osx_energy_prefs_class;

int
check_time(long check, long max)
{
        if (check > max) {
                error("Invalid time: %02ld > %02ld", check, max);
                return EXIT_FAILURE;
        }
    
        if (check < 0) {
                error("Invalid time: must be positive integer!");
                return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
}

void
ext_main(void *r)
{
        t_class *c;
    
        c = class_new("osx_energy_prefs", (method)osx_energy_prefs_new, 
            (method)NULL, sizeof(t_osx_energy_prefs), 0L, 0);
        class_addmethod(c, (method)power, "bang", 0);
        class_addmethod(c, (method)set_start_hour, "in1", A_LONG, 0);
        class_addmethod(c, (method)set_start_min, "in2", A_LONG, 0);
        class_addmethod(c, (method)set_end_hour, "in3", A_LONG, 0);
        class_addmethod(c, (method)set_end_min, "in4", A_LONG, 0);
    
    
        class_register(CLASS_BOX, c);
    
        s_osx_energy_prefs_class = c;
}

void *
osx_energy_prefs_new()
{
        t_osx_energy_prefs *x =
            (t_osx_energy_prefs *)object_alloc(s_osx_energy_prefs_class);
        intin(x, 4);
        intin(x, 3);
        intin(x, 2);
        intin(x, 1);
    
        x->start_hour = 0;
        x->start_min = 0;
        x->end_hour = 0;
        x->end_min = 0;
    
        return (void *)x;
}

int
power(t_osx_energy_prefs *x)
{
        FILE *fp;
        char times[NUM_TIMES][TIME_LEN + NULL_TERM];
    
        if (check_time(x->start_hour, HOURS))
                return EXIT_FAILURE;
        if (check_time(x->start_min, MINUTES))
                return EXIT_FAILURE;
        if (check_time(x->end_hour, HOURS))
                return EXIT_FAILURE;
        if (check_time(x->end_min, MINUTES))
                return EXIT_FAILURE;
    
        snprintf(times[0], TIME_LEN + NULL_TERM, "%02ld:%02ld:00", 
            x->start_hour, x->start_min);
        snprintf(times[1], TIME_LEN + NULL_TERM, "%02ld:%02ld:00", x->end_hour, 
            x->end_min);
    
        fp = fopen("/Users/Shared/giant_theremin/scripts/times", "w");
        if (!fp) {
                error("Couldn't open file");
                return EXIT_FAILURE;
        }
    
        fprintf(fp, "%u %s %s\n", (unsigned)time(NULL), times[0], times[1]);
        fclose(fp);
    
        post("Startup time: %s", times[0]);
        post("Shutdown time: %s", times[1]);
    
        return EXIT_SUCCESS;
}

void
set_end_hour(t_osx_energy_prefs *x, long l)
{
        if (l > HOURS || l < 0) {
                error("%ld is not a valid hour!", l);
                    return;
        }
        x->end_hour = l;
}

void
set_end_min(t_osx_energy_prefs *x, long l)
{
        if (l > MINUTES || l < 0) {
                error("%ld is not a valid minute!", l);
                return;
        }
        x->end_min = l;
}

void
set_start_hour(t_osx_energy_prefs *x, long l)
{
        if (l > HOURS || l < 0) {
                error("%ld is not a valid hour!", l);
                return;
        }
        x->start_hour = l;
}

void
set_start_min(t_osx_energy_prefs *x, long l)
{
        if (l > MINUTES || l < 0) {
                error("%ld is not a valid minute!", l);
                return;
        }
        x->start_min = l;
}
