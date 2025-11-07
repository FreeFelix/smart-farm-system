#!/bin/bash
# Interactive Device Configuration Helper
# For Milk Station Node

echo "=========================================="
echo "  MILK STATION - CONFIGURATION HELPER"
echo "=========================================="
echo ""
echo "This script helps you configure your milk station device."
echo ""
echo "IMPORTANT: This generates commands to send via Serial Monitor"
echo "           at 115200 baud rate."
echo ""

# Function to display menu
show_menu() {
    echo ""
    echo "========== CONFIGURATION OPTIONS =========="
    echo "1. Basic Setup (Name, Farm ID)"
    echo "2. WiFi Configuration"
    echo "3. LoRa Configuration"
    echo "4. Device Manager Setup"
    echo "5. View Current Configuration"
    echo "6. Reset to Factory Defaults"
    echo "7. Generate Complete Setup Script"
    echo "8. Exit"
    echo "==========================================="
    echo ""
}

# Function to generate basic setup
basic_setup() {
    echo ""
    echo "--- Basic Device Setup ---"
    read -p "Enter device name (e.g., 'Milk Station North'): " device_name
    read -p "Enter farm ID (e.g., 'FARM001'): " farm_id
    
    echo ""
    echo "Copy and paste these commands into Serial Monitor:"
    echo "-------------------------------------------"
    echo "SET_NAME $device_name"
    echo "SET_FARM $farm_id"
    echo "SAVE"
    echo "-------------------------------------------"
    echo ""
}

# Function to configure WiFi
wifi_setup() {
    echo ""
    echo "--- WiFi Configuration ---"
    read -p "Enter WiFi SSID: " wifi_ssid
    read -p "Enter WiFi Password: " wifi_pass
    read -p "Enable WiFi? (y/n): " wifi_enable
    
    echo ""
    echo "Copy and paste these commands into Serial Monitor:"
    echo "-------------------------------------------"
    echo "SET_WIFI $wifi_ssid $wifi_pass"
    if [ "$wifi_enable" = "y" ] || [ "$wifi_enable" = "Y" ]; then
        echo "WIFI_ON"
    else
        echo "WIFI_OFF"
    fi
    echo "SAVE"
    echo "-------------------------------------------"
    echo ""
}

# Function to configure LoRa
lora_setup() {
    echo ""
    echo "--- LoRa Configuration ---"
    echo "Frequency options:"
    echo "  433 MHz: 433000000"
    echo "  868 MHz: 868000000"
    echo "  915 MHz: 915000000 (default)"
    read -p "Enter LoRa frequency [915000000]: " lora_freq
    lora_freq=${lora_freq:-915000000}
    
    read -p "Enter Sync Word (hex, e.g., 0x12) [0x12]: " sync_word
    sync_word=${sync_word:-0x12}
    
    read -p "Enter Spread Factor (7-12) [7]: " spread_factor
    spread_factor=${spread_factor:-7}
    
    read -p "Enable LoRa? (y/n): " lora_enable
    
    echo ""
    echo "Copy and paste these commands into Serial Monitor:"
    echo "-------------------------------------------"
    echo "SET_LORA $lora_freq $sync_word $spread_factor"
    if [ "$lora_enable" = "y" ] || [ "$lora_enable" = "Y" ]; then
        echo "LORA_ON"
    else
        echo "LORA_OFF"
    fi
    echo "SAVE"
    echo "-------------------------------------------"
    echo ""
}

# Function to configure Device Manager
manager_setup() {
    echo ""
    echo "--- Device Manager Setup ---"
    read -p "Enter Device Manager IP [192.168.1.100]: " manager_ip
    manager_ip=${manager_ip:-192.168.1.100}
    
    read -p "Enter Device Manager Port [8080]: " manager_port
    manager_port=${manager_port:-8080}
    
    echo ""
    echo "Copy and paste these commands into Serial Monitor:"
    echo "-------------------------------------------"
    echo "SET_MANAGER $manager_ip $manager_port"
    echo "SAVE"
    echo "-------------------------------------------"
    echo ""
}

# Function to view configuration
view_config() {
    echo ""
    echo "Copy and paste this command into Serial Monitor:"
    echo "-------------------------------------------"
    echo "SHOW"
    echo "-------------------------------------------"
    echo ""
}

# Function to reset
reset_config() {
    echo ""
    echo "⚠️  WARNING: This will reset ALL settings to factory defaults!"
    read -p "Are you sure? (yes/no): " confirm
    
    if [ "$confirm" = "yes" ]; then
        echo ""
        echo "Copy and paste this command into Serial Monitor:"
        echo "-------------------------------------------"
        echo "RESET"
        echo "-------------------------------------------"
        echo ""
    else
        echo "Reset cancelled."
    fi
}

# Function to generate complete setup
complete_setup() {
    echo ""
    echo "--- Complete Station Setup ---"
    echo ""
    
    read -p "Device name: " device_name
    read -p "Farm ID: " farm_id
    echo ""
    
    read -p "WiFi SSID: " wifi_ssid
    read -p "WiFi Password: " wifi_pass
    echo ""
    
    read -p "Device Manager IP [192.168.1.100]: " manager_ip
    manager_ip=${manager_ip:-192.168.1.100}
    read -p "Device Manager Port [8080]: " manager_port
    manager_port=${manager_port:-8080}
    echo ""
    
    read -p "LoRa Frequency [915000000]: " lora_freq
    lora_freq=${lora_freq:-915000000}
    read -p "LoRa Sync Word [0x12]: " sync_word
    sync_word=${sync_word:-0x12}
    read -p "LoRa Spread Factor [7]: " spread_factor
    spread_factor=${spread_factor:-7}
    
    echo ""
    echo "=========================================="
    echo "  COMPLETE SETUP COMMANDS"
    echo "=========================================="
    echo "Copy ALL commands below and paste into Serial Monitor:"
    echo ""
    echo "SET_NAME $device_name"
    echo "SET_FARM $farm_id"
    echo "SET_WIFI $wifi_ssid $wifi_pass"
    echo "WIFI_ON"
    echo "SET_MANAGER $manager_ip $manager_port"
    echo "SET_LORA $lora_freq $sync_word $spread_factor"
    echo "LORA_ON"
    echo "SAVE"
    echo "SHOW"
    echo ""
    echo "=========================================="
    echo ""
    
    # Save to file
    filename="milk_station_config_$(date +%Y%m%d_%H%M%S).txt"
    {
        echo "# Milk Station Configuration"
        echo "# Generated: $(date)"
        echo ""
        echo "SET_NAME $device_name"
        echo "SET_FARM $farm_id"
        echo "SET_WIFI $wifi_ssid $wifi_pass"
        echo "WIFI_ON"
        echo "SET_MANAGER $manager_ip $manager_port"
        echo "SET_LORA $lora_freq $sync_word $spread_factor"
        echo "LORA_ON"
        echo "SAVE"
        echo "SHOW"
    } > "$filename"
    
    echo "✅ Configuration saved to: $filename"
    echo ""
}

# Main loop
while true; do
    show_menu
    read -p "Select option (1-8): " choice
    
    case $choice in
        1) basic_setup ;;
        2) wifi_setup ;;
        3) lora_setup ;;
        4) manager_setup ;;
        5) view_config ;;
        6) reset_config ;;
        7) complete_setup ;;
        8) 
            echo ""
            echo "Exiting configuration helper."
            echo "Don't forget to upload firmware: pio run --target upload"
            echo "Then monitor serial: pio device monitor"
            echo ""
            exit 0
            ;;
        *)
            echo "Invalid option. Please select 1-8."
            ;;
    esac
done
