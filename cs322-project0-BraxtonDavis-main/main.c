#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 10
#define MAX_NAME_LEN 39
#define EXIT_VALUE 99
#define MIN_PIN 1000
#define MAX_PIN 9999

void print_this_user_info(unsigned short userindex, char username[], unsigned short userpin, bool userIsAdmin);
bool get_user_preference();
int get_user_to_modify_vulnerable();
void change_pin_vulnerable(int user_i, unsigned short u_pin[], int new_pin);
int get_user_to_modify_more_secure(int current_num_users);
bool change_pin_more_secure(int user_i, unsigned short u_pin[], int new_pin, int num_users);

int main(void) {
    struct {
        unsigned short user_isAdmin[MAX_USERS];
        unsigned short user_pin[MAX_USERS];
        char user_name[MAX_USERS][MAX_NAME_LEN];
    } user_data;

    int num_users = 0;
    int user_index = 0;
    int new_pin = 0;
    bool vulnerable_mode = false;
    bool success = false;
    char buffer[256];

    memset(user_data.user_isAdmin, 0, sizeof(user_data.user_isAdmin));
    memset(user_data.user_pin, 0, sizeof(user_data.user_pin));
    memset(user_data.user_name, '-', sizeof(user_data.user_name));

    user_data.user_pin[0] = 16962;
    user_data.user_isAdmin[0] = true;
    strncpy(user_data.user_name[0], "ADMIN", strlen("ADMIN") + 1);

    user_data.user_pin[1] = 4369;
    user_data.user_isAdmin[1] = false;
    strncpy(user_data.user_name[1], "DEFAULT USER", strlen("DEFAULT USER") + 1);

    num_users = 2;

    /******* Ask the user if they want to run vulnerable code *******/
    vulnerable_mode = get_user_preference();

    while (1) {
        printf("\nCurrent Users:\n");
        for (int i = 0; i < num_users; i++) {
            print_this_user_info(i, user_data.user_name[i], user_data.user_pin[i], user_data.user_isAdmin[i]);
        }

        printf("\nWould you like to modify a user’s PIN? (Enter index or %d to exit)\n", EXIT_VALUE);

        if (vulnerable_mode) {
            user_index = get_user_to_modify_vulnerable();
        } else {
            user_index = get_user_to_modify_more_secure(num_users);
        }

        if (user_index == EXIT_VALUE) {
            printf("Exiting program.\n");
            break;
        }

        printf("Enter the new PIN (4-digit number): ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &new_pin) != 1) {
                printf("Invalid input. Try again.\n");
                continue;
            }
        }

        if (vulnerable_mode) {
            change_pin_vulnerable(user_index, user_data.user_pin, new_pin);
            printf("PIN changed (vulnerable mode) for user %d.\n", user_index);
        } else {
            success = change_pin_more_secure(user_index, user_data.user_pin, new_pin, num_users);
            if (!success) {
                printf("Failed to change PIN.\n");
            }
        }
    }

    return 0;
}

/* Purpose: print all user information */
void print_this_user_info(unsigned short userindex, char username[],
                          unsigned short userpin, bool userIsAdmin) {
    printf("---User Index#%d---\nName: %s  \nPIN: %u  \tisAdmin: %i\n",
           userindex, username, userpin, userIsAdmin);
}

/* Purpose: Ask the user if they want to run the vulnerable version */
bool get_user_preference() {
    int selection = 0;
    char buffer[256];

    printf("Select an option:\n");
    printf("1 - Run vulnerable code\n");
    printf("2 - Run secure code (default)\n");
    printf("Enter your choice: ");

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (sscanf(buffer, "%d", &selection) == 1) {
            return (selection == 1);
        }
    }

    return false;
}

/* Purpose: Read user index from the keyboard without validation */
int get_user_to_modify_vulnerable(void) {
    char buffer[256] = "";
    int desired_index = 0;
    printf("Enter the index of the user to modify (or %d to exit): ", EXIT_VALUE);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        sscanf(buffer, "%d", &desired_index);
    }
    return desired_index;
}

/* Purpose: Change PIN without validation */
void change_pin_vulnerable(int user_i, unsigned short u_pin[], int new_pin) {
    u_pin[user_i] = (unsigned short)new_pin;
}

/* Purpose: Get validated user index */
int get_user_to_modify_more_secure(int current_num_users) {
    char buffer[256] = "";
    int user_index = -1;

    while (1) {
        printf("Enter the index of the user to modify (0-%d) or %d to exit: ",
               current_num_users - 1, EXIT_VALUE);

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &user_index) == 1) {
                if (user_index == EXIT_VALUE) {
                    return EXIT_VALUE;
                }
                if (user_index >= 0 && user_index < current_num_users) {
                    return user_index;
                }
            }
        }
        printf("Invalid input. Please enter a number between 0 and %d, or %d to exit.\n",
               current_num_users - 1, EXIT_VALUE);
    }
}

bool change_pin_more_secure(int user_i, unsigned short u_pin[], int new_pin, int num_users) {
    if (user_i < 0 || user_i >= num_users) {
        printf("Error: Invalid user index.\n");
        return false;
    }
    if (new_pin < MIN_PIN || new_pin > MAX_PIN) {
        printf("Error: PIN must be a 4-digit number (%d-%d).\n", MIN_PIN, MAX_PIN);
        return false;
    }

    u_pin[user_i] = (unsigned short)new_pin;
    printf("Success: PIN changed for user %d.\n", user_i);
    return true;
}
