#ifndef PARKING_H
#define PARKING_H

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
extern ParkingSlot parkingSlots[MAX_SLOTS];
extern User users[MAX_USERS];
extern int slotCount;
extern int userCount;
extern char currentUser[20];
extern bool isAdmin;

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

#endif