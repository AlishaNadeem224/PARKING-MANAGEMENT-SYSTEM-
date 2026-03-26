#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_SLOTS 100
#define MAX_VEHICLES 100
#define MAX_USERS 50
#define FEE_PER_HOUR 10.0
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

typedef struct {
    char username[20];
    char password[20];
    bool isAdmin;
} User;

typedef struct {
    char vehicleNumber[20];
    char vehicleType[20];
    char entryTime[10];
    char exitTime[10];
    float parkingFee;
    int slotNumber;
    bool isBooked;
    char bookedBy[20];
} ParkingSlot;

// Global variables
ParkingSlot parkingSlots[MAX_SLOTS];
User users[MAX_USERS];
int slotCount = 0;
int userCount = 0;
char currentUser[20] = "";
bool isAdmin = false;

// Function prototypes
void initializeSystem();
bool authenticateUser();
void registerUser();
void addVehicle();
void exitVehicle();
void viewParkingSlots();
void generateReport();
void saveData();
void loadData();
void searchVehicle();
void displayAvailableSlots();
int validateTimeFormat(const char *time);
void toUpperCase(char *str);
void bookSlot();
void cancelBooking();
void adminMenu();
void userMenu();
void saveUserData();
void loadUserData();
void clearInputBuffer();

int main() {
    initializeSystem();
    loadData();
    loadUserData();

    while (1) {
        printf("\n=== Parking Lot Management System ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (authenticateUser()) {
                    if (isAdmin) {
                        adminMenu();
                    } else {
                        userMenu();
                    }
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                saveData();
                saveUserData();
                printf("Exiting system...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

void initializeSystem() {
    // Initialize one admin user if none exists
    if (userCount == 0) {
        strcpy(users[0].username, ADMIN_USERNAME);
        strcpy(users[0].password, ADMIN_PASSWORD);
        users[0].isAdmin = true;
        userCount = 1;
    }
}

bool authenticateUser() {
    char username[20], password[20];
    printf("Enter username: ");
    if (scanf("%19s", username) != 1) {
        printf("Invalid username.\n");
        clearInputBuffer();
        return false;
    }
    printf("Enter password: ");
    if (scanf("%19s", password) != 1) {
        printf("Invalid password.\n");
        clearInputBuffer();
        return false;
    }
    clearInputBuffer();

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
            strcpy(currentUser, username);
            isAdmin = users[i].isAdmin;
            printf("Login successful! Welcome, %s.\n", username);
            return true;
        }
    }
    printf("Invalid username or password.\n");
    return false;
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("Maximum user limit reached.\n");
        return;
    }

    char username[20], password[20];
    printf("Enter new username: ");
    if (scanf("%19s", username) != 1) {
        printf("Invalid username.\n");
        clearInputBuffer();
        return;
    }

    // Check if username already exists
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists.\n");
            clearInputBuffer();
            return;
        }
    }

    printf("Enter password: ");
    if (scanf("%19s", password) != 1) {
        printf("Invalid password.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    users[userCount].isAdmin = false;
    userCount++;

    saveUserData();
    printf("Registration successful! You can now login.\n");
}

void adminMenu() {
    int choice;
    while (1) {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Add Vehicle\n");
        printf("2. Exit Vehicle\n");
        printf("3. View Parking Slots\n");
        printf("4. Generate Report\n");
        printf("5. Search Vehicle\n");
        printf("6. Display Available Slots\n");
        printf("7. View All Users\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: addVehicle(); break;
            case 2: exitVehicle(); break;
            case 3: viewParkingSlots(); break;
            case 4: generateReport(); break;
            case 5: searchVehicle(); break;
            case 6: displayAvailableSlots(); break;
            case 7: 
                printf("\nRegistered Users:\n");
                for (int i = 0; i < userCount; i++) {
                    printf("%s (%s)\n", users[i].username, users[i].isAdmin ? "Admin" : "User");
                }
                break;
            case 8: 
                strcpy(currentUser, "");
                isAdmin = false;
                return;
            default: printf("Invalid choice. Please try again.\n");
        }
    }
}

void userMenu() {
    int choice;
    while (1) {
        printf("\n=== USER MENU (%s) ===\n", currentUser);
        printf("1. Book a Parking Slot\n");
        printf("2. Cancel Booking\n");
        printf("3. View Parking Slots\n");
        printf("4. Search Vehicle\n");
        printf("5. Display Available Slots\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: bookSlot(); break;
            case 2: cancelBooking(); break;
            case 3: viewParkingSlots(); break;
            case 4: searchVehicle(); break;
            case 5: displayAvailableSlots(); break;
            case 6: 
                strcpy(currentUser, "");
                return;
            default: printf("Invalid choice. Please try again.\n");
        }
    }
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

    // Log this entry to a file
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

            // Log this exit to a file
            FILE *logFile = fopen("parking_log.txt", "a");
            if (logFile) {
                fprintf(logFile, "EXIT, Slot %d, %s, %s, Entry: %s, Exit: %s, Fee: %.2f, %s\n", 
                        parkingSlots[i].slotNumber, parkingSlots[i].vehicleNumber, 
                        parkingSlots[i].vehicleType, parkingSlots[i].entryTime, 
                        exitTime, parkingSlots[i].parkingFee, currentUser);
                fclose(logFile);
            }

            // Remove the vehicle from the active slots
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
    int occupiedSlots = 0;
    
    for (int i = 0; i < slotCount; i++) {
        totalRevenue += parkingSlots[i].parkingFee;
        if (strcmp(parkingSlots[i].exitTime, "--:--") != 0) {
            vehiclesExited++;
        }
        if (parkingSlots[i].isBooked) {
            bookedSlots++;
        } else if (strcmp(parkingSlots[i].vehicleNumber, "BOOKED") != 0) {
            occupiedSlots++;
        }
    }
    
    printf("\n=== Parking Lot Status ===\n");
    printf("Total Slots: %d\n", MAX_SLOTS);
    printf("Available Slots: %d\n", MAX_SLOTS - slotCount);
    printf("Booked Slots: %d\n", bookedSlots);
    printf("Occupied Slots: %d\n", occupiedSlots);
    
    // Show recent parking activity
    printf("\n=== Recent Activity ===\n");
    FILE *logFile = fopen("parking_log.txt", "r");
    if (logFile) {
        char line[200];
        int count = 0;
        while (fgets(line, sizeof(line), logFile) && count < 10) {
            printf("%s", line);
            count++;
        }
        fclose(logFile);
    } else {
        printf("No parking log available.\n");
    }
}

void searchVehicle() {
    char vehicleNumber[20];
    printf("Enter Vehicle Number to Search: ");
    if (scanf("%19s", vehicleNumber) != 1) {
        printf("Invalid vehicle number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int found = 0;
    for (int i = 0; i < slotCount; i++) {
        if (strcmp(parkingSlots[i].vehicleNumber, vehicleNumber) == 0) {
            printf("\nVehicle Found:\n");
            printf("Slot Number: %d\n", parkingSlots[i].slotNumber);
            printf("Vehicle Type: %s\n", parkingSlots[i].vehicleType);
            printf("Entry Time: %s\n", parkingSlots[i].entryTime);
            printf("Exit Time: %s\n", parkingSlots[i].exitTime);
            printf("Parking Fee: %.2f\n", parkingSlots[i].parkingFee);
            printf("Status: %s\n", parkingSlots[i].isBooked ? "Booked" : "Occupied");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Vehicle Not Found!\n");
    }
}

void displayAvailableSlots() {
    printf("\nAvailable Slots:\n");
    int available = MAX_SLOTS - slotCount;
    
    if (available == 0) {
        printf("No slots available - Parking Full!\n");
        return;
    }
    
    // Track which slots are occupied
    int occupiedSlots[MAX_SLOTS] = {0};
    for (int i = 0; i < slotCount; i++) {
        if (parkingSlots[i].slotNumber > 0 && parkingSlots[i].slotNumber <= MAX_SLOTS) {
            occupiedSlots[parkingSlots[i].slotNumber - 1] = 1;
        }
    }
    
    // Display available slots in a grid
    printf("Available slot numbers:\n");
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (!occupiedSlots[i]) {
            printf("%3d ", i + 1);
            if ((i + 1) % 10 == 0) printf("\n");
        }
    }
    printf("\nTotal available slots: %d\n", available);
}

void bookSlot() {
    if (slotCount >= MAX_SLOTS) {
        printf("Parking Full! No slots available for booking.\n");
        return;
    }

    // Display available slots
    printf("\nAvailable Slots:\n");
    bool availableSlots[MAX_SLOTS + 1] = {false}; // Initialize all as available (index 1-MAX_SLOTS)
    for (int i = 1; i <= MAX_SLOTS; i++) {
        availableSlots[i] = true;
    }

    // Mark taken slots as unavailable
    for (int j = 0; j < slotCount; j++) {
        availableSlots[parkingSlots[j].slotNumber] = false;
    }

    // Print available slots
    bool anyAvailable = false;
    for (int i = 1; i <= MAX_SLOTS; i++) {
        if (availableSlots[i]) {
            printf("%d ", i);
            anyAvailable = true;
        }
    }
    printf("\n");

    if (!anyAvailable) {
        printf("No available slots found.\n");
        return;
    }

    // Get user's slot choice
    int chosenSlot;
    printf("\nEnter the slot number you want to book: ");
    scanf("%d", &chosenSlot);

    // Validate the chosen slot
    if (chosenSlot < 1 || chosenSlot > MAX_SLOTS) {
        printf("Invalid slot number. Please choose between 1 and %d.\n", MAX_SLOTS);
        return;
    }

    if (!availableSlots[chosenSlot]) {
        printf("Slot %d is already taken. Please choose another slot.\n", chosenSlot);
        return;
    }

    // Create and save the booking
    ParkingSlot newSlot;
    newSlot.slotNumber = chosenSlot;
    newSlot.isBooked = true;
    strcpy(newSlot.bookedBy, currentUser);
    strcpy(newSlot.vehicleNumber, "BOOKED");
    strcpy(newSlot.vehicleType, "BOOKED");
    strcpy(newSlot.entryTime, "--:--");
    strcpy(newSlot.exitTime, "--:--");
    newSlot.parkingFee = 0.0;

    parkingSlots[slotCount++] = newSlot;
    saveData();

    printf("\nSlot %d booked successfully for %s!\n", chosenSlot, currentUser);
    
    printf("BOOKING EXPIRES IN 24 HOURS!!.\n");
}

void cancelBooking() {
    int slotToCancel;
    printf("Enter slot number to cancel booking: ");
    if (scanf("%d", &slotToCancel) != 1) {
        printf("Invalid slot number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < slotCount; i++) {
        if (parkingSlots[i].slotNumber == slotToCancel && 
            strcmp(parkingSlots[i].bookedBy, currentUser) == 0 &&
            parkingSlots[i].isBooked) {
            
            // Shift all elements after this one left
            for (int j = i; j < slotCount - 1; j++) {
                parkingSlots[j] = parkingSlots[j + 1];
            }
            slotCount--;
            saveData();
            printf("\nBooking for slot %d cancelled successfully.\n", slotToCancel);
            return;
        }
    }
    printf("No booking found for slot %d under your account.\n", slotToCancel);
}

void saveData() {
    FILE *file = fopen("parking_data.txt", "w");
    if (!file) {
        printf("Error saving parking data!\n");
        return;
    }
    
    for (int i = 0; i < slotCount; i++) {
        fprintf(file, "%d,%s,%s,%s,%s,%.2f,%d,%s\n",
                parkingSlots[i].slotNumber,
                parkingSlots[i].vehicleNumber,
                parkingSlots[i].vehicleType,
                parkingSlots[i].entryTime,
                parkingSlots[i].exitTime,
                parkingSlots[i].parkingFee,
                parkingSlots[i].isBooked ? 1 : 0,
                parkingSlots[i].bookedBy);
    }
    fclose(file);
}

void loadData() {
    FILE *file = fopen("parking_data.txt", "r");
    if (!file) {
        return;
    }
    
    slotCount = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        ParkingSlot slot;
        int isBooked;
        
        if (sscanf(line, "%d,%19[^,],%19[^,],%9[^,],%9[^,],%f,%d,%19[^\n]",
               &slot.slotNumber,
               slot.vehicleNumber,
               slot.vehicleType,
               slot.entryTime,
               slot.exitTime,
               &slot.parkingFee,
               &isBooked,
               slot.bookedBy) >= 7) {
            
            slot.isBooked = isBooked;
            parkingSlots[slotCount++] = slot;
        }
    }
    fclose(file);
}

void saveUserData() {
    FILE *file = fopen("user_data.txt", "w");
    if (!file) {
        printf("Error saving user data!\n");
        return;
    }
    
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s,%s,%d\n",
                users[i].username,
                users[i].password,
                users[i].isAdmin ? 1 : 0);
    }
    fclose(file);
}

void loadUserData() {
    FILE *file = fopen("user_data.txt", "r");
    if (!file) {
        // Initialize with admin if no file exists
        strcpy(users[0].username, ADMIN_USERNAME);
        strcpy(users[0].password, ADMIN_PASSWORD);
        users[0].isAdmin = true;
        userCount = 1;
        return;
    }
    
    userCount = 0;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int isAdmin;
        if (sscanf(line, "%19[^,],%19[^,],%d",
               users[userCount].username,
               users[userCount].password,
               &isAdmin) == 3) {
            users[userCount].isAdmin = isAdmin;
            userCount++;
        }
    }
    fclose(file);
}

int validateTimeFormat(const char *time) {
    int hour, minute;
    if (sscanf(time, "%d:%d", &hour, &minute) != 2) {
        printf("Invalid time format. Use HH:MM\n");
        return 0;
    }
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        printf("Invalid time. Hour (0-23) and Minute (0-59)\n");
        return 0;
    }
    return 1;
}

void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}