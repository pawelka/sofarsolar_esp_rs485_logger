#include "InverterValue.h"

InverterValue::InverterValue(String valueName, int byteno, double divider, String unit) : InverterValue(valueName, byteno, 1, divider, unit){
}

InverterValue::InverterValue(String valueName, int byteno, int size, double divider, String unit){
    this->valueName = valueName;
    this->divider = divider;
    this->unit = unit;
    this->byteno = byteno;
    this->size = size;
}
String InverterValue::getValueName(){
    return this->valueName;
}

double InverterValue::getValue(ModbusMaster* master){
    if(size == 1){
        return (double) master->getResponseBuffer(this->byteno) / this->divider;
    }else{ //size 2
        return ((double) master->getResponseBuffer(this->byteno) * 256.0 +  (double) master->getResponseBuffer(this->byteno + 1)) / this->divider;
    }
}

String BYTE0_FAULTS[] = {"GridOVP", "GridUVP", "GridOFP", "GridUFP", "BatOVP"};

String InverterValue::getStrValue(ModbusMaster* master){
    if(this->byteno == -1){
        if(master->getResponseBuffer(0) == 0){
            if (master->getResponseBuffer(14) == 0 && master->getResponseBuffer(16) == 0 && master->getResponseBuffer(18) == 0){ //ac voltage zero
                return "off";
            }else if (master->getResponseBuffer(11) == 0){ //ac power zero
                return "standby";
            }else{
                return "normal"; 
            }
        }else{
            return "fault";
        }
    }else if(this->byteno == 0){
        int value = master->getResponseBuffer(0);
        String result = "";
        for(int i = 0; i < 5; i++){
            if( ((value >> i) & 0x1) == 1 ){
                if(result.length() > 0){
                    result +=",";
                }
                result += BYTE0_FAULTS[i];
            }
        }
        return result;
    }else{
        return "";
    }
}

String InverterValue::getUnit(){
    return this->unit;
};    