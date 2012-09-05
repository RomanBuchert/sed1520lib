/*************************************************************************//**
 * @file sed1520.c
 *
 * @brief Grundfunktionen um ein Display mit einem SED1520-Controller
 * anzusprechen
 * @author Roman Buchert (roman.buchert@googlemail.com)
 *****************************************************************************/
/*****************************************************************************/

/*************************************************************************//**
 * INCLUDE-Dateien
 *****************************************************************************/
#include <sed1520.h>
/*****************************************************************************/

/*************************************************************************//**
 * DEFINES
 *****************************************************************************/
/*****************************************************************************/

/*************************************************************************//**
 * ENUMS
 *****************************************************************************/
/*****************************************************************************/

/*************************************************************************//**
 * STRUKTUREN
 *****************************************************************************/
/*****************************************************************************/

/*************************************************************************//**
 * VARIABLEN
 *****************************************************************************/
///Struktur mit den Daten um das Display anzusteuern
TSed1520 Sed1520;
/*****************************************************************************/

/*************************************************************************//**
 * @brief Initialisiert die Grundfunktionen für den SED1520-Treiber
 *****************************************************************************/
void Sed1520Init(void)
{
	bzero((TSed1520*) &Sed1520,sizeof(TSed1520));
}
/*****************************************************************************/

/*************************************************************************//**
 * @brief Gibt einen Zeiger auf die Struktur der Displaydaten zurück.
 * @param **strSed1520 Zeiger auf die Datenstruktur @see SSed1520
 *****************************************************************************/
void Sed1520GetDisplayStruct(TSed1520 **strSed1520)
{
	*strSed1520 = &Sed1520;
}
/*****************************************************************************/

/*************************************************************************//**
 * @brief Initialisiert das Display
 *****************************************************************************/
void Sed1520InitDisplay(void)
{
	__u16 u16Zaehler;
	__u8 u8Daten = 0x55;
	__u8 u8Page = 0;

	//Sende Resetsequenz
	Sed1520SendData(SED1520CMD_RESET1, LEFT | RIGHT);
	Sed1520SendData(SED1520CMD_RESET1, LEFT | RIGHT);
	Sed1520SendData(SED1520CMD_RESET1, LEFT | RIGHT);

	Sed1520SendCmd(SED1520CMD_RESET2, LEFT | RIGHT);

	//Duty Ratio Select
	Sed1520SendCmd(SED1520CMD_DUTY_32, LEFT | RIGHT);

	//Static Drive Off
	Sed1520SendCmd(SED1520CMD_STATIC_DRV_OFF, LEFT | RIGHT);

	//ADC Select
	Sed1520SendCmd(SED1520CMD_ADC_CCW, LEFT | RIGHT);

	//Display ON
	Sed1520SendCmd(SED1520CMD_DISPLAY_ON, LEFT | RIGHT);

	u8Daten = 0x55;
	for (u8Page=0; u8Page < 4; u8Page++)
	{
		//Display Startline
		Sed1520SendCmd(SED1520CMD_DISP_START_LINE | 0x00, LEFT | RIGHT);

		//Pageadress set
		Sed1520SendCmd(SED1520CMD_SET_PAGE0 | u8Page, LEFT | RIGHT);

		//Columnadress set
		Sed1520SendCmd(SED1520CMD_SET_COL_ADDR | 0x13, LEFT | RIGHT);

		//Read-Modify-Write
		Sed1520SendCmd(SED1520CMD_RMW_START, LEFT | RIGHT);

		//Schicke Daten an das Display
		for (u16Zaehler = 0; u16Zaehler < 61; u16Zaehler++)
		{
			Sed1520SendData(u8Daten, LEFT | RIGHT);
			u8Daten ^= 0xFF;
		}

		//Sende Ende
		Sed1520SendCmd(SED1520CMD_RMW_END, LEFT | RIGHT);
	}

}
/*****************************************************************************/

/*************************************************************************//**
 * @brief Schickt ein Kommando an das Display
 * @param u8Cmd Kommandobyte
 * @param eSide An welche Seite des Displays? @see SSed1520Side
 *****************************************************************************/
void Sed1520SendCmd(__u8 u8Cmd, ESed1520Side eSide)
{
	Sed1520.WriteE0(0);
	Sed1520.WriteE1(0);
	Sed1520.WriteA0(0);
	Sed1520.WriteData(u8Cmd);
	Sed1520.WriteE0(eSide & 0x01);
	Sed1520.WriteE1(eSide & 0x02 >> 1);
	Sed1520.WriteE0(0);
	Sed1520.WriteE1(0);
	Sed1520.WriteData(0);
}
/*****************************************************************************/

/*************************************************************************//**
 * @brief Schickt ein Datenbyte an das Display
 * @param u8Data Datenbyte
 * @param eSide An welche Seite des Displays? @see SSed1520Side
 *****************************************************************************/
void Sed1520SendData(__u8 u8Data, ESed1520Side eSide)
{
	Sed1520.WriteE0(0);
	Sed1520.WriteE1(0);
	Sed1520.WriteA0(1);
	Sed1520.WriteData(u8Data);
	Sed1520.WriteE0(eSide & 0x01);
	Sed1520.WriteE1((eSide & 0x02) >> 1);
	Sed1520.WriteE0(0);
	Sed1520.WriteE1(0);
	Sed1520.WriteA0(0);
	Sed1520.WriteData(0);
}
/*****************************************************************************/

/*************************************************************************//**
 * @brief Gibt ein Bitmap auf dem Display aus
 * @details Das Bitmap muss 122x32 Pixel gross sein. Es wird ein Byte pro
 * Pixel verwendet.
 * @param u8Bmp[32][122] Zeiger auf das Bitmap
 *****************************************************************************/
void Sed1520DrawBmp(__u8 u8Bmp[32][122])
{
	__u8 u8DataLeft, u8DataRight;
	__u8 u8Cntr;

	__u8 u8Page, u8Col;
	for(u8Page = 0; u8Page < 4; u8Page++)
 	{
 		__u8 u8Test;
		//Cursor Positionieren
 		//Pageadress set
 		Sed1520SendCmd(SED1520CMD_SET_PAGE0 | u8Page, LEFT | RIGHT);
 		//Columnadress set
 		Sed1520SendCmd(SED1520CMD_SET_COL_ADDR | 0x13, LEFT | RIGHT);
		//Read-Modify-Write
 		Sed1520SendCmd(SED1520CMD_RMW_START, LEFT | RIGHT);

 		for(u8Col = 0; u8Col < 61; u8Col++)
 		{
 			u8DataLeft = 0;
 			u8DataRight = 0;
 			//Datenbyte zusammenbauen
 			for(u8Cntr = 0; u8Cntr < 8; u8Cntr++)
 			{
 				u8Test = u8Bmp[(u8Page*8) + u8Cntr][u8Col];
 				u8DataLeft >>= 1;
				if (u8Bmp[(u8Page*8) + u8Cntr][u8Col] > Sed1520.Pixelschwelle)
 				{
 					u8DataLeft |= 0x80;
 				}

 				u8DataRight >>= 1;
 				if (u8Bmp[(u8Page<<3) + u8Cntr][61 + u8Col] > Sed1520.Pixelschwelle)
 				{
 					u8DataRight |= 0x80;
 				}
 			}
 			Sed1520SendData(u8DataLeft, LEFT);
 			Sed1520SendData(u8DataRight, RIGHT);
 		}
 		Sed1520SendCmd(SED1520CMD_RMW_END, LEFT | RIGHT);
 	}
}
/*****************************************************************************/

/*************************************************************************//**
 *
 *****************************************************************************/
/*****************************************************************************/

