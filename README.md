# adriLogger

<pre>

    based one esp8266 framework
    https://github.com/AdriLighting/adri_tools_v2/
    
Dependency Graph
|-- (adri_tools_v2) 1.0.0
|   |-- (ESP8266WiFi) 1.0
|   |-- (LittleFS(esp8266)) 0.1.0
|-- (adri_logger) 1.0.0
|   |-- (adri_tools_v2) 1.0.0
|   |   |-- (ESP8266WiFi) 1.0
|   |   |-- (LittleFS(esp8266)) 0.1.0
|   |-- (ESP8266WiFi) 1.0
|   |-- (LittleFS(esp8266)) 0.1.0
</pre>

<pre>

    ADRI_LOG permet d"afficher avec Serial ou bien d'ecrire dans un fichier text 
    
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
    
</pre>