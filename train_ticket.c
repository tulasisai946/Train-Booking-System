#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BOOKINGS 200
#define FILENAME "bookings.dat"
typedef struct {
    int id;
    char name[50];
    char from[30];
    char to[30];
    char date[12]; // dd-mm-yyyy
    int seats;
} Booking;

Booking bookings[MAX_BOOKINGS];
int count = 0;
int next_id = 1;
void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}
void load_bookings() {
    FILE *f = fopen(FILENAME, "rb");
    if (!f) return;
    if (fread(&count, sizeof(int), 1, f) != 1) { fclose(f); return; }
    if (fread(&next_id, sizeof(int), 1, f) != 1) { fclose(f); return; }
    fread(bookings, sizeof(Booking), count, f);
    fclose(f);
}
void save_bookings() {
    FILE *f = fopen(FILENAME, "wb");
    if (!f) { printf("Error saving file.\n"); return; }
    fwrite(&count, sizeof(int), 1, f);
    fwrite(&next_id, sizeof(int), 1, f);
    fwrite(bookings, sizeof(Booking), count, f);
    fclose(f);
}
void add_booking() {
    if (count >= MAX_BOOKINGS) { printf("Booking full.\n"); return; }
    Booking b;
    b.id = next_id++;
    printf("Enter passenger name: ");
    fgets(b.name, sizeof(b.name), stdin); trim_newline(b.name);
    printf("From station: ");
    fgets(b.from, sizeof(b.from), stdin); trim_newline(b.from);
    printf("To station: ");
    fgets(b.to, sizeof(b.to), stdin); trim_newline(b.to);
    printf("Travel date (dd-mm-yyyy): ");
    fgets(b.date, sizeof(b.date), stdin); trim_newline(b.date);
    printf("Number of seats: ");
    if (scanf("%d", &b.seats) != 1) { while(getchar()!='\n'); printf("Invalid seats.\n"); return; }
    while(getchar()!='\n'); // clear newline
    bookings[count++] = b;
    save_bookings();
    printf("Booking added. ID: %d\n", b.id);
}
void view_all() {
    if (count == 0) { printf("No bookings.\n"); return; }
    printf("All bookings (%d):\n", count);
    for (int i = 0; i < count; ++i) {
        Booking *b = &bookings[i];
        printf("ID:%d | %s | %s->%s | %s | Seats:%d\n",
               b->id, b->name, b->from, b->to, b->date, b->seats);
    }
}
int find_index_by_id(int id) {
    for (int i = 0; i < count; ++i) if (bookings[i].id == id) return i;
    return -1;
}
void search_booking() {
    printf("Enter booking ID to search: ");
    int id; if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf("Invalid ID.\n"); return; }
    while(getchar()!='\n');
    int idx = find_index_by_id(id);
    if (idx == -1) { printf("Booking ID %d not found.\n", id); return; }
    Booking *b = &bookings[idx];
    printf("Found: ID:%d | %s | %s->%s | %s | Seats:%d\n",
           b->id, b->name, b->from, b->to, b->date, b->seats);
}
void cancel_booking() {
    printf("Enter booking ID to cancel: ");
    int id; if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf("Invalid ID.\n"); return; }
    while(getchar()!='\n');
    int idx = find_index_by_id(id);
    if (idx == -1) { printf("Booking ID %d not found.\n", id); return; }
    // shift left
    for (int i = idx; i < count-1; ++i) bookings[i] = bookings[i+1];
    count--;
    save_bookings();
    printf("Booking %d canceled.\n", id);
}
void menu() {
    printf("\n=== Train Booking System ===\n");
    printf("1. Add booking\n");
    printf("2. View all bookings\n");
    printf("3. Search booking by ID\n");
    printf("4. Cancel booking by ID\n");
    printf("5. Exit\n");
    printf("Choose: ");
}
int main() {
    load_bookings();
    // ensure next_id is > any existing ID
    for (int i=0;i<count;++i) if (bookings[i].id >= next_id) next_id = bookings[i].id + 1;

    int choice;
    while (1) {
        menu();
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); printf("Invalid choice.\n"); continue; }
        while(getchar()!='\n');
        switch (choice) {
            case 1: add_booking(); break;
            case 2: view_all(); break;
            case 3: search_booking(); break;
            case 4: cancel_booking(); break;
            case 5: printf("Goodbye.\n"); exit(0);
            default: printf("Invalid option.\n");
        }
    }
    return 0;
}
