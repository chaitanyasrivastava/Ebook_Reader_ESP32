#include <SPI.h>
#include <SD.h>

// Define the Chip Select (CS) pin for the SD card module.
// D8 is GPIO 15 on a NodeMCU.
const int chipSelectPin = D8;

// ===================================================================
// IMPORTANT: Change this to the exact name of your unzipped EPUB folder.
// The name must start with a "/" and is case-sensitive.
// ===================================================================
String epubFolderPath = "/Hello_unzipped";


void setup() {
  // Initialize Serial communication for viewing the output.
  Serial.begin(115200);
  Serial.println("\n\n--- EPUB File Path Printer ---");
  Serial.println("Initializing SD Card...");

  // Start the SD card module.
  if (!SD.begin(chipSelectPin)) {
    Serial.println("Initialization failed! Check wiring or SD card.");
    // Stop the program if the card is not found.
    while (true);
  }
  Serial.println("SD Card initialized.");

  // Open the main directory for the book.
  File root = SD.open(epubFolderPath);
  if (!root) {
    Serial.println("Failed to open folder: " + epubFolderPath);
    Serial.println("Check if the folder name in the code is correct.");
    return;
  }
  
  Serial.println("\nStarting traversal of: " + epubFolderPath);
  Serial.println("----------------------------------------");

  // Begin the recursive process of finding and printing file paths.
  printFilePaths(root);

  Serial.println("----------------------------------------");
  Serial.println("Traversal complete.");
}


/**
 * @brief Recursively traverses directories and prints the full path of every file found.
 * @param dir The directory to start scanning from.
 */
// **** THIS IS THE LINE THAT WAS FIXED ****
void printFilePaths(File dir) {
  while (true) {
    // Open the next file or directory in the current folder.
    File entry = dir.openNextFile();
    
    // If there are no more entries, exit the loop for this folder.
    if (!entry) {
      break;
    }

    // The .name() function on the ESP8266 SD library returns the full path
    // starting from the root of the SD card.
    
    // Check if the entry is a file (not a directory).
    if (!entry.isDirectory()) {
      // It's a file, so print its full path.
      Serial.print("Found file: ");
      Serial.println(entry.name());
    }

    // If the entry IS a directory, we need to go inside it.
    if (entry.isDirectory()) {
      Serial.print("Entering directory: ");
      Serial.println(entry.name());
      // This is the recursion: the function calls itself to scan the new directory.
      printFilePaths(entry);
    }

    // Close the current entry to free up memory.
    entry.close();
  }
}


void loop() {
  // The process only needs to run once, so the loop is empty.
}
