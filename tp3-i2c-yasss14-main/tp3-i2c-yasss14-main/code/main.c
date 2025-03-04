

#include "mcc_generated_files/mcc.h"

#define ADDRESSE_I2C_ECRAN  0x28    // V�rifier l?adresse exacte du LCD I2C
#define ADDRESSE_I2C_EEPROM 0x50    // Adresse EEPROM I2C (24LC256)
#define LCD_COMMAND 0xFE            // Commande LCD

// Fonction pour �crire sur l'�cran LCD via I2C
void LCD_Write(uint8_t *message, uint8_t length) {
    while (I2C_Open(ADDRESSE_I2C_ECRAN) == I2C_BUSY);
    I2C_SetBuffer(message, length);
    I2C_MasterOperation(0);
    while (I2C_Close() == I2C_BUSY);
    __delay_ms(1000);
}

// Fonction pour effacer l'�cran LCD
void LCD_Clear() {
    EUSART1_Write(LCD_COMMAND);
    EUSART1_Write(0x01);
}

// Fonction pour �crire un octet dans l'EEPROM
void EEPROM_Write(uint16_t addr, uint8_t data) {
    while (I2C_Open(ADDRESSE_I2C_EEPROM) == I2C_BUSY);
    uint8_t buffer[3] = { (addr >> 8) & 0xFF, addr & 0xFF, data };
    I2C_SetBuffer(buffer, 3);
    I2C_MasterOperation(0);
    while (I2C_Close() == I2C_BUSY);
    __delay_ms(5);
}

// Fonction pour lire 3 octets de l'EEPROM
void EEPROM_Read(uint16_t addr, uint8_t *buffer, uint8_t length) {
    while (I2C_Open(ADDRESSE_I2C_EEPROM) == I2C_BUSY);
    uint8_t addrBuffer[2] = { (addr >> 8) & 0xFF, addr & 0xFF };
    I2C_SetBuffer(addrBuffer, 2);
    I2C_MasterOperation(0);
    while (I2C_Close() == I2C_BUSY);
    __delay_ms(5);
    
    // Lire les donn�es
    while (I2C_Open(ADDRESSE_I2C_EEPROM) == I2C_BUSY);
    I2C_SetBuffer(buffer, length);
    I2C_MasterOperation(1);
    while (I2C_Close() == I2C_BUSY);
    __delay_ms(5);
}

void main(void) {
    // Initialisation du syst�me
    SYSTEM_Initialize();
    LCD_Clear();  // vider ecran

    uint8_t message[3] = { '0', '1', '2' };

    while (1) {
        // �criture en EEPROM
        EEPROM_Write(0x0000, message[0]);
        EEPROM_Write(0x0001, message[1]);
        EEPROM_Write(0x0002, message[2]);

        // Lecture des 3 octets stock�s en EEPROM
        EEPROM_Read(0x0000, message, 3);

        // Affichage sur l'�cran LCD
        LCD_Write(message, 3);
    }
}

