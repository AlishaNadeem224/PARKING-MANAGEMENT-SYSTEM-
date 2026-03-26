#include "parking.h"

// Initialize global variables
ParkingSlot parkingSlots[MAX_SLOTS];
int slotCount = 0;

void initializeSystem() {
    // Parking-specific initialization if needed
}

void addVehicle() {
    if (slotCount >= MAX_SLOTS) {
        printf("Parking Full!\n");
        return;
    }

    ParkingSlot newSlot;
    
    printf("Enter Vehicle Number (eg KA01AB1234): ");
    if (scanf("%19s", newSlot.vehicleNumber) != 1) {
        printf("Invalid vehicle number.\n");
        clearInputBuffer();
        return;
    }
    
    printf("Enter Vehicle Type (Car/Bike): ");
    if (scanf("%19s", newSlot.vehicleType) != 1) {
        printf("Invalid vehicle type.\n");
        clearInputBuffer();
        return;
    }
    toUpperCase(newSlot.vehicleType);
    
    do {
        printf("Enter Entry Time (HH:MM)(24hrs): ");
        if (scanf("%9s", newSlot.entryTime) != 1) {
            printf("Invalid time format.\n");
            clearInputBuffer();
            continue;
        }
    } while (!validateTimeFormat(newSlot.entryTime));

    strcpy(newSlot.exitTime, "--:--");
    newSlot.parkingFee = 0.0;
    newSlot.isBooked = false;
    strcpy(newSlot.bookedBy, "");

    // Find first available slot number
    int availableSlot = -1;
    for (int i = 1; i <= MAX_SLOTS; i++) {
        bool slotTaken = false;
        for (int j = 0; j < slotCount; j++) {
            if (parkingSlots[j].slotNumber == i) {
                slotTaken = true;
                break;
            }
        }
        if (!slotTaken) {
            availableSlot = i;
            break;
        }
    }

    if (availableSlot == -1) {
        printf("Error: No available slots found.\n");
        return;
    }

    newSlot.slotNumber = availableSlot;
    parkingSlots[slotCount++] = newSlot;
    
    printf("\nVEHICLE ADDED SUCCESSFULLY!\n");
    printf("Allotted Spot Number: %d\n", newSlot.slotNumber);
    printf("Vehicle Number: %s\n", newSlot.vehicleNumber);
    printf("Vehicle Type: %s\n", newSlot.vehicleType);
    printf("Entry Time: %s\n", newSlot.entryTime);
    printf("Remaining available slots: %d\n", MAX_SLOTS - slotCount);

    FILE *logFile = fopen("parking_log.txt", "a");
    if (logFile) {
        fprintf(logFile, "ENTRY, Slot %d, %s, %s, %s, %s\n", 
                newSlot.slotNumber, newSlot.vehicleNumber, newSlot.vehicleType, 
                newSlot.entryTime, currentUser);
        fclose(logFile);
    }
    saveData();
}

void exitVehicle() {
    char vehicleNumber[20];
    char exitTime[10];
    int entryHour, entryMinute, exitHour, exitMinute;
    int vehicleFound = 0;

    printf("Enter Vehicle Number (eg KA01AB1234): ");
    if (scanf("%19s", vehicleNumber) != 1) {
        printf("Invalid vehicle number.\n");
        clearInputBuffer();
        return;
    }
    
    do {
        printf("Enter Exit Time (HH:MM)(24hrs): ");
        if (scanf("%9s", exitTime) != 1) {
            printf("Invalid time format.\n");
            clearInputBuffer();
            continue;
        }
    } while (!validateTimeFormat(exitTime));

    for (int i = 0; i < slotCount; i++) {
        if (strcmp(parkingSlots[i].vehicleNumber, vehicleNumber) == 0) {
            sscanf(parkingSlots[i].entryTime, "%d:%d", &entryHour, &entryMinute);
            sscanf(exitTime, "%d:%d", &exitHour, &exitMinute);
            
            int entryTotal = entryHour * 60 + entryMinute;
            int exitTotal = exitHour * 60 + exitMinute;
            
            if (exitTotal <= entryTotal) {
                printf("Error: Exit time must be after entry time!\n");
                return;
            }

            strcpy(parkingSlots[i].exitTime, exitTime);
            float hours = (exitTotal - entryTotal) / 60.0;
            parkingSlots[i].parkingFee = FEE_PER_HOUR * hours;

            printf("\nExit Processed Successfully!\n");
            printf("Duration: %.2f hours\n", hours);
            printf("Parking Fee: %.2f\n", parkingSlots[i].parkingFee);
            vehicleFound = 1;

            FILE *logFile = fopen("parking_log.txt", "a");
            if (logFile) {
                fprintf(logFile, "EXIT, Slot %d, %s, %s, Entry: %s, Exit: %s, Fee: %.2f, %s\n", 
                        parkingSlots[i].slotNumber, parkingSlots[i].vehicleNumber, 
                        parkingSlots[i].vehicleType, parkingSlots[i].entryTime, 
                        exitTime, parkingSlots[i].parkingFee, currentUser);
                fclose(logFile);
            }

            for (int j = i; j < slotCount - 1; j++) {
                parkingSlots[j] = parkingSlots[j + 1];
            }
            slotCount--;
            saveData();
            
            break;
        }
    }

    if (!vehicleFound) {
        printf("Vehicle Not Found!\n");
    }
}

void viewParkingSlots() {
    printf("\nCurrent Parking Slots (%d occupied):\n", slotCount);
    printf("Slot | Vehicle Number  | Type  | Entry Time | Exit Time | Fee    | Status\n");
    printf("-----|-----------------|-------|------------|-----------|--------|--------\n");
    
    for (int i = 0; i < slotCount; i++) {
        printf("%-4d | %-15s | %-5s | %-10s | %-9s | %-6.2f | %s\n",
               parkingSlots[i].slotNumber, 
               parkingSlots[i].vehicleNumber, 
               parkingSlots[i].vehicleType, 
               parkingSlots[i].entryTime, 
               parkingSlots[i].exitTime, 
               parkingSlots[i].parkingFee,
               parkingSlots[i].isBooked ? "Booked" : "Occupied");
    }
}

void generateReport() {
    printf("\nGenerating Report...\n");
    float totalRevenue = 0.0;
    int vehiclesExited = 0;
    int bookedSlots = 0;
    int occupiedSlots;
}