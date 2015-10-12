// ------------- PROGRAM
//Calculate minimum bottom diameter on purpose max 45 deg 
bottom_diameter = top_diameter - height*2;

module case(outer_diameter,inner_diameter,bottom_diameter=bottom_diameter) {
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
            cylinder(h=thickness,d=top_diameter,center=true);
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

module clock(segments, outer_cut=top_diameter,inner_cut=0, shift=0) {
    module divider() {
        glow = thickness+1.5;
        translate([0,0,glow/2-1.5]) {
            difference() {
                cube([thickness/4,outer_cut,height-glow], center=true);
                cube([thickness/4,inner_cut,height-glow], center=true);
            }
        }
    }
    angle = 360/segments;
    for (i = [0:angle:179]) {
        rotate(i+shift,0,0) {
            divider();
        }
    }
}
