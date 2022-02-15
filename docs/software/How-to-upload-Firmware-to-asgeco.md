Overview
--------

Steps
-----

This procedure assumes there is a gnu tftp executable in /opt/local/bin.

### The manual way

1.  Build firmware as if with UNO board
2.  Open Terminal, go to
    
    [?](#)
    
    `/Users/afm/Documents/Arduino/asgeco3/asgeco3/Builds`
    
3.  Execute
    
    [?](#)
    
    `/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-objcopy -j .text -j .data -O binary embeddedcomputing.elf embeddedcomputing.bin`
    
4.  Make sure firmware is less than 28,672 bytes
    
    [?](#)
    
    `ls -l embeddedcomputing.bin`
    
5.  Upload to arduion
    
    [?](#)
    
    `/opt/local/bin/tftp -vm binary asgeco -c put embeddedcomputing.bin & asgeco2.sh restart`
    

### The quick way

This will do everything for me. Will not upload, if firmware is too big.

[?](#)

`cd /Users/afm/Documents/Arduino/asgeco3/asgeco3/Builds`

`/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avr-objcopy -j .text -j .data -O binary embeddedcomputing.elf embeddedcomputing.bin`

`if` `[` `28672` `-gt $(ls -l embeddedcomputing.bin | awk` `' {print $5} '``) ]; then /opt/local/bin/tftp -vm binary asgeco -c put embeddedcomputing.bin & asgeco2.sh restart;` `else` `echo` `"Cannot upload: Firmware too big"``; fi`

See Also
--------