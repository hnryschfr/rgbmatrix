const int col1r = 4;
const int col2r = 7;
const int col3r = 10;
const int col4r = 13;
const int col1g = 3;
const int col2g = 6;
const int col3g = 9;
const int col4g = 12;
const int col1b = 2;
const int col2b = 5;
const int col3b = 8;
const int col4b = 11;

const int row1 = 14;
const int row2 = 15;
const int row3 = 16;
const int row4 = 17;
const int row5 = 18;
const int row6 = 19;
unsigned long t = 500;
unsigned long tr = 500;
unsigned long tg = 2 * tr;
unsigned long tb = 3 * tr;


const int row[] = {row6, row5, row4, row3, row2, row1};
const int colr[] = {col1r, col2r, col3r, col4r};
const int colg[] = {col1g, col2g, col3g, col4g};
const int colb[] = {col1b, col2b, col3b, col4b};
const int sizerow = sizeof(row) / sizeof(row[0]);
const int sizecol = sizeof(colr) / sizeof(colr[0]);
int offset = 0;
int active = 0;
int resetColor = 0;

typedef bool colorArray[24]; //deffinition of a type called a color array

struct image {
  colorArray r, g, b; // three color buffers
};

image imageData[2]; //current and next data in a sturcture of color buffers

int displayBuffer = 0;

bool rDeath = 0;
bool gDeath = 0;
bool bDeath = 0;

void setup() {
  Serial.begin(9600);

  //set pins to output
  for (int r = 0; r < sizerow; r++) {
    pinMode (row[r], OUTPUT); //Set all pins to output
    Serial.print(row[r]);
    Serial.println(" Output ");
  }
  for (int c = 0; c < sizecol; c++) {
    pinMode(colr[c], OUTPUT);
    pinMode(colg[c], OUTPUT);
    pinMode(colb[c], OUTPUT);
    Serial.print(colr[c]);
    Serial.println(" Output ");
  }
  //pick a random seed for each color
  imageData[0].b[random(0, 24)] = 1;
  imageData[0].r[random(0, 24)] = 1;
  imageData[0].g[random(0, 24)] = 1;

  tr = millis();
  tg = millis() + t;
  tb = millis() + t * 2;
}

void loop() {


  //setup new image: make a random place also have a color

  if (millis() > t) {
    bDeath = imageUpdate(imageData[displayBuffer].b, imageData[1 - displayBuffer].b, bDeath);
    gDeath = imageUpdate(imageData[displayBuffer].g, imageData[1 - displayBuffer].g, gDeath);
    rDeath = imageUpdate(imageData[displayBuffer].r, imageData[1 - displayBuffer].r, rDeath);

    //update what is being displayed
    displayBuffer = 1 - displayBuffer;
    //set new time
    t = millis() + 500;
  }


  //loop through images
  for (active = 0; active < sizerow; active++) {
    readLineMode();
  }
}

bool imageUpdate(const colorArray input, colorArray output, bool death) {
  //add dots to the left
  int total = 0;
  for (int y = 0; y < sizerow; y++) {
    for (int x = 0; x < sizecol; x++) {
      int sum = 0;
      if (y > 0) sum += input[x + (y - 1) * sizecol] == !death; //check upper neighbor for color disply
      if (y < sizerow - 1) sum += input[x + (y + 1) * sizecol] == !death; //check lower neighbor for color disply
      if (x > 0) sum += input[(x - 1) + y * sizecol] == !death; //check left neighbor for color disply
      if (x < sizecol - 1) sum += input[(x + 1) + y * sizecol] == !death; //check right neighbor for color disply
      sum += input[x + y * sizecol] == !death; //include current self

      if (sum > 0) {
        output[x + y * sizecol] = !death;
        total++;
      } else {
        output[x + y * sizecol] = death;
      }
    }
  }

  if (total == sizerow * sizecol) {
    //change death value
    death = !death;
    //set new random seed
    output[random(0, 24)] = !death;
  }
  return death;
}


void displayReset(colorArray output) {
  memset(output, 0, sizerow * sizecol);//resets things to 0 turns off all of one color
}

void readLineMode() {
  for (int c = 0; c < sizecol; c++) {
    digitalWrite(colr[c], imageData[displayBuffer].r[(c + active * sizecol)]);
    digitalWrite(colg[c], imageData[displayBuffer].g[(c + active * sizecol)]);
    digitalWrite(colb[c], imageData[displayBuffer].b[(c + active * sizecol)]);
    //  Serial.println(offset%(sizeimage1/sizecol));
    //      Serial.println(offset%(sizeimage1/sizecol));
  }
  digitalWrite(row[active], LOW);
  delay(1);
  digitalWrite(row[active], HIGH);
}

