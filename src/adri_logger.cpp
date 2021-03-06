
// region ################################################ 
/**
 * @file    adri_logger.cpp
 *
 * @brief   fournit des fonctionnalités utiles pour déboguer les croquis via "printf" ou "LITTLEFS"
 * @see     
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */

/*
    Utiliser la macro ADRI_LOGV ou ADRI_LOG

                region  lvl     trace   param   args
    ADRI_LOG(   parm_d, parm_c, parm_b, parm_a, ...)

    region  : attribuer un seemble de ADRI_LOG a la meme region, et de pouvoir gerer indivuduelement avec la classe "adriToolsLogger"
    lvl     : atribbuer un lvl associer a la position ou se trouve ADRI_LOG au sein de la fonction a debuger
    trace   : permet d'afficher la MACRO __LINE__ et ARDUINOTRACE_FUNCTION_NAME
    param   : "%s   - %d  - etcc"
    args    :  char - int - etcc
    

    ADRI_LOG permet d"afficher avec Serial ou bien d'ecrire dans un fichier text manager avec adriToolsLogger
     

    adriToolsLogger

    lvl
        0 start     t
        1 end       b
        2 random
    region
        -1 other region
        -2 dont print
        >=0 region via "adriToolsLogger_region"

        by mod 
            printer_displayMod_set
                0   all
                1   all + only header bt       
                2   all + only header t       
                3   only region
                4   only region + only header bt
                5   only region + only header t
                6   no region
                7   no region + only header bt
                8   no region + only header t
                9   only bt
                10  only t  

            printer_displayMod_set
                print et littfs  via printer_displayMod_pos

        by all region
            printer_display_toggle
                activ/desactiv  print et littfs 
            spiff_toggle
                activ/desactiv  littfs

        by region
            activateByRegion_toggle    
                print et littfs 
            activateByRegion_toggleAddLine
                littfs
            activateByRegion_toggleSerial
                print

*/    
// endregion >>>> 


    #if defined(ESP8266)
        #define FILESYSTEM LittleFS
    #elif defined(ESP32)
        #define FILESYSTEM SPIFFS
    #else
    #endif


#include "adri_logger.h"
#include <adri_tools_v2.h>


// region ################################################ color
#define ADRITOOLSLOGGER_COLOR_BLACK "30"
#define ADRITOOLSLOGGER_COLOR_RED "31"     // ERROR
#define ADRITOOLSLOGGER_COLOR_GREEN "32"   // INFO
#define ADRITOOLSLOGGER_COLOR_YELLOW "33"  // WARNING
#define ADRITOOLSLOGGER_COLOR_BLUE "34"
#define ADRITOOLSLOGGER_COLOR_MAGENTA "35"  // CONFIG
#define ADRITOOLSLOGGER_COLOR_CYAN "36"     // DEBUG
#define ADRITOOLSLOGGER_COLOR_GRAY "37"     // VERBOSE
#define ADRITOOLSLOGGER_COLOR_WHITE "38"
#define ADRITOOLSLOGGER_SECRET_BEGIN "\033[5m"
#define ADRITOOLSLOGGER_SECRET_END "\033[6m"

#define ADRITOOLSLOGGER_COLOR(COLOR) "\033[0;" COLOR "m"
#define ADRITOOLSLOGGER_BOLD(COLOR) "\033[1;" COLOR "m"
#define ADRITOOLSLOGGER_RESET_COLOR "\033[0m"
static const char *LOG_LEVEL_COLORS[] = {
    "",                                            // NONE
    ADRITOOLSLOGGER_BOLD(ADRITOOLSLOGGER_COLOR_RED),       // ERROR
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_YELLOW),   // WARNING
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_GREEN),    // INFO
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_MAGENTA),  // CONFIG
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_CYAN),     // DEBUG
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_GRAY),     // VERBOSE
    ADRITOOLSLOGGER_COLOR(ADRITOOLSLOGGER_COLOR_WHITE),    // VERY_VERBOSE
};    
// endregion >>>> color


char* adriLogger_buffer1;

//################# adriToolsLogger_region
//################################################

// region ################################################  region
/**
 * @brief      contructor
 * 
 * @author     AdriLighting      
 * @date       06/02/2021 12:46:34
 * 
 * @param[in]  name  nom de la region      
 */
adriToolsLogger_region::adriToolsLogger_region(String name){
    _name = name;
    ADRI_LOGV(-1, 2, 2, _state, "%s", _name.c_str());
}
/**
 * @brief      active/desactive totalement la region
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger_region::toggle(){
    _state = !_state;
    ADRI_LOGV(-1, 2, 2, _state, "%s", _name.c_str());    
}   
/**
 * @brief       active/desactive "LITTLEFS"
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger_region::toggleAddLine(){
    _addLine = !_addLine;
    ADRI_LOGV(-1, 2, 2, _addLine, "%s", _name.c_str());    
} 
/**
 * @brief       active/desactive "printf"
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger_region::toggleSerialPrint(){
    _serial = !_serial;
    ADRI_LOGV(-1, 2, 2, _serial, "%s", _name.c_str());    
}   
/**
 * @brief      get the value of _addLine
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @return     _addLine
 */
boolean adriToolsLogger_region::statuAddLine(){
    return _addLine;
}  
/**
 * @brief     get the value of _state
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @return     _state
 */  
boolean adriToolsLogger_region::statu(){
    return _state;
}  
/**
 * @brief      get the value of _serial
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @return     _serial
 */
boolean adriToolsLogger_region::statuSerialPrint(){
    return _serial;
}  
// endregion >>>> region


//################# adriToolsLogger
//################################################
//
// region ################################################ consctructor
adriToolsLogger * adriToolsLogger_ptr = nullptr;
/**
 * @brief      ptr ver la dernierre instance de la classe "adriToolsLogger"
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @return     ptr
 */
adriToolsLogger * adriToolsLogger_ptrGet(){
    if (adriToolsLogger_ptr == nullptr) adriToolsLogger_ptr = new adriToolsLogger();
    return adriToolsLogger_ptr;
}
adriToolsLogger::adriToolsLogger(){
    adriToolsLogger_ptr = this;
    adriLogger_buffer1 = new char[255];
    ADRI_LOG(-1, 2, 2, "", "");
}   
adriToolsLogger::adriToolsLogger(int size){
    adriToolsLogger_ptr = this;
    adriLogger_buffer1 = new char[size];
    ADRI_LOGV(-1, 2, 2, size, "", "");
    ADRI_LOG(-1, 2, 2, "", "");
}   
void adriToolsLogger::setup(){
    spiff_setup();
}   
// endregion >>>> consctructor

// region ################################################ activateByVariable
/**
 * @brief      printf des regions et de leur statu
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::activateByRegion_print(){
    for (int i = 0; i < _activateByRegion_cnt; ++i)
    {
        ADRI_LOG(-1, 2, 0, "[%-5d][%-25S][state: %d][_addLine: %d][_serial: %d]", 
            i, 
            _activateByRegion[i]->_name.c_str(),
            _activateByRegion[i]->_state,
            _activateByRegion[i]->_addLine,
            _activateByRegion[i]->_serial
            );
    }
}
/**
 * @brief      ajoute une region, crée une instance "activateByRegion_add" 
  *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  pos   The position
 */
void adriToolsLogger::activateByRegion_add(String name){
    ADRI_LOG(-1, 2, 2, "pos:%d - name:%s", _activateByRegion_cnt, name.c_str());
    _activateByRegion[_activateByRegion_cnt] = new adriToolsLogger_region(name);
    _activateByRegion_cnt++;
}
/**
*   @brief      active/desactive totalement la region voullu
*/
void adriToolsLogger::activateByRegion_toggle(int pos){
    _activateByRegion[pos]->toggle();    
} 
/**
 * @brief      active/desactive "littlefs" pour la region voullue
 *
 * @param[in]  pos   poistion opur le region "_activateByRegion[pos]"
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::activateByRegion_toggleAddLine(int pos){
    _activateByRegion[pos]->toggleAddLine();    
} 
/**
 * @brief      active/desactive "printf" pour la region voullue
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  pos   poistion opur le region "_activateByRegion[pos]"
 */ 
void adriToolsLogger::activateByRegion_toggleSerial(int pos){
    _activateByRegion[pos]->toggleSerialPrint();    
} 
/**
 * @brief     obtenir si la region peut "printf" et "littlefs"
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  pos   poistion opur le region "_activateByRegion[pos]"
 *
 * @return     return boolean
 */
boolean adriToolsLogger::activateByRegion_stau(int pos){
    return _activateByRegion[pos]->statu();
} 
/**
 * @brief     obtenir si la region peut "littlefs"
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  pos   poistion opur le region "_activateByRegion[pos]"
 *
 * @return     return boolean
 */ 
boolean adriToolsLogger::activateByRegion_statuAddLine(int pos){
    return _activateByRegion[pos]->statuAddLine();
}
/**
 * @brief     obtenir si la region peut "printf"
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  pos   poistion opur le region "_activateByRegion[pos]"
 *
 * @return     return boolean
 */    
boolean adriToolsLogger::activateByRegion_statuSerialPrint(int pos){
    return _activateByRegion[pos]->statuSerialPrint();
}     
// endregion >>>> activateByVariable

// region ################################################ tools
/**
 * @brief      création du tempstamp
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @return     string
 */
String adriToolsLogger::timeStamp()
{
  char result[20];
  snprintf(result, 20, "%lu", millis());
  return String(result);
}
/**
 * @brief      recherche des similitude dans une chaine de caractére
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  str    le caractére a chercher
 * @param[in]  sfind  la chaine a analyser
 *
 * @return     le nombre de similitude
 */
int adriToolsLogger::strContains(char str, const char *sfind) {
    int result = 0;
    int len = strlen(sfind);
    for (int i = 0; i < len; ++i) {   
        if ( sfind[i] == str ) result++;

    }   
    return result; 
}    
// endregion >>>> tools

// region ################################################ display mod && lvl

/**
 * @brief       active  le "printf" et "littfs" uniquement pour le level voullu pour toutes les region
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]   lvl   level voullu
 */
void adriToolsLogger::printer_displayMod_set(int lvl){
    printer_displayMod_pos = lvl;
    ADRI_LOGV(-1, 2, 2, printer_displayMod_pos, "", "");  
}    
// endregion >>>> display mod

// region ################################################ toggle
/**
 * @brief      acitve/desactive le timestamp
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::printer_timestamp_toggle(){
    printer_timestamp = !printer_timestamp;
    ADRI_LOGV(-1, 2, 2, printer_timestamp, "", "");  
}
/**
 * @brief      activer/desactive le freeheap
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::printer_heap_toggle(){
    printer_heap = !printer_heap;
    ADRI_LOGV(-1, 2, 2, printer_heap, "", "");  
}
/**
 * @brief      active/desactive le "printf" et "littfs" pour toutes les region
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::printer_display_toggle(){
    printer_display = !printer_display;
    ADRI_LOGV(-1, 2, 2, printer_display, "", "");  
}
/**
 * @brief      active/desactive le "littfs" pour toutes les region
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_toggle(){
    spiff_sav = !spiff_sav;
    ADRI_LOGV(-1, 2, 2, spiff_sav, "", ""); 	
}
/**
 * @brief      active/desactive par region < 0 le "littlfs"
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiffAddLine_otherRegion_toggle(){
    spiff_addLine_otherRegion = !spiff_addLine_otherRegion;
    ADRI_LOGV(-1, 2, 2, spiff_addLine_otherRegion, "", "");     
}
/**
 * @brief      active/desactive par region < 0 le "printf"
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::seriealPrint_otherRegion_toggle(){
    seriealPrint_otherRegion = !seriealPrint_otherRegion;
    ADRI_LOGV(-1, 2, 2, seriealPrint_otherRegion, "", "");     
}
// endregion >>>> toggle

// region ################################################ menu
/**
 * @brief      fonction pour le keyboard, active/desactive totalement la region voullu
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_region(String cmd, String value){
    adriToolsLogger * _looger = adriToolsLogger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    _looger->activateByRegion_toggle(pos);
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      fonction pour le keyboard, active/desactive "littlefs" pour la region voullue
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_regionAddLine(String cmd, String value){
    adriToolsLogger * _looger = adriToolsLogger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    _looger->activateByRegion_toggleAddLine(pos);
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      fonction pour le keyboard, active/desactive "printf" pour la region voullue
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_regionSerialPrint(String cmd, String value){
    adriToolsLogger * _looger = adriToolsLogger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    _looger->activateByRegion_toggleSerial(pos);
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      fonction pour le keyboard, commnds menu
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  cmd    The command
 * @param[in]  value  The value
 *
 * @return     ""
 */
String adriToolLogger_serialMenu_cmd(String cmd, String value){
    adriToolsLogger * _looger = adriToolsLogger_ptrGet();
    ADRI_LOGP(false, -1, -1, 0, "<<<", "");
    ADRI_LOGP(false, -1, 0, 2,"","");
    ADRI_LOGP(false, -1, 2, 2, "cmd: %s - value: %S", cmd.c_str(), value.c_str());  
    int pos = value.toInt();
    if (value == "a") {
        _looger->serial_menu();
    } else if (value == "z") {
        _looger->printer_display_toggle();
    } else if (value == "e") {
        _looger->printer_heap_toggle();
    } else if (value == "r") {
        _looger->printer_timestamp_toggle();
    } else if (value == "t") {
        _looger->spiff_toggle();
    } else if (value == "y") {
        _looger->spiff_readCurrent();
    } else if (value == "u") {
        _looger->spiff_removeCurrent();
    } else if (value == "i") {
        _looger->spiffAddLine_otherRegion_toggle();  
    } else if (value == "o") {
        _looger->seriealPrint_otherRegion_toggle();           
    } else if (value == "p") {
        _looger->activateByRegion_print();            
    } else {
        _looger->printer_displayMod_set(pos);  
    }
    ADRI_LOGP(false, -1, 1, 2,"","");
    ADRI_LOGP(false, -1, -1, 0, ">>>", "");
    return "";
}
/**
 * @brief      printf des comandes disponnible
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::serial_menu() {
    adri_toolsV2 * _ptr = adri_toolsv2Ptr_get();
    if (_ptr->tempStrArray != nullptr) delete[] _ptr->tempStrArray;
    int size = 23;
    _ptr->tempStr_sizeSet(size);
    _ptr->tempStrArray   	= new String[size];

    _ptr->tempStrArray[0]	= "\nPrinter lvl\n";
    _ptr->tempStrArray[1]	= _ptr->info_parm(F("0"),	F("all"), 10);
    _ptr->tempStrArray[2]	= _ptr->info_parm(F("1"),	F("all + only header bt "), 10);
    _ptr->tempStrArray[3]	= _ptr->info_parm(F("2"),	F("all + only header t "), 10);
    _ptr->tempStrArray[4]	= _ptr->info_parm(F("3"),	F("only region"), 10);
    _ptr->tempStrArray[5]	= _ptr->info_parm(F("4"),	F("only region + only header bt"), 10);
    _ptr->tempStrArray[6]	= _ptr->info_parm(F("5"),	F("only region + only header t"), 10);
    _ptr->tempStrArray[7]	= _ptr->info_parm(F("6"),	F("no region"), 10);
    _ptr->tempStrArray[8]	= _ptr->info_parm(F("7"),	F("no region + only header bt"), 10);
    _ptr->tempStrArray[9]	= _ptr->info_parm(F("8"),	F("no region + only header t"), 10);
    _ptr->tempStrArray[10]	= _ptr->info_parm(F("9"),	F("only bt"), 10);
    _ptr->tempStrArray[11]	= _ptr->info_parm(F("10"),	F("only t"), 10);
    _ptr->tempStrArray[12]	= "\nMenu\n";
    _ptr->tempStrArray[13]	= _ptr->info_parm(F("a"),	F("menu"), 10);
    _ptr->tempStrArray[14]	= _ptr->info_parm(F("z"),	F("printer_display_toggle"), 10);
    _ptr->tempStrArray[15]	= _ptr->info_parm(F("e"),	F("printer_heap_toggle"), 10);
    _ptr->tempStrArray[16]	= _ptr->info_parm(F("r"),	F("printer_timestamp_toggle"), 10);
    _ptr->tempStrArray[17]	= _ptr->info_parm(F("t"),	F("spiff activate"), 10);
    _ptr->tempStrArray[18]	= _ptr->info_parm(F("y"),	F("spiff read"), 10);
    _ptr->tempStrArray[19]	= _ptr->info_parm(F("u"),	F("spiff remove"), 10);    
    _ptr->tempStrArray[20] 	= _ptr->info_parm(F("i"),	F("toggle spifAddline lvl -1"), 10);
    _ptr->tempStrArray[21] 	= _ptr->info_parm(F("o"),	F("toggle serialPrint lvl -1"), 10);
    _ptr->tempStrArray[22] 	= _ptr->info_parm(F("p"),   F("print region"), 10);
    _ptr->tempStr_print();        
}    
// endregion >>>> menu

// region ################################################ count
void adriToolsLogger::region_start(int lvl, boolean print){
    _region_start = true;
    // if (print) ADRI_LOG(lvl, -2, 0, "----<<<", "");
}
void adriToolsLogger::region_end(int lvl, boolean print){
     _region_start = false;
     _region_start_pos = 0;
     for (int i = 0; i < 10; ++i)
     {
         _region_start_func[i] = "";
     }
    // if (print) ADRI_LOG(lvl, -2, 0, "---->>>", "");
}    
void adriToolsLogger::printer_title(int lvl, String func, const char * color, const char * color2, String & result, boolean incr) {
        String title = "[n]";
        switch (lvl) {
            case 0:
                if (!_region_start) title = "[n]";
                else {
                    title = "["+String(_region_start_pos)+"]";
                    if (incr) {
                        _region_start_func[_region_start_pos] = func;
                        _region_start_pos++;                        
                    }
                }
            break;
            case 1:
                if (!_region_start) title = "[n]";
                else {
                    for (int i = 0; i < 10; ++i)
                    {
                        if (_region_start_func[i] == func) title = "["+String(i)+"]";
                    }
                    if(title=="")title = "[n]";
                }        

            break;
            default:title = "[n]";break;
        }      
        result = title; 
}
// endregion >>>> count

// region ################################################ print

/**
 * @brief      fonction pour determiner si un "printf" ou "littlefs" est possible
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  region  The region
 * @param[in]  lvl     The level
 *
 * @return     true si un "printf" ou "littlefs" est possible
 */
boolean adriToolsLogger::printer_displayMod(int region, int lvl){
    if (!printer_display) return false;
    if (region == -2) return false; 

    if (region >= 0) {
        if (!adriToolsLogger_ptrGet()->activateByRegion_stau(region)) return false;

        if (printer_displayMod_pos == 6) return false;
        if (printer_displayMod_pos == 7) {
            if (lvl < 0) return true;
            if ((lvl != 0) && (lvl != 1))   return false;
        }
        if (printer_displayMod_pos == 8) {
            if (lvl < 0) return true;
            if (lvl!=0) return false;
        } 
    } else {
        if (printer_displayMod_pos == 3) return false;
        if (printer_displayMod_pos == 4) {
            if (lvl < 0) return true;
            if ((lvl != 0) && (lvl != 1)) return false;
        }
        if (printer_displayMod_pos == 5) {
            if (lvl < 0) return true;            
            if (lvl!=0) return false;
        }        
    }

    if (printer_displayMod_pos == 9) {
        if (lvl < 0) return true;  
        if ((lvl != 0) && (lvl != 1)) return false;
    }
    if (printer_displayMod_pos == 10) {
        if (lvl < 0) return true;  
        if (lvl!=0) return false;
    } 

    return true;
}   

/**
 * @brief      fonction final pour le "printf" et "littfs"
 * @details    ajouter timestamp et freeheap
 * @details    check si la region est activer
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  addLine  check if can print with "littfls"
 * @param[in]  region   The region
 * @param      buffer   The buffer
 */
void adriToolsLogger::printer_serial(boolean addLine, int region, int lvl, String func, char * buffer){
    String          result  = String(buffer);
    unsigned int    len     = result.length();
    char            bufferPrint[len + 255];
    int             heap    = ESP.getFreeHeap();
    String          cTtime;
    // adri_toolsv2Ptr_get()->ntpTime_getTime(cTtime);
    cTtime = adri_toolsv2Ptr_get()->om_time();
    const char      * color  = LOG_LEVEL_COLORS[0];
    const char      * color2 = LOG_LEVEL_COLORS[6];
    String title = "[n]";
    printer_title(lvl, func, (char*)"", (char*)"", title, false);

    if ((printer_timestamp) && (printer_heap)) {
        fssprintf(bufferPrint, "[%-15s][%-8d]%s", cTtime.c_str(), heap, buffer);
    }
    else {
        if (printer_timestamp)  fssprintf(bufferPrint, "[%-15s]%s", cTtime.c_str(), buffer);
        if (printer_heap)       fssprintf(bufferPrint, "[%-8d]%s", heap, buffer);
    } 

    if (spiff_sav && addLine) {
        if (region >= 0) {
             if (adriToolsLogger_ptrGet()->activateByRegion_statuAddLine(region)) {spiff_addLine(title+String(bufferPrint));}
        } else {
            if (spiff_addLine_otherRegion) spiff_addLine(title+String(bufferPrint));
        }
    }
    boolean sPrint = false;
    if (region >= 0) {
         if (adriToolsLogger_ptrGet()->activateByRegion_statuSerialPrint(region)) {sPrint = true;}
    } else {
        if (seriealPrint_otherRegion) sPrint = true;
    }
    if (sPrint) {

        String fBuffer = String(bufferPrint);
        Serial.println(title+fBuffer);

        if (adri_toolsv2Ptr_get() != nullptr) {
            if (adri_toolsv2Ptr_get()->_telnetPut != NULL) {
                int pos = 0;
                switch (lvl) {
                    case -1:
                        pos = 7; 
                    break; 
                    case -2:
                        pos = 6; 
                    break;                                            
                    case 0:
                        pos = 2;
                        color2 = LOG_LEVEL_COLORS[1];
                    break;
                    case 1:
                        pos = 2;
                        color2 = LOG_LEVEL_COLORS[1];
                    break;
                    case 2:
                        pos = 3;
                    break;                    
                    default:
                        pos = 7;
                    break;
                }
                color = LOG_LEVEL_COLORS[pos];
                title  = "[n]";
                printer_title(lvl, func, color2, color, title, true);                
                fssprintf(bufferPrint, "%s%s%s%s\n", color2, title.c_str(), color, fBuffer.c_str());
                adri_toolsv2Ptr_get()->_telnetPut(String(bufferPrint));
            }
        }


  
    }
}    
// endregion >>>> print

// region ################################################ printer line
void esp_log_print_lines(boolean addLine, int region, int lvl, int mod, String pLine, String func, String var, const char *tag){
    
adriToolsLogger * _looger = adriToolsLogger_ptrGet();
String      pTag        = String(tag);
int         searchCR    = 0;
            searchCR    = _looger->strContains('\n', tag);    
String      varP        = var;
String      title       = "";


switch (lvl) {
    case 0:
        title = "[<<START>>]";
    break;
    case 1:
        title = "[<<END>>]";    
    break;
    default:break;
}    

if (pLine != "") {

    int sizeMax = 90;
    if (func == "") sizeMax = 35;
    int size = pLine.length();
    while (size<sizeMax){
        pLine+=" ";
        size = pLine.length();
    }       

    if (pTag != "")     {
        String cr = "";
        int len = strlen(tag);
        if (searchCR > 0)   {
            if ((pTag.substring(len-1) == "\n") && (pTag.substring(0, 1) == "\n")  ) {
                pTag.remove(len-1, 1);
                pTag.remove(0, 1);
                cr = "\n";
            } else {
                if (pTag.substring(0,1)     == "\n") {pTag.remove(0, 1);}
                if (pTag.substring(len-1)   == "\n") {pTag.remove(len-1, 1);}
                cr = "\n";
            }
            if (varP != "") {
                fssprintf(adriLogger_buffer1, "[%s][%-25s]%s%s\t[%s]", 
                        pLine.c_str(), 
                        varP.c_str(),
                        title.c_str(), 
                        cr.c_str(),
                        pTag.c_str()
                    );
            } else {
                fssprintf(adriLogger_buffer1, "[%s]%s%s\t[%s]", 
                        pLine.c_str(), 
                        title.c_str(), 
                        cr.c_str(),
                        pTag.c_str()
                    );                    
            } 
        }  else {
            if (varP != "") {                 
                fssprintf(adriLogger_buffer1, "[%s][%-25s]%s[%s]", 
                        pLine.c_str(), 
                        varP.c_str(),
                        title.c_str(), 
                        pTag.c_str()
                    ); 
            } else {

                fssprintf(adriLogger_buffer1, "[%s]%s[%s]", 
                        pLine.c_str(), 
                        title.c_str(), 
                        pTag.c_str()
                    ); 
            }
        }
    } else {
        if (varP != "") {fssprintf(adriLogger_buffer1, "[%s][%-25s]%s", pLine.c_str(), varP.c_str(), title.c_str()); }
        else {fssprintf(adriLogger_buffer1, "[%s]%s", pLine.c_str(), title.c_str()); }
    }       
    
}
else {

    if (pTag != "")     {
        String cr = "";
        int len = strlen(tag);
        if (searchCR > 0)   {
            if ((pTag.substring(len-1) == "\n") && (pTag.substring(0, 1) == "\n")  ) {
                pTag.remove(len-1, 1);
                pTag.remove(0, 1);
                cr = "\n";
            } else {
                if (pTag.substring(0,1)     == "\n") {pTag.remove(0, 1);}
                if (pTag.substring(len-1)   == "\n") {pTag.remove(len-1, 1);}
                cr = "\n";
            }
            if (varP != "") {
                fssprintf(adriLogger_buffer1, "[%-25s]%s%s\t[%s]", 
                        varP.c_str(),
                        title.c_str(), 
                        cr.c_str(),
                        pTag.c_str()
                    );
            } else {
                fssprintf(adriLogger_buffer1, "%s%s\t[%s]", 
                        title.c_str(), 
                        cr.c_str(),
                        pTag.c_str()
                    );                    
            } 
        }  else {
            if (varP != "") {
                fssprintf(adriLogger_buffer1, "[%-25s]%s[%s]", 
                        varP.c_str(),
                        title.c_str(), 
                        pTag.c_str()
                    ); 
            } else {
                fssprintf(adriLogger_buffer1, "%s[%s]", 
                        title.c_str(), 
                        pTag.c_str()
                    ); 
            }
        }
    } else {
        if (varP != "") {fssprintf(adriLogger_buffer1, "[%-25s]%s", varP.c_str(), title.c_str()); }
        else {fssprintf(adriLogger_buffer1, "%s",  title.c_str()); }
    } 
}

_looger->printer_serial(addLine, region, lvl, func, adriLogger_buffer1 );
}
// endregion >>>> printer line

// region ################################################ esp_log_printf_
void  esp_log_printf_(int region, int lvl, int mod, String line, String func, String var, const char *tag) {
if (!adriToolsLogger_ptrGet()->printer_displayMod(region, lvl)) return;
String pLine = "";
switch (mod) {
    case 1:
        pLine = line;
        break;
    case 2:
        pLine = line + " " + func;       
        break;
    default:
        break;
}
esp_log_print_lines(true, region, lvl, mod, pLine, func, var, tag );
}
void  esp_log_printf_(int region, int lvl, int mod, String line, String func, const char *tag) {
if (!adriToolsLogger_ptrGet()->printer_displayMod(region, lvl)) return;
String pLine = "";
switch (mod) {
    case 1:
        pLine = line;
        break;
    case 2:
        pLine = line + " " + func;       
        break;
    default:
        break;
}
esp_log_print_lines(true, region, lvl, mod, pLine, func, (char*)"", tag );
} 
void  esp_log_printf_(boolean addLine, int region, int lvl, int mod, String line, String func, const char *tag) {
if (!adriToolsLogger_ptrGet()->printer_displayMod(region, lvl)) return;
String pLine = "";
switch (mod) {
    case 1:
        pLine = line;
        break;
    case 2:
        pLine = line + " " + func;       
        break;
    default:
        break;
}
esp_log_print_lines(addLine, region, lvl, mod, pLine, func, (char*)"", tag );
}     
// endregion >>>> 

// region ################################################ SPIFF
/**
 * @brief      initialize le fichier log
 * @details    utiliser pour chaque demmarage de l'esp, inscript dans le fichier log la raisson du dernier reset de l'esp 
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_setup(){


    ADRI_LOG(-1, 0, 2,"","");

    // Dir dir = LittleFS.openDir(spiff_folder);

    // int totalFile = 0;
    // while (dir.next()) {
    //     if (dir.isDirectory()) {

    //     } else  {
    //      totalFile++;
    //     }
    // }

    // spiff_fileName = String(totalFile+1) + ".txt";

    spiff_fileName = "log.txt";

    spiff_addLine("\n");
    adri_toolsV2 * _tools = adri_toolsv2Ptr_get();
    _tools->ESP_boot_info();
    int size = _tools->tempStrArraySize;
    for (int i = 0; i < size; ++i)
    {
        spiff_addLine(_tools->tempStrArray[i]);
    }

    ADRI_LOG(-1, 1, 2,"","");
}   
/**
 * @brief      stock le fichier log dans une string
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[int/out]  result  totalitée du fichier log
 */
void adriToolsLogger::spiff_get(String & ret){
    // ADRI_LOG(-1, 0, 2,"","");

    String lines = "";

    File file = FILESYSTEM.open(spiff_folder+"/"+spiff_fileName, "r");

    if (file) {
        while (file.position()<file.size()) {
            String xml = file.readStringUntil('\n');
            if (xml != "") {
                lines += xml + "\n";
            }
        }
        file.close(); 
    }

    ret = lines;

    // ADRI_LOG(-1, 1, 2,"","");
}  
/**
 * @brief      ajoute un ligne au fichier log
 *
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 * @param[in]  result  la ligne a ajouter
 */
void adriToolsLogger::spiff_addLine(String result){
    // String old;
    // spiff_get(old);
    File file = FILESYSTEM.open(spiff_folder+"/"+spiff_fileName, "a");
    file.println(result);
    file.close();

}
/**
 * @brief      printf du fichier log
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_readCurrent(){
    ADRI_LOG(-1, 0, 2, "", "");

    fsprintf("\n[spiff_readCurrent]\n"); 
    char buffer[512];
    String print = "";
    int nbr = 0;
    File file = FILESYSTEM.open(spiff_folder+"/"+spiff_fileName, "r");
    if (file) {
        while (file.position()<file.size()) {
            String xml = file.readStringUntil('\n');
            if (xml != "") {
                // Serial.println(xml);
                sprintf(buffer, "[%5d] %s", nbr, xml.c_str());
                print += String(buffer) + "\n";
                fsprintf("%s\n", buffer);
                delay(5);
                nbr++;
            }
        }
        file.close(); 
    }
    ADRI_LOG(-1, 1, 2, "", "");
}
/**
 * @brief      supprime le fichier log de la mem spiff
 * 
 * @author     AdriLighting   
 * @see        https://github.com/AdriLighting/adri_tools_v2
 * @date       06/02/2021 12:46:34
 *
 */
void adriToolsLogger::spiff_removeCurrent(){
    FILESYSTEM.remove(spiff_folder+"/"+spiff_fileName);
}
    
// endregion >>>> SPIFF
