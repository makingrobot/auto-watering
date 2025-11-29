#ifndef _SENSOR_VALUE_H
#define _SENSOR_VALUE_H

#include <vector>
#include <string>

class SensorValue {
public:
    const int intValue() const { return int_val_; }
    void setIntValue(int int_val) { int_val_ = int_val; }
    
    const std::vector<int>& intList() const { return int_list_; }
    void setIntList(const std::vector<int>& int_list) { int_list_ = int_list; }
    
    const int floatValue() const { return float_val_; }
    void setFloatValue(int float_val) { float_val_ = float_val; }
    
    const std::vector<float>& floatList() const { return float_list_; }
    void setFloatList(const std::vector<float>& float_list) { float_list_ = float_list; }
    
    const std::string& stringValue() const { return string_val_; }
    void setStringValue(const std::string& string_val) { string_val_ = string_val; }


private:
    int int_val_;
    std::vector<int> int_list_;

    float float_val_;
    std::vector<float> float_list_;
    
    std::string string_val_;

};

#endif //_SENSOR_VALUE_H