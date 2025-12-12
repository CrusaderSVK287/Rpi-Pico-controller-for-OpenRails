#include "sensors.h"

void sensors_report(struct sensor_data *data)
{
    if (data == NULL)
        return;

    /* Switches */
    data->pause         = 1;
    data->track_monitor = 1;
    data->next_station  = 1;
    data->headlights    = 1;
    data->pantograph1   = 1;
    data->pantograph2   = 1;

    /* 3-state switch */
    data->view          = 2;
    /* 4-state switch */
    data->train_break   = 3;

    /* Analogs */
    data->throttle      = 9;
    data->direction     = 10;
    data->engine_break  = 11;
}
