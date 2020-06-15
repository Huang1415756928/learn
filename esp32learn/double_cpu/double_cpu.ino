void setup() {
 
  Serial.begin(112500);
  delay(1000);
 
  Serial.print("Setup: Executing on core ");
  Serial.println(xPortGetCoreID());
 
  xTaskCreate(
                    genericTask,       /* Task function. */
                    "genericTask",     /* String with name of task. */
                    10000,             /* Stack size in words. */
                    NULL,              /* Parameter passed as input of the task */
                    2,                 /* Priority of the task. */
                    NULL);             /* Task handle. */
 delay(2000); 
 
}
 
void loop() {
  Serial.print("Main Loop: Executing on core ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}
 
void genericTask( void * parameter ){
    Serial.print("Created task: Executing on core ");
    Serial.println(xPortGetCoreID());
    vTaskDelete(NULL);
}
