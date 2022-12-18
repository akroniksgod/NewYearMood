#define R_PIN 3
#define G_PIN 5
#define B_PIN 6

void setup() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
}

void loop() {
  // плавно проходимся по всем цветам
  static int counter = 0;
  counter += 10;
  colorWheel(counter);
  delay(100);
}

// включает цвет по цветовому колесу, принимает 0-1530
void colorWheel(int color) {
  byte _r, _g, _b;
  if (color <= 255) {                       // красный макс, зелёный растёт
    _r = 255;
    _g = color;
    _b = 0;
  }
  else if (color > 255 && color <= 510) {   // зелёный макс, падает красный
    _r = 510 - color;
    _g = 255;
    _b = 0;
  }
  else if (color > 510 && color <= 765) {   // зелёный макс, растёт синий
    _r = 0;
    _g = 255;
    _b = color - 510;
  }
  else if (color > 765 && color <= 1020) {  // синий макс, падает зелёный
    _r = 0;
    _g = 1020 - color;
    _b = 255;
  }
  else if (color > 1020 && color <= 1275) {   // синий макс, растёт красный
    _r = color - 1020;
    _g = 0;
    _b = 255;
  }
  else if (color > 1275 && color <= 1530) { // красный макс, падает синий
    _r = 255;
    _g = 0;
    _b = 1530 - color;
  }
  analogWrite(R_PIN, 255 - _r);
  analogWrite(G_PIN, 255 - _g);
  analogWrite(B_PIN, 255 - _b);
}