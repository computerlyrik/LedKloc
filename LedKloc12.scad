//wall thickness (e.g. stability, max printer res)
thickness = 2;
top_diameter = 130;
height = 30;

led_inner_dia = 27;
led_outer_dia = 38.4;



case(230,led_inner_dia) {
    case_bottom() {
        led_ring(outer=led_outer_dia,inner=led_inner_dia,num=24);
    }
    clock(segments=12);
}




// ------------- PROGRAM
//Calculate minimum bottom diameter on purpose max 45 deg 
bottom_diameter = top_diameter - height*2;

module case(outer_diameter,inner_diameter) {
    $fn=48;
    difference() {
        /* Create case_outline and stamp out inner hole for microcontroller etc */
        difference() {
            cylinder(h=height, d1=bottom_diameter, d2=top_diameter, center=true);
            cylinder(h=height, d=inner_diameter-thickness, center=true);
        }
        difference() {
            /* create case inline and substract as negative */
            cylinder(h=height, d1=bottom_diameter-thickness, d2=top_diameter-thickness, center=true);            
            /* inner ring */
            cylinder(h=height, d=inner_diameter, center=true);
            /* add child content  */
            children();
        }
    }

}

module case_bottom() { 
    translate([0,0,-height/2+thickness/2]){
        difference() {
            cylinder(h=thickness,d=bottom_diameter,center=true);
            children();
        }
    }
}


module led_ring(outer,inner,h=3,manufacturer_tolerance = 1) {
        difference() {
                cylinder(h=thickness,d=outer+manufacturer_tolerance,center=true);
                cylinder(h=thickness,d=inner-manufacturer_tolerance,center=true);
        }
}

module clock(segments, outer_cut=top_diameter,inner_cut=0) {
    module divider() {
        difference() {
            cube([thickness/2,outer_cut,height], center=true);
            cube([thickness/2,inner_cut,height], center=true);
        }
    }
    angle = 360/segments;
    for (i = [0:angle:179]) {
        rotate(i,0,0) {
            divider();
        }
    }
}