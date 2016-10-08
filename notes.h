#ifndef NOTES_H
#define NOTES_H

#define NUM_STRINGS     13
#define NUM_TONES       12
#define NUM_OCTAVES     11
#define MAX_TONE        127

#define SM_MODAL 0
#define NUM_SCALES 8

class Notes {
    public:
        static char *getToneName(char tone);
        static char getOctaveName(char tone);
        static char getMidiForNote(char tone, char octave);
        static char scaleMode;

        static char getTone();
        static char getScaleIndex();
        static char *getCurrentScaleName();
        static char getMidiNoteForString(char string);

        static void incrementTone();
        static void decrementTone();
        static void incrementOctave();
        static void decrementOctave();
        static void incrementScaleIndex();
        static void decrementScaleIndex();
    private:
        static char tone;

        static char scaleIndex;
        static const char *scaleNames[NUM_SCALES];
        static const char scaleIntervals[NUM_SCALES][NUM_STRINGS - 1];
};

#endif
