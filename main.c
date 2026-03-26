#include "parking.h"

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
