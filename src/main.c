#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "net_discovery.h"

#define __FILENAME__ "main.c"

int main() {
  interface_info_t *interfaces = NULL;
  int count = 0;
  int i;
  int row = 0;

  // --- ncurses Initialization ---
  initscr();            // Start ncurses mode
  cbreak();             // Line buffering disabled, Pass on ever char
  noecho();             // Don't echo() while we do getch
  keypad(stdscr, TRUE); // Enable function keys (like arrows, F1, etc.)

  // Check if ncurses initialization failed
  if (stdscr == NULL) {
    fprintf(stderr, "Error initializing ncurses.\n");
    return 1;
  }
  // --- End ncurses Initialization ---

  // Use printw (ncurses' printf equivalent)
  printw("Discovering network interfaces...\n");
  row++;
  refresh(); // Refresh the screen to show the message immediately

  if (discover_interfaces(&interfaces, &count) != 0) {
    // Use mvprintw to print at a specific location (row, col)
    mvprintw(row, 0, "ERROR: Failed to discover interfaces\n");
    row++;
    mvprintw(row, 0, "Press any key to exit.");
    refresh();                              // Show error message
    getch();                                // Wait for user input
    endwin();                               // End ncurses mode
    ERROR("Failed to discover interfaces"); // Log the error as well
    return 1;
  }

  clear(); // Clear the screen before printing results
  row = 0; // Reset row counter

  mvprintw(row++, 0, "Found %d network interfaces:", count);
  row++; // Add a blank line

  for (i = 0; i < count; i++) {
    // Check if we are about to go off screen
    if (row >= LINES - 2) { // Leave space for the exit message
      mvprintw(LINES - 1, 0, "Too many interfaces. Press any key...");
      refresh();
      getch();
      clear();
      row = 0;
      mvprintw(row++, 0, "...continuing interface list...");
      row++;
    }

    mvprintw(row++, 2, "Interface #%d:", i + 1); // Indent with col 2
    mvprintw(row++, 4, "Name: %s", interfaces[i].name);
    mvprintw(row++, 4, "Index: %d", interfaces[i].index);

    // Print type on the same line
    mvprintw(row, 4, "Type: %d ", interfaces[i].type);
    // printw continues from the current cursor position
    if (interfaces[i].type == 1) { // ARPHRD_ETHER
      printw("(Ethernet)");
    } else if (interfaces[i].type == 801) { // ARPHRD_IEEE80211
      printw("(WiFi)");
    } else {
      printw("(Other)");
    }
    row++; // Move to the next line *after* printing the type description

    mvprintw(row++, 4, "Status: %s", interfaces[i].is_up ? "UP" : "DOWN");
    mvprintw(row++, 4, "MAC: %s", interfaces[i].mac_address);
    row++; // Add a blank line between interfaces
  }

  // Add exit prompt at the end
  mvprintw(row++, 0, "Press any key to exit.");

  refresh(); // Refresh the screen to show all the interface info
  getch();   // Wait for user input before exiting

  // --- ncurses Cleanup ---
  endwin(); // Restore normal terminal behavior
  // --- End ncurses Cleanup ---

  free_interface_list(interfaces);
  return EXIT_SUCCESS;
}
