int colors[8][3] = {
  {0, 0, 0}, /* off */
  {20, 20, 20}, /* white */
  {20, 0, 0}, /* red */
  {20, 20, 0}, /* orange */
  {0, 20, 0}, /* green */
  {0, 20, 20}, /* turquoise */
  {0, 0, 20}, /* blue */
  {20, 0, 20} /* purple */
};

void setup()
{
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(A0, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  setColor(0);
}

bool leftEnabled = true, rightEnabled = true, buttonPress = false;
void loop()
{
  digitalWrite(8, !leftEnabled);
  digitalWrite(7, !rightEnabled);
  
  if(!digitalRead(9))
  {
    rightEnabled = !rightEnabled;
    while(!digitalRead(9));
    buttonPress = true;
  }
  if(!digitalRead(10))
  {
    leftEnabled = !leftEnabled;
    while(!digitalRead(10));
    buttonPress = true;
  }
  
  if(buttonPress)
  {
    Serial.print(leftEnabled);
    Serial.print(":");
    Serial.println(rightEnabled);
    delay(10); // debounce
    buttonPress = false;
  }
  
  String s = "";
  bool rec = false;
  while(Serial.available() > 0)
  {
    char c = Serial.read();
    s += c;
    rec = true;
  }
  if(rec)
  {
    //Serial.print("Color: ");
    //Serial.println(s);
    setColor(atoi(s.c_str()));
  }
}

void setColor(int col)
{
  analogWrite(3, 255 - colors[col][0]); // R
  analogWrite(5, 255 - colors[col][1]); // G
  analogWrite(6, 255 - colors[col][2]); // B
}
