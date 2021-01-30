// SD Card
void setupSDCard();
void resetEeprom();
void printDebitCount();
void testSDCard();

// ***** SD CARD ***** 
// Setup SD Card
void setupSDCard() {
  Serial.println("Setup: Initializing SD Card...");
 
  if (!SD.begin(3)) {
    Serial.println("Setup: Initialization SD Card failed!");
    while (1);
  } else {
    Serial.println("Setup: Initialization SD Card successs!");
    // try create, write, delete file
    testSDCard();
  }
}

void testSDCard() {
  // perform create, write, delete file [still WIP]
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  Serial.println("Setup: SD Card Done!");
}