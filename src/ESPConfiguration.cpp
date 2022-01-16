#include "ESPConfiguration.h"

std::vector<IoTSensor*> iotSensors;
ModuleInfo getAPI(String subtype, String params);

void configure(String path) {
    File file = seekFile(path);
    file.find("[");
    while (file.available()) {
        String jsonArrayElement = file.readStringUntil('}') + "}";
        if (jsonArrayElement.startsWith(",")) {
            jsonArrayElement = jsonArrayElement.substring(1, jsonArrayElement.length());  //это нужно оптимизировать в последствии
        }
        
        String subtype;
        if (!jsonRead(jsonArrayElement, F("subtype"), subtype)) {  //если нет такого ключа в представленном json или он не валидный
            SerialPrint(F("E"), F("Config"), "json error " + subtype);
            continue;
        } else {
            myIoTSensor = (IoTSensor*)getAPI(subtype, jsonArrayElement).apiToComponent;
            if (myIoTSensor) {
                iotSensors.push_back(myIoTSensor);
                createWidget(jsonArrayElement);
            }
        }
    }
    file.close();
}