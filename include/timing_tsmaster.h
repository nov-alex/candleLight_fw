#pragma once

#include "gs_usb.h"

/*
TOSUN TSmaster calculates and set can timing by
constant can peripheral frequency 48MHz (not the clock of the controller or crystal)

TOSUN TSmaster default sample point is 75%
*/

static const struct gs_device_bittiming bittimingMap48MHz[] = {
    // 75%  default sample point
    // 1000k
    { .prop_seg = 1u, .phase_seg1 = 10u, .phase_seg2 = 4u, .sjw = 1u, .brp = 3u },
    // 800k
    { .prop_seg = 1u, .phase_seg1 = 13u, .phase_seg2 = 5u, .sjw = 1u, .brp = 3u },
    // 500k
	{ .prop_seg = 1u, .phase_seg1 = 10u, .phase_seg2 = 4u, .sjw = 1u, .brp = 6u },
    // 250k
    { .prop_seg = 1u, .phase_seg1 = 10u, .phase_seg2 = 4u, .sjw = 1u, .brp = 12u},

    // 87.5%
    // 1000k
    { .prop_seg = 1u, .phase_seg1 = 12u, .phase_seg2 = 2u, .sjw = 1u, .brp = 3u },
    // 800k
    { .prop_seg = 1u, .phase_seg1 = 11u, .phase_seg2 = 2u, .sjw = 1u, .brp = 4u },
    // 500k
    { .prop_seg = 1u, .phase_seg1 = 12u, .phase_seg2 = 2u, .sjw = 1u, .brp = 6u },
    // 250k
    { .prop_seg = 1u, .phase_seg1 = 12u, .phase_seg2 = 2u, .sjw = 1u, .brp = 12u}

};
