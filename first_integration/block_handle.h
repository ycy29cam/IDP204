// colour detection function
bool colour_detect(){
    const int InfraredSensorPin = 8;
    pinMode(InfraredSensorPin,INPUT);

    // red returns true, black returns false
    if (digitalRead(InfraredSensorPin)){
        return true;
    }
    else{
        return false;
    }
}