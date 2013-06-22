# Bash script: PBCAcosmUpdater-Cron

#!/bin/bash

ARDUINO_IP="asgeco.ciapee.lan.afm.co"
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
    #VPV=$(echo $STRING | awk '{print $6}' | tr -d "\r")
    BATTS=$(echo $STRING | awk '{print $7}')
    VCC=$(echo $STRING | awk '{print $8}')
    ST_CUR=$(echo $STRING | awk '{print $9}')
    ENGINE=$(echo $STRING | awk '{print $10}')
    WARMINGUP=$(echo $STRING | awk '{print $11}')
    COOLINGDOWN=$(echo $STRING | awk '{print $12}')
    WAITING=$(echo $STRING | awk '{print $13}')
    MANU_ENABLE=$(echo $STRING | awk '{print $14}')
    FATAL=$(echo $STRING | awk '{print $15}')
    LASTAUXSTATE=$(echo $STRING | awk '{print $16}')
    AUTO_ENABLE=$(echo $STRING | awk '{print $17}')
    TIMEOUTS=$(echo $STRING | awk '{print $18}')
    SECS_TOT=$(echo $STRING | awk '{print $19}')
    SECS=$(echo $STRING | awk '{print $20}')
    GENON=$(echo $STRING | awk '{print $21}')
    GENOFF=$(echo $STRING | awk '{print $22}')
    REMOTE_ENABLE=$(echo $STRING | awk '{print $23}')
    REMOTE_REQUEST=$(echo $STRING | awk '{print $24}')
    VCONV=$(echo $STRING | awk '{print $25}')
    OIL=$(echo $STRING | awk '{print $26}')

    echo "STARTER (1/0): ${STARTER}"
    echo "ONSOLENOID (1/0): ${ONSOLENOID}"
    echo "OFFSOLENOID (1/0): ${OFFSOLENOID}"
    echo "MAINS (1/0): ${MAINS}"
    echo "MANU_REQUEST (1/0): ${MANU_REQUEST}"
    echo "BATT (mV): ${BATTS}"
    echo "Vcc (mV): ${VCC}"
    echo "Starter Current (bits): ${ST_CUR}"
    echo "Engine: ${ENGINE}"
    echo "Total run timer (secs): ${SECS_TOT}"
    echo "Engine now on for ${SECS} seconds. Total for $[ ${SECS_TOT} / 60 ] mins."
    echo "Engine Warming Up: ${WARMINGUP}"
    echo "Engine Cooling down: ${COOLINGDOWN}"
    echo "System is waiting: ${WAITING}"
    echo "Fatal error: ${FATAL}"
    echo "AUTO_ENABLE is: ${AUTO_ENABLE}, Request: ${AUTO_REQUEST}"
    echo "GENON: ${GENON}  --  GENOFF: ${GENOFF}"
    echo "Failed startup attempts: ${TIMEOUTS}"
    echo "MANU_ENABLE is ${MANU_ENABLE}"
    echo "Remote enabled: ${REMOTE_ENABLE}, Request: ${REMOTE_REQUEST}"
    echo "Voltage conversion factor: ${VCONV}"

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

function start(){
    curl "http://asgeco/?ASGECOv2&17=1&23=1&24=1"
}

function stop(){
    curl "http://asgeco/?ASGECOv2&17=0&23=0&24=0"
}

function upload() {
 curl --request PUT \
      --data "$(echo -e ${DATA_BINARY})" \
      --header "X-ApiKey: ${API_KEY}" \
      --verbose \
      http://api.cosm.com/v2/feeds/$FEED_ID
}

if [ "$1" == "up" ]; then upload;
elif [ "$1" == "start" ]; then start;
elif [ "$1" == "stop" ]; then stop;
else status;
fi;
