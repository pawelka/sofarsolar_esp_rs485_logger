#ifndef InverterValue_h
#define InverterValue_h

#include <Arduino.h>
#include <ModbusMaster.h>

class InverterValue {

  private:
    String valueName;
    double divider;
    String unit;
    int byteno;
    int size;
  public:
    InverterValue(String valueName, int byteno, double divider, String unit);
    InverterValue(String valueName, int byteno, int size, double divider, String unit);
    String getValueName();
    double getValue(ModbusMaster* master); 
    String getStrValue(ModbusMaster* master); 
    String getUnit();    

};

#endif