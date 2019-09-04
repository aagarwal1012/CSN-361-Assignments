#!/usr/bin/perl

use :strict;
if($#ARGV<0){
printf("Usage: <trace-file>\n");
exit 1;
}

# to open the given trace file
open(Trace, $ARGV[0]) or die "Cannot open the trace file";
my $sc = 0; # sending counter
my $rc = 0; # receiving counter
my $rp = 0;
my $mc =0;
my $d_udp = [0,0];
my $d_tcp = [0,0];
my $t_tcp = [0,0];
my $t_udp = [0,0];

my %pkt_fc = (); #packet forwarding counter
while(<Trace>){ # read one line in from the file
my @line = split; #split the line with delimin as space


if($line[4] eq "cbr"){
    if($line[2] eq "0"){
        $t_udp[0]++;
        if($line[0] eq "d"){
            $d_udp[0]++;
        }
    }
    if($line[2] eq "2"){
        $t_udp[1]++;
        if($line[0] eq "d"){
            $d_udp[1]++;
        }
    }
}

if($line[4] eq "tcp"){
    if($line[2] eq "0"){
        $t_tcp[0]++;
        if($line[0] eq "d"){
            $d_tcp[0]++;
        }
    }
    if($line[2] eq "2"){
        $t_tcp[1]++;
        if($line[0] eq "d"){
            $d_tcp[1]++;
        }
    }
}
}
my $i = 0;
while($i<2){
    if($i==0){
        printf("Node 0 to 1\n");
    }else{
        printf("Node 2 to 1\n");
    }
    printf("%f\n",$t_tcp[$i]);
    # Total tcp length 
    # printf("Total udp length %f\n",$t_udp[$i]);

    printf("%f\n",$d_tcp[$i]);
    # Dropped tcp length 
    # printf("Dropped udp length %f\n\n",$d_udp[$i]);
    $i++;
}
