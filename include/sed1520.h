/*************************************************************************//**
 * @file sed1520.h
 *
 * @brief Grundfunktionen um ein Display mit einem SED1520-Controller
 * anzusprechen
 * @author Roman Buchert (roman.buchert@googlemail.com)
 *****************************************************************************/
#ifndef __SED1520_H___
#define __SED1520_H___
/*****************************************************************************/

/*************************************************************************//**
 * INCLUDE-Dateien
 *****************************************************************************/
#include <mytypes.h>
#include <string.h>
#include <unistd.h>
/*****************************************************************************/

/*************************************************************************//**
 * DEFINES
 *****************************************************************************/
#ifndef __sed1520_DEF__
#define __sed1520_DEF__

#define SED1520CMD_SET_COL_ADDR		0x00	///< Setzt die Spaltenadresse. Diese muss dazu geodert werden (0..79)

#define SED1520CMD_ADC_CW			0xA0	///< Das Display arbeitet im Uhrzeigersinn
#define SED1520CMD_ADC_CCW			0xA1	///< Das Display arbeitet gegen den Uhrzeigersinn

#define SED1520CMD_STATIC_DRV_OFF	0xA4	///< StaticDrive ist ausgeschaltet
#define SED1520CMD_STATIC_DRV_ON	0xA5	///< StaticDrive ist eingeschaltet

#define SED1520CMD_DUTY_16			0xA8	///< Setzt den DutyCycle auf 1/16
#define SED1520CMD_DUTY_32			0xA9	///< Setzt den DutyCycle auf 1/32

#define SED1520CMD_DISPLAY_OFF		0xAE	///< Schaltet das Display aus (PowerSave)
#define SED1520CMD_DISPLAY_ON		0xAF	///< Schaltet das Display ein

#define SED1520CMD_SET_PAGE0		0xB8	///< Wählt die Page 0 aus
#define SED1520CMD_SET_PAGE1		0xB9	///< Wählt die Page 1 aus
#define SED1520CMD_SET_PAGE2		0xBA	///< Wählt die Page 2 aus
#define SED1520CMD_SET_PAGE3		0xBB	///< Wählt die Page 3 aus

#define SED1520CMD_DISP_START_LINE	0xC0	///< Setzt die Startadresse. Dies muss dazu geodert werden (0..31)

#define SED1520CMD_RMW_START		0xE0	///< Startet das Read-Modify-Write
#define SED1520CMD_RMW_END			0xEE	///< Beendet das Read-Modify-Write

#define SED1520CMD_RESET1			0xFF	///< Reset-Sequenz 1
#define SED1520CMD_RESET2			0xE2	///< Reset-Sequenz 2

#endif //__sed1520_DEF__
/*****************************************************************************/

/*************************************************************************//**
 * ENUMS
 *****************************************************************************/
#ifndef __sed1520_ENUM__
#define __sed1520_ENUM__
typedef enum SSed1520Side
{
	LEFT = 1,
	RIGHT = 2
}ESed1520Side;
#endif //__sed1520_ENUM__
/*****************************************************************************/

/*************************************************************************//**
 * STRUKTUREN
 *****************************************************************************/
#ifndef __sed1520_STRUCT__
#define __sed1520_STRUCT__
#endif //__sed1520_STRUCT__

/// @brief Struktur mit Funktionszeigern um den Datenbus und die
/// Steuerleitungen des SED1520-Controllers anzusprechen
typedef struct SSed1520{
	void (*WriteData)(__u8 u8Data);	///<Zeiger auf Funktion um Daten zu schreiben
	void (*WriteA0)(__u8 u8Data);		///<Zeiger auf Funktion um A0 zu verändern
	void (*WriteE0)(__u8 u8Data);		///<Zeiger auf Funktion um E0 zu verändern
	void (*WriteE1)(__u8 u8Data);		///<Zeiger auf Funktion um E1 zu verändern
	__u8 Pixelschwelle;					///<Alle Farbwerte größer dem hier angegebenen setzen einen Pixel.
}TSed1520;
/*****************************************************************************/

/*************************************************************************//**
 * VARIABLEN
 *****************************************************************************/
#ifndef __sed1520_VAR__
#define __sed1520_VAR__
#endif //__sed1520_VAR__
/*****************************************************************************/

/*************************************************************************//**
 * @brief Initialisiert die Grundfunktionen für den SED1520-Treiber
 *****************************************************************************/
void Sed1520Init(void);
/*****************************************************************************/

/*************************************************************************//**
 * @brief Gibt einen Zeiger auf die Struktur der Displaydaten zurück.
 * @param *strSed1520 Zeiger auf die Datenstruktur @see SSed1520
 *****************************************************************************/
void Sed1520GetDisplayStruct(TSed1520 **strSed1520);
/*****************************************************************************/

/*************************************************************************//**
 * @brief Initialisiert das Display
 *****************************************************************************/
void Sed1520InitDisplay(void);
/*****************************************************************************/

/*************************************************************************//**
 * @brief Schickt ein Kommando an das Display
 * @param u8Cmd Kommandobyte
 * @param eSide An welche Seite des Displays? @see SSed1520Side
 *****************************************************************************/
void Sed1520SendCmd(__u8 u8Cmd, ESed1520Side eSide);
/*****************************************************************************/

/*************************************************************************//**
 * @brief Schickt ein Datenbyte an das Display
 * @param u8Data Datenbyte
 * @param eSide An welche Seite des Displays? @see SSed1520Side
 *****************************************************************************/
void Sed1520SendData(__u8 u8Data, ESed1520Side eSide);
/*****************************************************************************/

/*************************************************************************//**
 * @brief Gibt ein Bitmap auf dem Display aus
 * @details Das Bitmap muss 122x32 Pixel gross sein. Es wird ein Byte pro
 * Pixel verwendet.
 * @param u8Bmp[32][122] Zeiger auf das Bitmap
 *****************************************************************************/
void Sed1520DrawBmp(__u8 u8Bmp[32][122]);
/*****************************************************************************/

/*************************************************************************//**
 * 
 *****************************************************************************/
/*****************************************************************************/

/*************************************************************************//**
 * 
 *****************************************************************************/
/*****************************************************************************/

#endif // __SED1520_H___
