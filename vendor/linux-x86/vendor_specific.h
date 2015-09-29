//
// Created by ebassetti on 23/08/15.
//

#ifndef GALILEO_TERREMOTI_VENDOR_SPECIFIC_H
#define GALILEO_TERREMOTI_VENDOR_SPECIFIC_H

#define WATCHDOG_FILE        "/tmp/galileo_watchdog"
#define DEFAULT_LOG_PATH     "/tmp/sketch.log"
#define WATCHDOG_LOG_PATH    "/tmp/watchdog.log"
#define DEFAULT_CONFIG_PATH  "/tmp/seismoconfig.txt"
#define CMD_PING             "ping -w 2 8.8.8.8 > /dev/null"
#define CALIBRATION_FILE     "/tmp/calibration.dat"

#endif //GALILEO_TERREMOTI_VENDOR_SPECIFIC_H