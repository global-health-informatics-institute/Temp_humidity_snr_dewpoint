
#include <Wire.h>

#define WIFI_SSID "Fadenlauf-2"
#define WIFI_PASSWORD "watchout"

#define MQTT_HOST IPAddress(192, 168, 0, 59)
#define MQTT_PORT 1883

#define si7021 0x40 //set humidity sensor address

double Humidity = 0.00;
double Temperature = 0.00;
double Dew_point = 0.00;
long lastMsg = 0;
int X0,X1,temp;
double X,X_out;

const int MeasureHumid = 0xE5; //Command to sent to the si7021 to read the humidity 

bool WireEnd() {
  unsigned char err;
  err = Wire.endTransmission();
  if( err ) {
    Serial.print("Error: ");
    Serial.println(err);
  }
  return err;
}

double GetHumid(int SDA_Pin, int SLC_pin) {
  unsigned int data[2];
  Wire.begin(SDA_Pin,SLC_pin,10000);
  Wire.beginTransmission(si7021);
    Wire.write(0xF5);
  WireEnd();
  delay(20);
  Wire.requestFrom(si7021, 2);
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  unsigned int temp = ((data[0] << 8) + data[1]);
  float humidity = ((125.0 * temp) / 65536.0) - 6;
  return humidity;
}


double GetTemp(int SDA_Pin, int SLC_pin) {
  Wire.begin(SDA_Pin,SLC_pin,10000);
  Wire.beginTransmission(si7021);
  Wire.write(0xE3);
  Wire.endTransmission();
  Wire.requestFrom(si7021,2);
  if(Wire.available()<=2);{
    X0 = Wire.read();
    X1 = Wire.read();
    X0 = X0<<8;
    X_out = X0+X1;
  }
  /**Calculate temperature**/
  X=(175.72*X_out)/65536;
  X=X-46.85;
  return X;
}

void setup() {
  Serial.begin(115200);
}

void loop() {

  Humidity = GetHumid(22, 23);
  Temperature = GetTemp(22, 23);
  Dew_point = Temperature - ((100 - Humidity)/5);
  
  Serial.print("Temperature % C =" + String(Temperature));
  Serial.print(" Humidity % RH =" + String(Humidity));
  Serial.println(" Dew Point % DP =" + String(Dew_point));
  }
