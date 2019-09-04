set ns [new Simulator]

set ql1 100
set ql2 100

set bw1 10Mb
set bw2 10Mb
set d1 10ms
set d2 10ms
set d2 10ms

$ns color 0 Blue
$ns color 1 Red
$ns color 2 Azure
$ns color 3 Cyan
$ns color 4 Coral

set f [open 1.nam w]
$ns namtrace-all $f

set t [open 1.tr w]
$ns trace-all $t

proc finish {} {
    global ns f t
    $ns flush-trace
    close $f
    close $t

    # exec nam 1.nam &
    exec perl trac.p 1.tr > packet_loss.txt
    exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 $bw1 $d1 DropTail
$ns duplex-link $n2 $n1 $bw2 $d2 DropTail

$ns queue-limit $n0 $n1 $ql1
$ns queue-limit $n2 $n1 $ql2

$ns duplex-link-op $n0 $n1 queuePos 0.5
$ns duplex-link-op $n2 $n1 queuePos 0.5
 
set tcp [new Agent/TCP]
$tcp set class_ 0
$ns attach-agent $n0 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n1 $sink
$ns connect $tcp $sink
$tcp set fid_ 1

set ftp0 [new Application/FTP]
$ftp0 set packetSize_ 500
$ftp0 attach-agent $tcp
$ftp0 set type_ FTP

set tcp [new Agent/TCP]
$tcp set class_ 1
$ns attach-agent $n2 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n1 $sink
$ns connect $tcp $sink
$tcp set fid_ 2

set ftp2 [new Application/FTP]
$ftp2 set packetSize_ 500
$ftp2 attach-agent $tcp
$ftp2 set type_ FTP

$ns at 0.1 "$ftp0 start"
$ns at 1.6 "$ftp0 stop"

$ns at 0.1 "$ftp2 start"
$ns at 1.6 "$ftp2 stop"

$ns at 2.0 "finish"

$ns run