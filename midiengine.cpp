#include "midiengine.h"

MidiEngine::MidiEngine(QObject *parent) :
    QObject(parent)
{
    initialize();
}

int MidiEngine::initialize()
{
    midiport = 0;

    // Open the MIDI output port
    flag = midiOutOpen(&device, midiport, 0, 0, CALLBACK_NULL); // NEKOC TO TUT ZAPRI!
    if (flag != MMSYSERR_NOERROR) {
      qDebug() << "Error opening MIDI Output.";
      return 1;
    }

    return 0;
}

MidiEngine::~MidiEngine()
{
    flag = midiOutClose(device);
}

void MidiEngine::setPatch(int patch, int instrument)
{
    message.data[0] = 0xC0|patch;
    message.data[1] = instrument;
    message.data[2] = 0;
    message.data[3] = 0;

    flag = midiOutShortMsg(device, message.word);
}

void MidiEngine::playNote(int note, int patch, int velocity)
{
    message.data[0] = 0x90|patch;  // MIDI note-on message (requires to data bytes)
    message.data[1] = note;    // MIDI note-on message: Key number (60 = middle C)
    message.data[2] = velocity;   // MIDI note-on message: Key velocity (100 = loud)
    message.data[3] = 0;     // Unused parameter

    flag = midiOutShortMsg(device, message.word);
    if (flag != MMSYSERR_NOERROR) {
       qDebug() << "Warning: MIDI Output is not open.\n";
    }
}

void MidiEngine::stopNote(int note, int patch)
{
    message.data[0] = 0x80|patch;
    message.data[1] = note;
    message.data[2] = 0;
    message.data[3] = 0;

    flag = midiOutShortMsg(device, message.word);
    if (flag != MMSYSERR_NOERROR) {
       qDebug() << "Warning: MIDI Output is not open.\n";
    }
}
