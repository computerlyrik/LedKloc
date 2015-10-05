thickness = 0.3;
top_diameter = 15;
bottom_diameter = 8;
height = 3;

module plate() {
    difference() {
        cylinder(h=height, d1=bottom_diameter, d2=top_diameter, center=true);
        cylinder(h=height, d1=bottom_diameter-thickness, d2=top_diameter-thickness, center=true);
    }

}

module divider() {
    cube([thickness,top_diameter,height], center=true);
}

module clock() {
    for (i = [1:30:180]) {
        rotate(i,0,0) {
            divider();
        }
    }
}

led_inner = 2.7;
led_outer = 3.84;
led_tolerance = 0.3;

module inner() {
    diameter = led_inner-led_tolerance;
    difference() {
        cylinder(h=height, d=diameter, center=true);
        cylinder(h=height, d=diameter-thickness, center=true);
    }
}

module backplane() {
    translate([0,0,-height/2]){
        difference() {
            cylinder(h=thickness,d=bottom_diameter,center=true);
            cylinder(h=thickness,d=led_outer+led_tolerance,center=true);
        }
    }
}

plate();
clock();
inner();
backplane();