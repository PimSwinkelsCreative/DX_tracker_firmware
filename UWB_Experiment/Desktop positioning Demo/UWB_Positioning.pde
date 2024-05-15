import processing.serial.*;

//define the anchors and their locations.
Serial anchor1;
Serial anchor2;
float anchor1Pos[] = { - 0.9, 0};
float anchor2Pos[] = {0.9, 0};
float zeroPosition[] = {0, 0};

float setupWidth = (abs(anchor1Pos[0]-anchor2Pos[0]));

// distance to anchor parameters
float anchor1Distance = 0;
float anchor2Distance = 0;

//max distances to accept data:
float maxDistance = 10.0;

String tagId;
float tagPosition[] = {0, 0};

void setup() {
  size(800, 800);


  //open the serial ports:
  anchor1 = new Serial(this, Serial.list()[0], 115200);
  anchor2 = new Serial(this, Serial.list()[1], 115200);
  anchor1.bufferUntil('\n');
  anchor2.bufferUntil('\n');
}

void draw() {
  setZeroPosition();
  background(255);

  //draw the distances:
  stroke(0);
  noFill();

  circle(zeroPosition[0]+metersToPixels(anchor1Pos[0]), zeroPosition[1]+metersToPixels(anchor1Pos[1]), 2*metersToPixels(anchor1Distance));
  circle(zeroPosition[0]+metersToPixels(anchor2Pos[0]), zeroPosition[1]+metersToPixels(anchor2Pos[1]), 2*metersToPixels(anchor2Distance));

  //draw the anchors:
  stroke(255, 0, 0);
  fill(255, 0, 0);
  circle(zeroPosition[0]+metersToPixels(anchor1Pos[0]), zeroPosition[1]+metersToPixels(anchor1Pos[1]), metersToPixels(0.1));
  circle(zeroPosition[0]+metersToPixels(anchor2Pos[0]), zeroPosition[1]+metersToPixels(anchor2Pos[1]), metersToPixels(0.1));

  //draw the tag:
  stroke(0, 100, 0);
  fill(0, 100, 0);
  circle(zeroPosition[0]+metersToPixels(tagPosition[0]), zeroPosition[1]+metersToPixels(tagPosition[1]), metersToPixels(0.1));


  fill(0);
  text(anchor1Distance, zeroPosition[0]+metersToPixels(anchor1Pos[0]), zeroPosition[1]+metersToPixels(anchor1Pos[1]));
  text(anchor2Distance, zeroPosition[0]+metersToPixels(anchor2Pos[0]), zeroPosition[1]+metersToPixels(anchor2Pos[1]));
}

void serialEvent(Serial thisPort) {
  //parse the incoming string:
  String inputString = thisPort.readString();
  print("input: ");
  println(inputString);
  String inputFields[] = split(inputString, '\t');
  int currentAnchor = 0;
  for (int i = 0; i < inputFields.length; i++) {
    String inputParameter[] = split(inputFields[i], ':');
    if (inputParameter[0].equals("ANCHORID")) {
      currentAnchor = Integer.parseInt(inputParameter[1]);
    } else if (inputParameter[0].equals("TAGID")) {
      tagId = inputParameter[1];
    } else if (inputParameter[0].equals("DIST")) {
      float inDistance = Float.parseFloat(inputParameter[1]);
      if (inDistance<maxDistance) {
        if (currentAnchor ==1) {
          anchor1Distance = inDistance;
        } else if (currentAnchor == 2) {
          anchor2Distance = inDistance;
        }
      }
      //calculate the tag position based on the new info:
      calculateTagPosition();


      //print all data:
      String message = "";
      message+="tagId ";
      message+=tagId;
      message+="\tanchor1 distance: ";
      message+=anchor1Distance;
      message+="\tanchor2 distance: ";
      message+=anchor2Distance;
      message+="\tTag coordinates: ";
      message+=tagPosition[0];
      message+=" ";
      message+=tagPosition[1];
      println(message);
    }
  }

  //TODO: Handle the "NEW" and "DEL" keywords
}

float metersToPixels(float meters) {
  float scalingFactor = width/(2*setupWidth);
  return meters*scalingFactor;
}

void setZeroPosition() {
  zeroPosition[0] = width/2;
  zeroPosition[1] = height/10;
}

void calculateTagPosition() {
  if (anchor1Distance<0||anchor2Distance<0||anchor1Distance>10||anchor2Distance>10) return;

  //update the target position usong the law of consines
  float cos_anchor1 = (anchor1Distance*anchor1Distance+setupWidth*setupWidth-anchor2Distance*anchor2Distance)/(2*anchor1Distance*setupWidth);
  float sin_anchor1 = sqrt(1-cos_anchor1*cos_anchor1);
  tagPosition[0] = anchor1Pos[0]+anchor1Distance*cos_anchor1;
  tagPosition[1] = anchor1Pos[1]+anchor1Distance*sin_anchor1;
}
