
//Globals need to be defined for the library
thickness = 2;
top_diameter = 230;
height = 30;

include <LedKloc.scad>

led24_inner_dia = 54;
led24_outer_dia = 66;
led60_inner_dia = 2*72.39;
led60_outer_dia = 2*78.74;



case(230,led24_inner_dia) {
    case_bottom() {
        led_ring(outer=led24_outer_dia,inner=led24_inner_dia,num=24);
        led_ring(outer=led60_outer_dia,inner=led60_inner_dia,num=60);
    }
    clock(segments=24, outer_cut=led60_inner_dia);
    clock(segments=60, outer_cut=led60_outer_dia*1.7, inner_cut=led60_outer_dia*0.58);

}


