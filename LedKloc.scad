thickness = 0.2;
top_diameter = 15;
bottom_diameter = 8;
height = 3;

module outline() {
    cylinder(h=height, d1=bottom_diameter, d2=top_diameter, center=true);
}
module plate() {
    $fn=48;
    difference() {
        outline();
        difference() {
            cylinder(h=height, d1=bottom_diameter-thickness, d2=top_diameter-thickness, center=true);
            translate([0,0,-height/2]){
                difference() {
                    cylinder(h=thickness,d=bottom_diameter,center=true);
                    cylinder(h=thickness,d=led_outer+led_tolerance,center=true);
                }
            }
            
        }
    }
    
}

led_inner = 2.7;
led_outer = 3.84;
led_tolerance = 0.3;
diameter = led_inner-led_tolerance;
$fn=12;

module inline() {
    cylinder(h=height, d=diameter, center=true);
}

module inner() {
    difference() {
        inline();
        cylinder(h=height, d=diameter-thickness, center=true);
    }
}

module backplane() {

}

module divider() {
    cube([thickness/2,top_diameter,height], center=true);
}

module clock() {
    difference() {
        intersection() {
            outline();
             for (i = [1:30:180]) {
                rotate(i,0,0) {
                    divider();
                }
            }
        };
        inline();
    }
}

plate();
clock();
inner();
backplane();