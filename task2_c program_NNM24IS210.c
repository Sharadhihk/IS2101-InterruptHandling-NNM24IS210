#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Mask flags for devices (0 = enabled, 1 = masked)
int maskKeyboard = 0, maskMouse = 0, maskPrinter = 0;
int running = 1; // Control variable for exit option
pthread_mutex_t lock;

// ---------- ISR (Interrupt Service Routines) ----------
void handleKeyboard() {
    printf("Keyboard Interrupt Triggered -> Handling ISR -> Completed\n");
}
void handleMouse() {
    printf("Mouse Interrupt Triggered -> Handling ISR -> Completed\n");
}
void handlePrinter() {
    printf("Printer Interrupt Triggered -> Handling ISR -> Completed\n");
}

// ---------- Interrupt Controller Thread ----------
void* interruptController(void* arg) {
    while (running) {
        sleep(rand() % 3 + 1); // Random delay between interrupts
        int interrupt = rand() % 3; // Randomly trigger 0=Keyboard,1=Mouse,2=Printer

        pthread_mutex_lock(&lock);

        // Priority: Keyboard > Mouse > Printer
        if (interrupt == 0) { // Keyboard (Highest)
            if (!maskKeyboard)
                handleKeyboard();
            else
                printf("Keyboard Interrupt Ignored (Masked)\n");
        }
        else if (interrupt == 1) { // Mouse (Medium)
            if (!maskMouse && maskKeyboard)
                handleMouse();
            else if (maskMouse)
                printf("Mouse Interrupt Ignored (Masked)\n");
            else if (!maskKeyboard)
                printf("Mouse Interrupt Deferred (Keyboard Active)\n");
        }
        else { // Printer (Lowest)
            if (!maskPrinter && maskKeyboard && maskMouse)
                handlePrinter();
            else if (maskPrinter)
                printf("Printer Interrupt Ignored (Masked)\n");
            else
                printf("Printer Interrupt Deferred (Higher Priority Active)\n");
        }

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// ---------- Main Function ----------
int main() {
    srand(time(0)); // Initialize random generator
    pthread_t t1;
    pthread_mutex_init(&lock, NULL);

    printf("=== INTERRUPT HANDLING SIMULATION ===\n");
    printf("Devices: Keyboard (High) | Mouse (Medium) | Printer (Low)\n");
    printf("-----------------------------------------------------------\n");
    printf("Simulation Started...\n");
    printf("Use menu below to Mask/Unmask devices or Exit.\n");

    // Start interrupt controller thread
    pthread_create(&t1, NULL, interruptController, NULL);

    while (running) {
        int choice, device;
        printf("\nMenu:\n");
        printf("1. Mask/Unmask Device\n");
        printf("2. Continue Simulation\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter Device to Toggle Mask:\n");
            printf("1. Keyboard  2. Mouse  3. Printer\n");
            scanf("%d", &device);
            if (device == 1) maskKeyboard = !maskKeyboard;
            else if (device == 2) maskMouse = !maskMouse;
            else if (device == 3) maskPrinter = !maskPrinter;

            printf("Mask States → Keyboard=%d  Mouse=%d  Printer=%d\n",
                   maskKeyboard, maskMouse, maskPrinter);
        }
        else if (choice == 3) {
            running = 0;
        }
    }

    printf("\nExiting simulation...\n");
    pthread_join(t1, NULL);
    pthread_mutex_destroy(&lock);
    printf("Simulation Ended Successfully.\n");
    return 0;
}
