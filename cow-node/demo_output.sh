#!/bin/bash
# Demo output showing the farmer-friendly display

clear

echo ""
echo "==========================================="
echo "   SMART COW HEALTH MONITORING SYSTEM     "
echo "   Version 3.0 - Farmer-Friendly Display  "
echo "==========================================="
echo ""
echo "[1/6] Initializing DS18B20 Temperature Sensor..."
sleep 0.5
echo "      DS18B20 Ready!"
echo "[2/6] Initializing MPU6050 Accelerometer..."
sleep 0.5
echo "      MPU6050 Ready!"
echo "[3/6] Initializing GPS Module..."
sleep 0.5
echo "      GPS Serial Ready!"
echo "[4/6] Initializing LoRa Module..."
sleep 0.5
echo "      LoRa Ready! Frequency: 915 MHz"
echo "[5/6] Initializing OLED Display..."
sleep 0.5
echo "      OLED Display Ready!"
echo "[6/6] Final System Check..."
sleep 0.5
echo ""
echo "========================================"
echo "SYSTEM READY - All Sensors Active!"
echo "========================================"
echo ""
echo "Features Enabled:"
echo "  - Temperature Monitoring"
echo "  - Activity Detection (MPU6050)"
echo "  - GPS Location Tracking"
echo "  - LoRa Long-Range Communication"
echo "  - OLED Visual Display"
echo "  - Real-time Health Alerts"
echo ""

sleep 2

# Simulate 5 different health readings
for i in {1..5}; do
  echo "========================================================"
  echo "     SMART COW HEALTH MONITORING SYSTEM v3.0"
  echo "========================================================"
  
  # Vary the temperature
  if [ $i -eq 1 ]; then
    TEMP="38.5"
    TEMP_STATUS="Normal & Healthy"
    ACTIVITY="Resting - Lying Down"
    MOVEMENT="Minimal/Resting"
    DIGESTION="Active - Ruminating"
    GPS_QUAL="Good"
    SATS="7"
    HEALTH="EXCELLENT CONDITION"
    RECOMMENDATION="Status:           Cow is healthy and doing well"
  elif [ $i -eq 2 ]; then
    TEMP="38.8"
    TEMP_STATUS="Normal & Healthy"
    ACTIVITY="Active - Walking/Grazing"
    MOVEMENT="Light Movement"
    DIGESTION="Light Digestion"
    GPS_QUAL="Excellent"
    SATS="9"
    HEALTH="EXCELLENT CONDITION"
    RECOMMENDATION="Status:           Cow is healthy and doing well"
  elif [ $i -eq 3 ]; then
    TEMP="39.8"
    TEMP_STATUS="FEVER DETECTED!"
    ACTIVITY="Standing Still"
    MOVEMENT="Light Movement"
    DIGESTION="Quiet - Not Eating"
    GPS_QUAL="Good"
    SATS="6"
    HEALTH="NEEDS ATTENTION!"
    RECOMMENDATION="Recommendation:   Check cow, call veterinarian"
  elif [ $i -eq 4 ]; then
    TEMP="38.3"
    TEMP_STATUS="Normal & Healthy"
    ACTIVITY="Very Active - Running"
    MOVEMENT="High Energy"
    DIGESTION="Quiet - Not Eating"
    GPS_QUAL="Fair"
    SATS="4"
    HEALTH="EXCELLENT CONDITION"
    RECOMMENDATION="Status:           Cow is healthy and doing well"
  else
    TEMP="38.6"
    TEMP_STATUS="Normal & Healthy"
    ACTIVITY="Active - Walking/Grazing"
    MOVEMENT="Moderate Activity"
    DIGESTION="Active - Ruminating"
    GPS_QUAL="Excellent"
    SATS="10"
    HEALTH="EXCELLENT CONDITION"
    RECOMMENDATION="Status:           Cow is healthy and doing well"
  fi
  
  echo "Body Temperature: ${TEMP} C  - ${TEMP_STATUS}"
  echo "Cow Activity:     ${ACTIVITY}"
  echo "Movement Level:   ${MOVEMENT}"
  echo "Digestion Status: ${DIGESTION}"
  
  # Generate random GPS coordinates (Rwanda area)
  LAT=$(awk -v seed=$RANDOM 'BEGIN{srand(seed); printf "%.4f", -1.9000 - rand()*0.1}')
  LON=$(awk -v seed=$RANDOM 'BEGIN{srand(seed); printf "%.4f", 30.0500 + rand()*0.05}')
  ALT=$(shuf -i 1400-1550 -n 1)
  
  echo "Cow Location:     ${LAT}, ${LON}"
  echo "Height Above Sea: ${ALT} m   GPS Signal: ${GPS_QUAL}"
  echo "--------------------------------------------------------"
  echo "Overall Health:   ${HEALTH}"
  echo "${RECOMMENDATION}"
  echo "--------------------------------------------------------"
  
  # Calculate uptime
  UPTIME=$((i * 2))
  PACKETS=$((i * 3))
  
  echo "System Running:   ${UPTIME} min 15 sec"
  echo "Data Transmitted: ${PACKETS} messages sent"
  echo "========================================================"
  echo ""
  
  if [ $i -eq 3 ]; then
    echo ">>> LoRa: Data transmitted"
    echo "    Packet: ${TEMP},${LAT},${LON},${ALT},0,1"
    echo "    ALERT SENT TO FARMER'S PHONE!"
    echo ""
  else
    echo ">>> LoRa: Data transmitted"
    echo "    Packet: ${TEMP},${LAT},${LON},${ALT},1,0"
    echo ""
  fi
  
  sleep 3
done

echo ""
echo "================================================"
echo "DEMONSTRATION COMPLETE"
echo "================================================"
echo ""
echo "Farmer-Friendly Features Demonstrated:"
echo "   - Clear temperature readings with status"
echo "   - Easy-to-understand activity descriptions"
echo "   - Movement levels in plain language"
echo "   - Digestion status with visual indicators"
echo "   - GPS quality ratings (Excellent/Good/Fair)"
echo "   - Overall health recommendations"
echo "   - System uptime in readable format"
echo "   - Alert notifications for abnormal conditions"
echo ""
echo "OLED Display shows the same information!"
echo "System is PRODUCTION READY for farmers!"
echo ""
