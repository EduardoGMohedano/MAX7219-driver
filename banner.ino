#include "max2719.h"
#include "alphabet.h"
#include "time.h"
#include "string.h"

// Function to convert Unix timestamp to human-readable format
void unixToHumanReadable(time_t timestamp, char *buffer, size_t bufferSize) {
    // Convert timestamp to local time
    struct tm *localTime = localtime(&timestamp);
    if (localTime == NULL) {
        // Handle error
        snprintf(buffer, bufferSize, "Invalid timestamp");
        return;
    }

    // Format the time into the buffer
    // Example format: "2024-04-27 14:35:00"
    if (strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S\n", localTime) == 0) {
        // Handle formatting error
        snprintf(buffer, bufferSize, "Formatting error");
    }
}


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE3)); 
  pinMode(chip_select_pin, OUTPUT);
  digitalWrite(chip_select_pin, HIGH);
  delay(200);

  write_all_matrix(SHUTDOWN_CODE, SHUTDOWN_MODE, 4);

  //Test off which means normal mode operation
  write(DISPLAY_TEST_CODE, DISPLAY_NORMAL_OP_MODE);

  //Scan limit to the max
  write(SCAN_LIMIT_CODE, DISPLAY_DIGIT_UP_TO_7);

  //Intensity to half the maximum value
  write(INTENSITY_CODE, INTENSITY_15_PERCENT);

  //No decoding
  write(DECODE_MODE, NO_DECODE_MODE);

  //Shutdown off
  //write(SHUTDOWN_CODE, NORMAL_MODE);

  //Write only one letter to the selected matrix - DEMO ONLY 
  char letters[10] = "abcdeABCDE";
  for(int i = 0; i < 10; i++){
    //write_one_letter(letters[i],3);
    write_one_letter(letters[i], i%4); 
    delay(700); 
  }

  //write(SHUTDOWN_CODE, NORMAL_MODE);

}

// time_t unixTimestamp = 1728239856;
// char humanReadable[100];
// int i = 0;

void loop() {
  
  //String message = Serial.readString();
  // if( message.length() > 0 ){
  //   Serial.print("New arrived message is: ");
  //   Serial.print(message);

  //   for(int index = 0; index < message.length(); index++){
  //     write_one_letter(message[index]);
  //     delay(500); 
  //   }
  //   write_one_letter(' ');
  // }


  //Write only one letter to the selected matrix
  // char letters[5] = "abcde";
  // for(int i = 0; i < 5; i++){
  //   //write_one_letter(letters[i],3);
  //   delay(1000); 
  // }

  //This function allow you to shift a letter over the selected matrix number
  //Shift your letter over a 4 matrix array
  char letter = 'E';
  for(int m = 0; m < 4; m++ )
    shift_one_letter(letter, m); 


  // IMPLEMENTATION OF A CLOCK 
  // Convert and print the current time
  // Example format: "2024-04-27 14:35:00"
  // unixToHumanReadable(unixTimestamp, humanReadable, sizeof(humanReadable));
  // write_mine(humanReadable[14], humanReadable[15],humanReadable[17], humanReadable[18]);
  // i++;
  // if( i % 50 == 0)
  //   unixTimestamp++;
  
  
  //USE THIS FUNCTION TO SHIFT  A MESSAGE THROUGH ALL 4 MATRIX
  // shift_message("Hola amigos!!?");
  // delay(350);

    
}
