//wall thickness (e.g. stability, max printer res)
thickness = 2;
top_diameter = 130;
height = 30;


led60_inner_dia = 72.39;
led60_outer_dia = 78.74;
led24_inner_dia = 54;
led24_outer_dia = 66;
//add some space for manufactoring params
led_ring_tolerance = 2;

// ------------- PROGRAM
//Calculate minimum bottom diameter on purpose max 45 deg 
bottom_diameter = top_diameter - height*2;
$fn=12;

diameter = led_inner-led_tolerance;


module outline() {
    cylinder(h=height, d1=bottom_diameter, d2=top_diameter, center=true);
}

module plate() {
    $fn=48;
    difference() {
        outline();
        difference() {
            cylinder(h=height, d1=bottom_diameter-thickness, d2=top_diameter-thickness, center=true);
            translate([0,0,-height/2+thickness/2]){
                difference() {
                    cylinder(h=thickness,d=bottom_diameter*2,center=true);
                    cylinder(h=thickness,d=led_outer+led_tolerance,center=true);
                }
            }
        }
    }
}


module clock(segments) {
    module divider() {
        cube([thickness/2,top_diameter,height], center=true);
    }
    angle = 360/segments;
    difference() {
        intersection() {
            outline();
            for (i = [0:angle:179]) {
                rotate(i,0,0) {
                    divider();
                }
            }
        };
    }
}

module led_ring(outer,inner,height=3) {
        cube([thickness/2,top_diameter,height], center=true);

    
}


module inline() {
    cylinder(h=height, d=diameter, center=true);
}

module inner() {
    difference() {
        inline();
        cylinder(h=height, d=diameter-thickness, center=true);
    }
}





//plate();
//clock(24);
//clock(60);
led_ring(led60_inner_dia,led60_outer_dia);
//inner();
//backplane();