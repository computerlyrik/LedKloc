//Globals need to be defined for the library
thickness = 2;
top_diameter = 130;
height = 30;

include <LedKloc.scad>

led_inner_dia = 27;
led_outer_dia = 38.4;

case(230,led_inner_dia) {
    case_bottom() {
        led_ring(outer=led_outer_dia,inner=led_inner_dia,num=24);
    }
    clock(segments=12);
}

