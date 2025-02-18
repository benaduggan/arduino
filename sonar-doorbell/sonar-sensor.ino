#include <WiFi.h>
#include <HTTPClient.h>

long duration;
int distance;
const int trigPin = 26;
const int echoPin = 25;

const char *ssid = "<ssid>";
const char *password = "<password>";

const char *webhookUrl = "http://<home-assistant-thing>/api/webhook/<secret>";
const int DISTANCE_THRESHOLD = 20;
unsigned long lastWebhookTime = 0;
const long webhookInterval = 10000; // 10 seconds

const int REQUIRED_CONSECUTIVE_HITS = 3;
int consecutiveHits = 0; // Counter for consecutive detections

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
}

int getReading()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration / 74 / 2; // Convert to inches
  Serial.printf("Distance: %d inches\n", distance);

  return distance;
}

void sendToWebhook(int distance)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(webhookUrl);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"distance\":" + String(distance) + "}";

    int httpResponseCode = http.POST(payload);
    Serial.printf("HTTP Response Code: %d\n", httpResponseCode);

    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}

void loop()
{
  int inches = getReading();

  if (inches < DISTANCE_THRESHOLD)
  {
    consecutiveHits++;

    if (consecutiveHits >= REQUIRED_CONSECUTIVE_HITS)
    {
      unsigned long currentMillis = millis();

      if (currentMillis - lastWebhookTime >= webhookInterval)
      {
        Serial.println("Sending webhook...");
        sendToWebhook(inches);
        lastWebhookTime = currentMillis;
        consecutiveHits = 0;  // Reset after sending the webhook
      }
      else
      {
        Serial.println("Motion detected, but waiting for cooldown.");
      }
    }
    else
    {
      Serial.printf("Motion detected, %d/%d readings below threshold.\n", consecutiveHits, REQUIRED_CONSECUTIVE_HITS);
    }
  }
  else
  {
    consecutiveHits = 0; // Reset the count if a reading is above the threshold
  }
}
