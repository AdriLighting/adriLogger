
#ifndef ADRI_LOGGER_H
	#define ADRI_LOGGER_H

	#include <arduino.h>

	extern char* adriLogger_buffer1;

	/**
	* @class	adriToolsLogger_region
	*
	* @brief    crée une region atribbuer a un ou plusieur "printf" ou "LITTLEFS"
	* @see		https://github.com/AdriLighting/adri_tools_v2
	*
	* @author	adrilighting
	* @date		05/02/2021 06:25:54
	*/
	class adriToolsLogger_region 
	{

		public:
			/** \brief active/desactive une totalement region */
			boolean _state 		= true;

			/** \brief active/desactive "LITTLEFS" */
			boolean _addLine 	= true;

			/** \brief active/desactive "printf" */
			boolean _serial 	= true;

			/** \brief nom d'affichage */
			String 	_name 		= "0";

			adriToolsLogger_region(String name);

			void toggle();
			void toggleAddLine();
			void toggleSerialPrint();
			boolean statu();
			boolean statuAddLine();
			boolean statuSerialPrint();
	};

	/**
	* @class	adriToolsLogger
	*
	* @brief	fournit des fonctionnalités utiles pour déboguer les croquis via "printf" ou "LITTLEFS"
	* @see		https://github.com/AdriLighting/adri_tools_v2
	*
	* @author	adrilighting
	* @date		05/02/2021 06:25:54
	*/
	class adriToolsLogger
	{
		/** \brief nbr de class "adriToolsLogger_region" ceer */
		int _activateByRegion_cnt = 0;
		/** \brief array cotenat les class "adriToolsLogger_region" */
		adriToolsLogger_region 	* _activateByRegion[10];

		/** \brief active/desactive le "printf" et "littfs" pour toute les region */
		boolean printer_display = true;

		/** \brief active  le "printf" et "littfs" uniquement pour le lvl selectionner pour toute les region */
		int printer_displayMod_pos 	= 0;

		/** \brief active/desactive par region < 0 le "printf"  */
		boolean seriealPrint_otherRegion = true;
		/** \brief active/desactive par region < 0 le "littfs"  */
		boolean spiff_addLine_otherRegion = true;

		/** \brief active/desactive le "littfs" pour toutes les region */
		boolean spiff_sav = false;

		/** \brief acitve/desactive le timestamp */
		boolean printer_timestamp = true;

		/** \brief activer/desactive le freeheap */
		boolean printer_heap = true;		

		/** \brief enplacement du doosier des log situé da la mem SPIFF */
		String spiff_folder = "/logger";

		/** \brief nom du fichier log situé da le dossier "spiff_folder" */
		String spiff_fileName = "";


	public:
		String _region_start_func[10];
		boolean _region_start = false;
		int _region_start_pos = 0;
		int _region_end_pos = 0;
		void region_start(int lvl, boolean print = false);
		void region_end(int lvl, boolean print = false);

		void serial_menu();

		void printer_serial(boolean addLine, int region, int lvl, String func, char * buffer);
		void printer_title(int lvl, String func, const char * color, const char * color2, String & result, boolean incr);
		
		void printer_timestamp_toggle();
		void printer_heap_toggle();

		void printer_display_toggle();

		void spiff_setup();
		void spiff_toggle();
		void spiff_readCurrent();
		void spiff_removeCurrent();
		void spiff_get(String & result);
		void spiff_addLine(String result);

		void spiffAddLine_otherRegion_toggle();
		void seriealPrint_otherRegion_toggle();

		adriToolsLogger();
		adriToolsLogger(int size);
		void setup();

		boolean printer_displayMod(int region, int lvl);
		void printer_displayMod_set(int lvl);

		String 	timeStamp();
		int 	strContains(char str, const char *sfind);


		void activateByRegion_print();

		void activateByRegion_add(String name);

		void activateByRegion_toggle(int pos);
		void activateByRegion_toggleAddLine(int pos);
		void activateByRegion_toggleSerial(int pos);

		boolean activateByRegion_stau(int pos);
		boolean activateByRegion_statuAddLine(int pos);
		boolean activateByRegion_statuSerialPrint(int pos);
	};

	adriToolsLogger * adriToolsLogger_ptrGet();
	String adriToolLogger_serialMenu_region(String cmd, String value);
	String adriToolLogger_serialMenu_regionAddLine(String cmd, String value);
	String adriToolLogger_serialMenu_regionSerialPrint(String cmd, String value);
	String adriToolLogger_serialMenu_cmd(String cmd, String value);	
#endif // LOGGER_H
