//wall thickness (e.g. stability, max printer res)
thickness = 2;
top_diameter = 230;
height = 20;

led24_inner_dia = 54;
led24_outer_dia = 66;
led60_inner_dia = 2*72.39;
led60_outer_dia = 2*78.74;

//add some space for manufactoring params
led_ring_tolerance = 2;

// ------------- PROGRAM
//Calculate minimum bottom diameter on purpose max 45 deg 
bottom_diameter = top_diameter - height*2;
$fn=12;

diameter = led24_inner_dia-led_ring_tolerance;

module inner_outline() {
    cylinder(h=height, d=diameter, center=true);
}

module inner() {
    difference() {
        inline();
        cylinder(h=height, d=diameter-thickness, center=true);
    }
}

module case_outline() {
    cylinder(h=height, d1=bottom_diameter, d2=top_diameter, center=true);
}
module case_inline() { 
    cylinder(h=thickness,d=bottom_diameter*2,center=true);
}

module case() {
    $fn=48;
    difference() {
        case_outline();
        difference() {
            cylinder(h=height, d1=bottom_diameter-thickness, d2=top_diameter-thickness, center=true);
            translate([0,0,-height/2+thickness/2]){
                difference() {
                    case_inline();
                    children();
                }
            }
        }
    }
}


module led_ring(outer,inner,h=3,num=12) {
        difference() {
            cylinder(h=thickness,d=outer+led_ring_tolerance,center=true);
            cylinder(h=thickness,d=inner+led_ring_tolerance,center=true);
        }

        //clock(segments=num, outer=outer, inner=inner*0.7);
        
        module clock(segments=num, outer,inner=0) {
            module divider() {
                difference() {
                    cube([thickness/2,outer,height], center=true);
                    cube([thickness/2,inner,height], center=true);
                }

            }
            angle = 360/segments;
            difference() {
                intersection() {
                    case_outline();
                    for (i = [0:angle:179]) {
                        rotate(i,0,0) {
                            divider();
                        }
                    }
                };
            }
        }
}







case() {
        led_ring(outer=led24_outer_dia,inner=led24_inner_dia,num=24);
        led_ring(outer=led60_outer_dia,inner=led60_inner_dia,num=60);
}

        led_ring(outer=led24_outer_dia,inner=led24_inner_dia,num=24);

//clock(24);
//clock(60);
//led_ring(led60_inner_dia,led60_outer_dia);
//inner();
//backplane();