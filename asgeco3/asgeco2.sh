#!/bin/bash

package=asgeco2

# Default arduino IP.
ARDUINO_IP="asgeco.ciapee.lan.afm.co"

function printHelp() {
        echo "$package - Package description"
        echo " "
        echo "$package [options] action"
        echo " "
        echo "options:"
        echo "-h, --help                show brief help"
        echo "-d, --COOLINGDOWN=<secs>  specify cool down time, before deactivating load"
        echo "-f, --GENOFF=<mV>         specify threshold for turning off generator in mV"
        echo "-i, --ipaddress=<IP>      specify IP address of arduino"
        echo "-m, --MODE=<0|1>          0=Manual Operation, 1=Automatic Operation"
        echo "                              (Automatic is PV voltage based)"
        echo "-n, --GENON=<mV>          specify threshold for turning on generator in mV"
        echo "-r, --minruntime=<mins>   specify minimum running time in minutes"
        echo "-u, --WARMINGUP=<secs>    specify warm up time, before activating load"
        echo "-v, --VCONV=factor        specify conversion factor to calibrate proper BATT mV"
        echo " "
        echo "actions:"
        echo "up                        upload data to cosm"
        echo "start                     start generator"
        echo "stop                      stop generator"
        echo "choke                     close fuel valve"
}

while test $# -gt 0; do
        case "$1" in
                -h|--help)
                        printHelp
                        exit 0
                        ;;
                -i|--ipaddress)
                        shift
                        export ARDUINO_IP=$1
                        shift
                        ;;
                -v|--VCONV)
                        shift
                        export NEW_VCONV=$1
                        shift
                        ;;
                -n|--GENON)
                        shift
                        export NEW_GENON=$1
                        shift
                        ;;
                -f|--GENOFF)
                        shift
                        export NEW_GENOFF=$1
                        shift
                        ;;
                -r|--minruntime)
                        shift
                        export MINIMUMRUNTIME=$1
                        shift
                        ;;
                -u|--WARMINGUP)
                        shift
                        export NEW_WARMINGUP=$1
                        shift
                        ;;
                -d|--COOLINGDOWN)
                        shift
                        export NEW_COOLINGDOWN=$1
                        shift
                        ;;
                -m|--MODE)
                        shift
                        export NEW_MODE=$1
                        shift
                        ;;
                up|start|stop|choke)
                        export ACTION=$1
                        shift
                        ;;
        esac
done


FEED_ID="68890"
API_KEY="ryX-pFwLbjGw7cH2RagB4NeGeAwZVR3EcZWy-Sh52jw"

# ping -c 3 ${ARDUINO_IP} > /dev/null
# if [ $? -ne 0 ]; then
# 	echo "cosm Updater: ${ARDUINO_IP} is not responding";
# 	exit;
# fi

AWK=


function status() {

    STRING=$(curl --silent ${ARDUINO_IP} | grep ASGECOv2)
    STARTER=$(echo $STRING | awk '{print $2}')
    ONSOLENOID=$(echo $STRING | awk '{print $3}')
    OFFSOLENOID=$(echo $STRING | awk '{print $4}')
    MAINS=$(echo $STRING | awk '{print $5}')
    MANU_REQUEST=$(echo $STRING | awk '{print $6}')
    BATTS=$(echo $STRING | awk '{print $7}')
    VCC=$(echo $STRING | awk '{print $8}')
    ST_CUR=$(echo $STRING | awk '{print $9}')
    ENGINE=$(echo $STRING | awk '{print $10}')
    WARMINGUP=$(echo $STRING | awk '{print $11}')
    COOLINGDOWN=$(echo $STRING | awk '{print $12}')
    WAITING=$(echo $STRING | awk '{print $13}')
    MODE=$(echo $STRING | awk '{print $14}')
    FATAL=$(echo $STRING | awk '{print $15}')
    AUTO_REQUEST=$(echo $STRING | awk '{print $16}')
    AUX_STATE=$(echo $STRING | awk '{print $17}')
    TIMEOUTS=$(echo $STRING | awk '{print $18}')
    SECS_TOT=$(echo $STRING | awk '{print $19}')
    SECS=$(echo $STRING | awk '{print $20}')
    GENON=$(echo $STRING | awk '{print $21}')
    GENOFF=$(echo $STRING | awk '{print $22}')
    VALVE=$(echo $STRING | awk '{print $23}')
    REMOTE_REQUEST=$(echo $STRING | awk '{print $24}')
    VCONV=$(echo $STRING | awk '{print $25}')
    OIL=$(echo $STRING | awk '{print $26}')
    WARMUPTIME=$(echo $STRING | awk '{print $27}')
    COOLDOWNTIME=$(echo $STRING | awk '{print $28}')
    MINIMUMRUNTIME=$(echo $STRING | awk '{print $29}')
    OFF_LOCK=$(echo $STRING | awk '{print $30}')

    echo "STARTER (1/0): ........${STARTER}"
    echo "ONSOLENOID (1/0): .....${ONSOLENOID}"
    echo "OFFSOLENOID (1/0): ....${OFFSOLENOID}"
    echo "MAINS (1/0): ..........${MAINS}"
    echo "MODE (1=AUTO/0=MANU): .${MODE}"
    echo "OFF_LOCK:..............${OFF_LOCK}"
    echo "AUTO_REQUEST (1/0): ...${AUTO_REQUEST}"
    echo "MANU_REQUEST (1/0): ...${MANU_REQUEST}"
    echo "AUX_STATE (1/0): ......${AUX_STATE}"
    echo "BATT (mV): ............${BATTS}"
    echo "Vcc (mV): .............${VCC}"
    echo "Starter Current (bits):${ST_CUR}"
    echo "Fuel Valve (1=open): ..${VALVE}"
    echo "Engine: ...............${ENGINE}"
    echo "Total run timer (secs):${SECS_TOT}"
    echo "Engine Warming Up: ....${WARMINGUP}"
    echo "Engine Cooling down: ..${COOLINGDOWN}"
    echo "System is waiting: ....${WAITING}"
    echo "Fatal error: ..........${FATAL}"
    echo "Low Batt Trigger(mV)...${GENON}"
    echo "Hi  Batt Trigger(mV)...${GENOFF}"
    echo "Failed start attempts:.${TIMEOUTS}"
    echo "Voltage conversion:....${VCONV}"
    echo "Warmup timer (secs): ..${WARMUPTIME}"
    echo "Cooldown timer (secs):.${COOLDOWNTIME}"
    echo "Min. Run Time (mins):..${MINIMUMRUNTIME}"
    echo "Engine now on for ${SECS} seconds. Total for $[ ${SECS_TOT} / 60 ] mins."

    let $SECS;

    if [ ${SECS} -gt 0 ]; then
        RUNNING="YES";
    else
        RUNNING="NO";
    fi

    echo "Engine running: ${RUNNING}"
    echo "OIL alarm: ${OIL}"

    DATA_BINARY="
    {\n
      \"version\":\"1.0.0\",\n
      \"datastreams\":[\n
          {\"id\":\"Battery\", \"current_value\":\"${BATTS}\"},\n
          {\"id\":\"Arduino_Vcc\", \"current_value\":\"${VCC}\"},\n
          {\"id\":\"Engine_Running\", \"current_value\":\"${ES}\"},\n
          {\"id\":\"AUTO\", \"current_value\":\"${AUTO}\"},\n
          {\"id\":\"Current_ON_time\", \"current_value\":\"${SECS}\"}\n
      ]\n
    }\n
    "
}

URL="http://${ARDUINO_IP}/?ASGECOv2"

if [ "${NEW_WARMINGUP}" != "" ]; then
    URL="${URL}&27=${NEW_WARMINGUP}"
fi

if [ "${NEW_COOLINGDOWN}" != "" ]; then
    URL="${URL}&28=${NEW_COOLINGDOWN}"
fi

if [ "${NEW_VCONV}" != "" ]; then
    URL="${URL}&25=${NEW_VCONV}"
fi

if [ "${NEW_GENON}" != "" ]; then
    URL="${URL}&21=${NEW_GENON}"
fi

if [ "${NEW_GENOFF}" != "" ]; then
    URL="${URL}&22=${NEW_GENOFF}"
fi

if [ "${MINIMUMRUNTIME}" != "" ]; then
    URL="${URL}&29=${MINIMUMRUNTIME}"
fi

if [ "${NEW_MODE}" != "" ]; then
    URL="${URL}&14=${NEW_MODE}"
fi

function updateSettings(){
    curl "${URL}"
}

function start(){
    curl "http://${ARDUINO_IP}/?ASGECOv2&14=0&6=1"
}

function stop(){
    curl "http://${ARDUINO_IP}/?ASGECOv2&14=0&6=0"
}

function choke(){
    curl "http://${ARDUINO_IP}/?ASGECOv2&4=1"
    curl "http://${ARDUINO_IP}/?ASGECOv2&4=0"
}


function upload() {
 curl --request PUT \
      --data "$(echo -e ${DATA_BINARY})" \
      --header "X-ApiKey: ${API_KEY}" \
      --verbose \
      http://api.cosm.com/v2/feeds/$FEED_ID
}

updateSettings;

if [ "$ACTION" == "up" ]; then upload;
elif [ "$ACTION" == "start" ]; then start;
elif [ "$ACTION" == "stop" ]; then stop;
elif [ "$ACTION" == "choke" ]; then choke;
else status;
fi;
