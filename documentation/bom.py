import csv
import codecs

# Data
bom_data = [
    ["D3", "SSB43L", "DO-214AA(SMB)", "SSB43L-E3/52T", "C5338051", "40V 0.45V 5A DO-214AA Schottky Barrier Diodes (SBD) ROHS", "7"],
    ["ENC1,ENC2", "440054-3", "SHDR3W50P0X200_1X3_800X470X635P", "440054-3", "C486256", "1x3P 1 2mm Male pin 3 Straight Plugin,P=2mm Wire To Board / Wire To Wire Connector ROHS", "10"],
    ["J2", "2013499-1", "2013499-1", "2013499-1", "C5163346", "USB Connectors MICRO USB REC TYPE B DIP ASSY(AU)", "5"],
    ["J4", "1757242", "1757242", "1757242", "C90074", "2 Bend 1 5.08mm 1x2P Green Board Edge/Receptacle-Close Push-Pull,P=5.08mm Pluggable System Terminal Block ROHS", "5"],
    ["LED1", "CheckFuse", "LED_SMD_0805_R", "NCD0805R1", "C84256", "Colorless transparence 67mcd~195mcd -30℃~+85℃ 615nm~630nm Red 130° 65mW 0805 Light Emitting Diodes (LED) ROHS", "20"],
    ["LED2", "YELLOW", "LED_SMD_0805_R", "NCD0805R1", "C84256", "Colorless transparence 67mcd~195mcd -30℃~+85℃ 615nm~630nm Red 130° 65mW 0805 Light Emitting Diodes (LED) ROHS"],
    ["LED3", "BLUE", "LED_SMD_0805_R", "NCD0805R1", "C84256", "Colorless transparence 67mcd~195mcd -30℃~+85℃ 615nm~630nm Red 130° 65mW 0805 Light Emitting Diodes (LED) ROHS"],
    ["LED7", "GREEN", "LED_SMD_0805_R", "NCD0805R1", "C84256", "Colorless transparence 67mcd~195mcd -30℃~+85℃ 615nm~630nm Red 130° 65mW 0805 Light Emitting Diodes (LED) ROHS"],
    ["U5", "ATMEGA32U4-AU", "QFP80P1200X1200X120-44N", "ATMEGA32U4-AU", "C44854", "32KB 2.7V~5.5V AVR 2.5KB 16MHz FLASH 26 QFP-44(10x10) Microcontroller Units (MCUs/MPUs/SOCs) ROHS", "5"],
    ["D1,D2,D6", "CM1223-02SO", "SOT95P280X145-5N", "CM1223-02SO", "C890511", "8.8V 6V 3.3V SOT-23-5 TVS ROHS", "15"],
    ["J3,J5,J6", "61900411121", "SHDR4W64P0X254_1X4_1020X580X1175P", "1125-2232G0S395C001", "C920766", "3A Straight Square Pins 64P -40℃~+105℃ 2.54mm 双排 Black 2x32P 2.54mm Plugin,P=2.54", "15"]
]

# File name
file_name = "bom_data.tsv"

# Write data to TSV file
with codecs.open(file_name, "w", "utf-8-sig") as tsvfile:
    writer = csv.writer(tsvfile, delimiter='\t')
    writer.writerows(bom_data)

print(f"TSV file '{file_name}' has been created.")
