#!/bin/bash
#
# Run up a test mqtt-sn network.
# Simulates radio traffic using socat and serial gateway
#
# Assume that the various programs have been placed into the path.
# Easiest method is to put symlinks in ~/bin
#
# Architecture
# ------------
#
# RSMB broker providing mqtt broker on tcp:1883 and mqtt-sn broker on udp:1883
# mqtt-sn bridge udp:1883 <--> /tmp/ttyBridge1
# socat PTY,link=/tmp/ttyBridge1  
# socat PTY,link=/tmp/ttyBridge2 
# mqtt-sn bridge udp:1884 <--> /tmp/ttyBridge2
# RSMB broker providing mqtt-sn broker on udp:1893

function onexit( ) {
  sleep 1
  killall broker_mqtts   # <-- for some reason these dont go away ...
}

trap onexit 0

# Start in the listed order...

# CURRENT PROBLEM EVEN USING NETCAT CANT SEND DATA INTO THE BRIDGE

#xterm -fn 6x10 -geom 64x7+600+112 -hold -title "Broker0" -e broker_mqtts rsmb-config-1 &
xterm -fn 6x10 -geom 64x7+600+336 -hold -e socat -d -d PTY,link=/tmp/ttyBridge0 PTY,link=/tmp/ttyBridge1 &
#xterm -fn 6x10 -geom 128x7+300+560 -hold -title "Broker1" -e broker_mqtts rsmb-config-2 &
sleep 1
xterm -fn 6x10 -geom 128x7+300+224 -hold -title "Bridge0" -e mqtt-sn-serial-bridge -d -p 1884 /tmp/ttyBridge0 &
xterm -fn 6x10 -geom 128x7+300+448 -hold -title "Bridge1" -e strace mqtt-sn-serial-bridge -d -p 1894 /tmp/ttyBridge1 &
sleep 1
xterm -fn 6x10 -geom 64x7+600+672 -hold -title "Publisher" -e \
    bash -c "while true ; do sleep 2 ; mqtt-sn-pub -p 1893 -i sensor1 -t /far/sensor/1 -m 'Hello,World'; done" &
xterm -fn 6x10 -geom 64x7+600+0   -hold -title "SubscriberBase" -e mqtt-sn-sub -p 1883 -i basesub1 -t /far/sensor/1 &
xterm -fn 6x10 -geom 64x7+400+672 -hold -title "SubscriberFar"  -e mqtt-sn-sub -p 1893 -i farsub1 -t /far/sensor/1 &

wait
sleep 1
killall broker_mqtts   # <-- for some reason these dont go away ...

