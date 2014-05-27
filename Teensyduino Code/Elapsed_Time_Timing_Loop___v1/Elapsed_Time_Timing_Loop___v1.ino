// create elapsedMillis outside loop(), to
// retain its value each time loop() runs.

elapsedMillis sinceTest1 = 0;
elapsedMillis sinceTest2 = 0;
elapsedMillis sinceTest3 = 0;

elapsedMicros totaltime = 0;
elapsedMicros runtime = 0;
long runtimetotal = 0;

void setup()   {                
  Serial.begin(115200);

  // prep idle and run time counters
  totaltime = 0;
  runtime = 0;
  runtimetotal = 0;
}

void loop() {
  runtime=0;
  if (sinceTest1 >= 1000) {
    // By subtracting the timer amount instead of resetting to zero
    // the timimg loop stays (close) to the original loop timimg
    sinceTest1 = sinceTest1 - 1000;
    Serial.println("Test1 (1 sec)");
    
    Serial.print("Run Time % = ");
    Serial.print((float)runtimetotal/totaltime*100);
    Serial.print("  Idle Time % = ");
    Serial.println(100-(float)runtimetotal/totaltime*100);
    Serial.print("totaltime=");
    Serial.print(totaltime,DEC);
    Serial.print("\t runtime=");
    Serial.print(runtime,DEC);
    Serial.print("\t runtimetotal=");
    Serial.println(runtimetotal,DEC);
    
    delay(500);
  }
  runtimetotal+=runtime;
  
  runtime=0;
  if (sinceTest2 >= 500) {
    sinceTest2 = sinceTest2 - 500;
    Serial.println("             Test2 (0.5 sec)");
    delay(2);
  }
  runtimetotal+=runtime;

  runtime=0;
  if (sinceTest3 >= 22) {
    sinceTest3 = sinceTest3 - 22;
    Serial.println("                            Test3 (22 m/sec)");
    delay(1);
  }
  runtimetotal+=runtime;
  
  delay(1000);

}

