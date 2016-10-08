#include <Arduino.h>
#include "notes.h";

// Initialize static variables
char Notes::scaleMode = SM_MODAL;
char Notes::tone = 60;
char Notes::scaleIndex = 0;
const char *Notes::scaleNames[NUM_SCALES] = {
    "Major",
    "Dorian",
    "Phrygian",
    "Lydian",
    "Mixolydian",
    "Minor",
    "Locrian",
    "Chromatic"
};
const char Notes::scaleIntervals[NUM_SCALES][NUM_STRINGS - 1] = {
//   1 2 3 4 5 6 7 1 2 3 4 5
    {2,2,1,2,2,2,1,2,2,1,2,2},      // Major
    {2,1,2,2,2,1,2,2,1,2,2,2},      // Dorian
    {1,2,2,2,1,2,2,1,2,2,2,2},      // Phrygian
    {2,2,2,1,2,2,1,2,2,2,2,1},      // Lydian
    {2,2,1,2,2,1,2,2,2,2,1,2},      // Mixolydian
    {2,1,2,2,1,2,2,2,2,1,2,2},      // Minor
    {1,2,2,1,2,2,2,2,1,2,2,2},      // Locrian
    {1,1,1,1,1,1,1,1,1,1,1,1},      // Chromatic
};

char *Notes::getToneName(char tone) {
    switch(tone % NUM_TONES) {
        case 0: return "C";
        case 1: return "C#";
        case 2: return "D";
        case 3: return "D#";
        case 4: return "E";
        case 5: return "F";
        case 6: return "F#";
        case 7: return "G";
        case 8: return "G#";
        case 9: return "A";
        case 10: return "A#";
        case 11: return "B";
        case 12: return "C";
    }
    return "N/A";
}

char Notes::getOctaveName(char tone) {
    return (tone + 3) / NUM_TONES;
}


char Notes::getTone() { return tone; }
char Notes::getScaleIndex() { return scaleIndex; }

void Notes::incrementTone() {
    if(tone >= 127) {
        tone = 0;
    } else {
        tone++;
    }
}

void Notes::decrementTone() {
    tone--;
    if(tone < 0) {
        tone = 127;
    }
}

void Notes::incrementOctave() {
    int ret = tone + NUM_TONES;
    if(ret > MAX_TONE) {
        ret = ret - MAX_TONE - 5;
        if(ret < 0) {
            ret += NUM_TONES;
        }
    }
    tone = ret;
}

void Notes::decrementOctave() {
    int ret = tone - NUM_TONES;
    if(ret < 0) {
        ret = MAX_TONE + (tone - NUM_TONES) + 5;
        if(ret > MAX_TONE) {
            ret -= NUM_TONES;
        }
    }
    tone = ret;
}

void Notes::incrementScaleIndex() {
    scaleIndex++;
    scaleIndex %= NUM_SCALES;
}

void Notes::decrementScaleIndex() {
    if(scaleIndex == 0) {
        scaleIndex = NUM_SCALES - 1;
    } else {
        scaleIndex--;
    }
}

char *Notes::getCurrentScaleName() {
    return scaleNames[scaleIndex];
}

/**
 * Compute the correct midi note based on the scale mode
 * and string number
 */
char Notes::getMidiNoteForString(char string) {
    // get the scale intervals
    int actual = tone;
    for(int i = 0 ; i < string ; i++) {
        actual += scaleIntervals[scaleIndex][i];
    }
    actual %= (MAX_TONE + 1);
    return (char)actual;
}
