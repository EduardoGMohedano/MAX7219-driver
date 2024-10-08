#include "max2719.h"
#include "alphabet.h"

// This function allow you to directly send SPI command with indicated data to the MAX7219
static void write(int opcode, int data){
    digitalWrite(chip_select_pin, LOW);
    SPI.transfer(opcode);   //OPCODE 
    SPI.transfer(data);     //DATA
    digitalWrite(chip_select_pin, HIGH);
    SPI.endTransaction();
}

//Only send one command and send no-op code to repeat it to all your matrix in case they are connected in chain 
void write_all_matrix(int opcode, int data, int matrix_number){
    write(opcode, data);
    delay(300);
    
    for(int i = 0; i < matrix_number; i++){
        digitalWrite(chip_select_pin, LOW);
        SPI.transfer(NO_OP_CODE);   //OPCODE for noop
        SPI.transfer(0x00);         //Dummy data used with no-op code
        digitalWrite(chip_select_pin, HIGH);
        SPI.endTransaction();
        delay(300);
    }
}

//Write data to all the rows in a matrix 
void write_to_matrix(uint8_t* data){
    for(int row = 0; row < 8; row++){
        write(DIGIT_0_CODE + row, *(data+row));
    }
}

//Display the char in the matrix
void write_one_letter(char letter){
    int index = 0;
    if(letter < 32 || letter > 126)
        return;

    index = letter - 32;
    for(int row = 0; row < 8; row++){
        write(DIGIT_0_CODE + row, alphabet[index][row]);
    }
}

void shift_one_letter(char letter){
    int index = 0;
    if(letter < 32 || letter > 126)
        return;

    index = letter - 32;
    uint16_t letter_shifted = 0;
    for(int i = 0; i < 16; i++){
        for(int row = 0; row < 8; row++){
            if(i < 8)
                letter_shifted = (uint16_t)alphabet[index][row] >> (8-i);
            else
                letter_shifted = (uint16_t)alphabet[index][row] << (i-8);

            write(DIGIT_0_CODE + row, letter_shifted);
        }  
        write(SHUTDOWN_CODE, NORMAL_MODE);
        delay(350);
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    }
}


//This api allows us to write a letter in certain matrix. The rest of the matrix should be turned off
void write_one_letter(char letter, uint8_t matrix_no){
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    //flush_data();
    write_one_letter(letter);

    write(SHUTDOWN_CODE, NORMAL_MODE);
    for(int i = 0; i < matrix_no; i++) 
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);

}

void shift_message(String message){
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    
    message+="   ";
    Serial.print("New arrived msg ");
    Serial.print(message);

    int msg_size = message.length()-1;
    int matrix_size = 8;
    uint16_t shift_size = (msg_size+4)*matrix_size;
    int index4letter = 0;
    uint8_t letter_shifted[4][8];
    memset(&letter_shifted[0][0], 0, 8);
    memset(&letter_shifted[1][0], 0, 8);
    memset(&letter_shifted[2][0], 0, 8);
    memset(&letter_shifted[3][0], 0, 8);

    for(int i = 0; i < shift_size; i++){
      index4letter = i/matrix_size;

      for(int row = 0; row < 8; row++){
        if( i < 8)
            letter_shifted[0][row] = alphabet[ message[index4letter] -32 ][row] >> (8-i);
        else
            letter_shifted[0][row] = (alphabet[ message[index4letter-1] - 32 ][row]) << (i%8) | alphabet[ message[index4letter] - 32 ][row] >>( 8 - i%8 );

        if( (i > 8) && (i <16))
            letter_shifted[1][row] = alphabet[ message[index4letter-1] -32 ][row] >> (16-i);
        else if( i >= 16 )
            letter_shifted[1][row] =(alphabet[ message[index4letter-2] - 32 ][row]) << (i%8) | alphabet[ message[index4letter-1] - 32 ][row] >> (8 - i%8 );

        if( (i > 16) && (i < 24))
            letter_shifted[2][row] = alphabet[message[index4letter-2] -32 ][row] >> (24-i);
        else if( i >= 24 )
            letter_shifted[2][row] = (alphabet[ message[index4letter-3] - 32 ][row]) << (i%8) | alphabet[ message[index4letter-2] - 32 ][row]>> (8 - i%8);

        if( (i > 24) && (i <32))
            letter_shifted[3][row] = alphabet[ message[index4letter-3] -32 ][row] >> (32-i);
        else if( i >= 32 )
            letter_shifted[3][row] = (alphabet[ message[index4letter-4] - 32 ][row]) << (i%8) | alphabet[ message[index4letter-3] - 32 ][row] >> (8 - i%8);
      } 
       
      //Times to quickly show data on all matrix
      for(int t = 0; t < 10; t++){
          write_data_mine( &letter_shifted[3][0], &letter_shifted[2][0] , &letter_shifted[1][0] , &letter_shifted[0][0]);
      }
    }

    flush_data();
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

}

void shift_one_letter_all_matrix(char letter){
    //int index4letter = 0;
    uint8_t letter_shifted[4][8];
    memset(&letter_shifted[0][0], 0, 8);
    memset(&letter_shifted[1][0], 0, 8);
    memset(&letter_shifted[2][0], 0, 8);
    memset(&letter_shifted[3][0], 0, 8);

    for(int i = 0; i < 40; i++){ //loop to go through all the matrix columns
        //index4letter = i/8;

        for(int row = 0; row < 8; row++){
            if(i < 8)
                letter_shifted[0][row] = alphabet[ letter -32 ][row] >> (8-i);
            else
                letter_shifted[0][row] = (alphabet[ letter - 32 ][row]) << (i-8);

            if( (i > 8) && (i <16))
                letter_shifted[1][row] = alphabet[ letter -32 ][row] >> (16-i);
            else if( i > 16 )
                letter_shifted[1][row] =(alphabet[ letter - 32 ][row]) << (i-16);

            if( (i > 16) && (i < 24))
                letter_shifted[2][row] = alphabet[ letter -32 ][row] >> (24-i);
            else if( i > 24 )
                letter_shifted[2][row] = (alphabet[ letter - 32 ][row]) << (i-24);

            if( (i > 24) && (i <32))
                letter_shifted[3][row] = alphabet[ letter -32 ][row] >> (32-i);
            else if( i > 32 )
                letter_shifted[3][row] = (alphabet[ letter - 32 ][row]) << (i-32);
        }

        //Times  to quickl show data on all matrix
        for(int t = 0; t < 30; t++){
            write_data_mine( &letter_shifted[3][0], &letter_shifted[2][0] , &letter_shifted[1][0] , &letter_shifted[0][0]);
        }
    }  

}
 
void write_data_mine(uint8_t* letter, uint8_t* letter2, uint8_t* letter3, uint8_t* letter4){

    write_to_matrix(letter4);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    delay(5);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    write_to_matrix(letter3);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    delay(5);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    write_to_matrix(letter2);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    delay(5);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    write_to_matrix(letter);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    delay(5);
}

void write_mine(char letter, char letter2, char letter3, char letter4){
    // write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    // write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    // write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    // write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    write_one_letter(letter4);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    delay(5);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);


    write_one_letter(letter3);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    delay(5);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    write_one_letter(letter2);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    delay(5);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    write_one_letter(letter);
    write(SHUTDOWN_CODE, NORMAL_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    delay(5);
}

void shift_one_letter(char letter, uint8_t matrix_no){
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    flush_data();
    // shift_one_letter(letter);

    int index = 0;
    if(letter < 32 || letter > 126)
        return;

    index = letter - 32;
    Serial.print("Letter to shift index is ");
    Serial.println(index);


    uint8_t letter_shifted = 0;
    uint8_t letter_arr[8];

    for(int i = 0; i < 16; i++){
        for(int row = 0; row < 8; row++){
            if(i < 8)
                letter_arr[row] = alphabet[index][row] >> (8-i);
            //letter_shifted = alphabet[index][row] >> (8-i);
            else
                letter_arr[row]= alphabet[index][row] << (i-8);
            //letter_shifted = alphabet[index][row] << (i-8);

            //write(DIGIT_0_CODE + row, letter_shifted);
        }  
        write_to_matrix(letter_arr);
        write(SHUTDOWN_CODE, NORMAL_MODE);
        for(int i = 0;  i < matrix_no; i++ )
            write(SHUTDOWN_CODE, SHUTDOWN_MODE);

        delay(350);
        
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
        write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    }
}

void shift_one_letter_all(char letter){
    flush_data();
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);
    write(SHUTDOWN_CODE, SHUTDOWN_MODE);

    int index = 0;
    if(letter < 32 || letter > 126)
        return;

    index = letter - 32;
    uint16_t letter_shifted = 0;
    for(int i = 0; i < 16; i++){
        for(int times = 0; times < 25; times++){
            for(int row = 0; row < 8; row++){
                if(i < 8)
                    letter_shifted = (uint16_t)alphabet[index][row] >> (8-i);
                else
                    letter_shifted = (uint16_t)alphabet[index][row] << (i-8);

                write(DIGIT_0_CODE + row, letter_shifted);
            }  
            write(SHUTDOWN_CODE, NORMAL_MODE);
            for(int i = 0;  i < 0; i++ )
                write(SHUTDOWN_CODE, SHUTDOWN_MODE);

            delay(3);

            letter_shifted = 0;
            for(int row = 0; row < 8; row++){
                if( (i > 8) & (i < 16) )
                    letter_shifted = (uint16_t)alphabet[index][row] >> (8-i);
                else
                    letter_shifted = (uint16_t)alphabet[index][row] << (i-8);

                write(DIGIT_0_CODE + row, letter_shifted);
            }  
            write(SHUTDOWN_CODE, NORMAL_MODE);
            for(int i = 0;  i < 1; i++ )
                write(SHUTDOWN_CODE, SHUTDOWN_MODE);
            delay(3);

            write(SHUTDOWN_CODE, SHUTDOWN_MODE);
            write(SHUTDOWN_CODE, SHUTDOWN_MODE);
            write(SHUTDOWN_CODE, SHUTDOWN_MODE);
            write(SHUTDOWN_CODE, SHUTDOWN_MODE);
        }
    }
}

void flush_data(){
    for(int row = 0; row < 8; row++){
        write(DIGIT_0_CODE + row, 0x00);
    }
}