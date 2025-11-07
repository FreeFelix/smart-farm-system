// Smart Cow Health Monitoring System - Health Monitoring Implementation

#include <Arduino.h>
#include "config.h"
#include "health_data.h"
#include "health_monitor.h"

void checkHealthAlerts() {
  // Temperature alert check
  if (healthData.temperature < TEMP_MIN || healthData.temperature > TEMP_MAX) {
    if (!healthData.tempAlert) {
      Serial.println("\n!!!!! TEMPERATURE ALERT !!!!!");
      Serial.print("Current temperature: ");
      Serial.print(healthData.temperature, 1);
      Serial.println(" C");
      
      if (healthData.temperature > TEMP_MAX) {
        Serial.println("Status: COW HAS FEVER");
        Serial.println("Recommendation: Check the cow immediately and call veterinarian");
      } else {
        Serial.println("Status: COW TEMPERATURE TOO LOW");
        Serial.println("Recommendation: Check the cow and provide warm shelter");
      }
      
      Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      healthData.tempAlert = true;
    }
  } else {
    if (healthData.tempAlert) {
      Serial.println("\n***** Temperature normalized *****");
      Serial.print("Current temperature: ");
      Serial.print(healthData.temperature, 1);
      Serial.println(" C - Healthy range");
      Serial.println("**********************************\n");
      healthData.tempAlert = false;
    }
  }
  
  // Activity alert check
  if (!healthData.isMoving && healthData.activityLevel < ACTIVITY_LIGHT) {
    unsigned long currentTime = millis();
    if (healthData.lastActivityTime == 0) {
      healthData.lastActivityTime = currentTime;
    } else {
      unsigned long inactiveDuration = currentTime - healthData.lastActivityTime;
      if (inactiveDuration > INACTIVITY_ALERT_TIME && !healthData.inactivityAlert) {
        Serial.println("\n!!!!! INACTIVITY ALERT !!!!!");
        Serial.print("Cow has been inactive for ");
        Serial.print(inactiveDuration / 60000);
        Serial.println(" minutes");
        Serial.println("Recommendation: Check if the cow is resting or needs assistance");
        Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        healthData.inactivityAlert = true;
      }
    }
  } else {
    healthData.lastActivityTime = millis();
    healthData.inactivityAlert = false;
  }
}

void updateStatusLEDs() {
  // Turn OFF all LEDs first
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  
  // RED LED: Critical alerts (fever, hypothermia, battery critical)
  if (healthData.tempAlert || healthData.batteryLevel < 10) {
    digitalWrite(LED_RED, HIGH);
  }
  // YELLOW LED: Warnings (low activity, battery low, no GPS)
  else if (healthData.inactivityAlert || healthData.batteryLevel < 20 || !healthData.gpsValid) {
    digitalWrite(LED_YELLOW, HIGH);
  }
  // GREEN LED: All healthy
  else {
    digitalWrite(LED_GREEN, HIGH);
  }
}

void printHealthData() {
  Serial.println("\n╔═══════════════════════════════════════════════════╗");
  Serial.println("║     SMART COW HEALTH MONITORING SYSTEM v3.0      ║");
  Serial.println("╠═══════════════════════════════════════════════════╣");
  
  // Temperature
  Serial.printf("║ Temperature:   %.1f C   ", healthData.temperature);
  if (healthData.tempAlert) {
    Serial.println("[ALERT!]               ║");
  } else {
    Serial.println("[OK]                   ║");
  }
  
  // Activity
  Serial.printf("║ Activity:      %-8s Level: %.1f m/s²  ║\n",
                healthData.isMoving ? "Active" : "Resting",
                healthData.activityLevel);
  
  // Battery
  Serial.printf("║ Battery:       %d%%   Solar: %dV          ║\n",
                healthData.batteryLevel,
                healthData.solarVoltage);
  
  // Digestion
  Serial.printf("║ Digestion:     %-8s Sound: %-6d      ║\n",
                healthData.isDigesting ? "Active" : "Resting",
                healthData.soundLevel);
  
  // Location
  if (healthData.gpsValid && healthData.satellites >= 3) {
    Serial.printf("║ Location:      LAT: %.4f  LON: %.4f        ║\n",
                  healthData.latitude,
                  healthData.longitude);
    Serial.printf("║ Altitude:      %.0f m  Satellites: %-2d             ║\n",
                  healthData.altitude,
                  healthData.satellites);
  } else {
    Serial.println("║ Location:      GPS acquiring signal...            ║");
  }
  
  // LoRa packets
  Serial.printf("║ LoRa Packets:  %-35ld ║\n", healthData.loraPacketsSent);
  
  // Overall status
  Serial.printf("║ Overall:       %-35s ║\n",
                (healthData.tempAlert || healthData.inactivityAlert) ? "ALERT" : "HEALTHY");
  
  Serial.println("╚═══════════════════════════════════════════════════╝\n");
}
