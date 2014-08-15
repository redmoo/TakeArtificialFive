#include "midi_engine.h"

#include <QDebug>

MidiEngine::MidiEngine(QObject *parent) :
    QObject(parent)
{
    initialize();
}

int MidiEngine::initialize()
{
    midi_port = 0;

    // Open the MIDI output port
    event_flag = midiOutOpen(&device, midi_port, 0, 0, CALLBACK_NULL); // NEKOC TO TUT ZAPRI!
    if (event_flag != MMSYSERR_NOERROR) {
      qDebug() << "Error opening MIDI Output.";
      return 1;
    }

    return 0;
}

MidiEngine::~MidiEngine()
{
    event_flag = midiOutClose(device);
}

void MidiEngine::setPatch(int patch, int instrument)
{
    message.data[0] = 0xC0|patch;
    message.data[1] = instrument;
    message.data[2] = 0;
    message.data[3] = 0;

    event_flag = midiOutShortMsg(device, message.word);
}

void MidiEngine::playNote(int note, int patch, int velocity)
{
    message.data[0] = 0x90|patch;  // MIDI note-on message (requires to data bytes)
    message.data[1] = note;    // MIDI note-on message: Key number (60 = middle C)
    message.data[2] = velocity;   // MIDI note-on message: Key velocity (100 = loud)
    message.data[3] = 0;     // Unused parameter

    event_flag = midiOutShortMsg(device, message.word);
    if (event_flag != MMSYSERR_NOERROR) {
       qDebug() << "Warning: MIDI Output is not open.\n";
    }
}

void MidiEngine::stopNote(int note, int patch)
{
    message.data[0] = 0x80|patch;
    message.data[1] = note;
    message.data[2] = 0;
    message.data[3] = 0;

    event_flag = midiOutShortMsg(device, message.word);
    if (event_flag != MMSYSERR_NOERROR) {
       qDebug() << "Warning: MIDI Output is not open.\n";
    }
}

void MidiEngine::exportTrack(QVector<Entity *> entities, int speed, int seed) // kok generacij pa ker seed je pa kok entitet
{ // zgleda kokr da odseka zadno noto.. cudn
    MIDIfile file;
    file.AddLoopStart();

    for(int i = 0; i < entities.size(); i++) {
        file[0].Patch(entities.at(i)->getPatch(), entities.at(i)->getInstrument());
    }

    int track_length = entities.at(0)->getTrack().length();
    QVector<int> keys_on(entities.size(), -1);

    for (int loops = 0; loops < 1; loops++) {

        for (int beat = 0; beat < track_length; beat++) {

            for (int channel = 0; channel < entities.size(); channel++) {

                int note = entities.at(channel)->getTrack().at(beat);
                int vol = 100; // TODO: RECORD DYNAMICS!

                //temp
                //if (channel == 1) vol = 40;
                //if (channel == 3) vol = 80;


                if (note == midi_state::SUSTAIN) continue;

                file[0].KeyOff(channel, keys_on[channel], 0x20);
                keys_on[channel] = -1;

                if (note == midi_state::PAUSE) continue;

                file[0].KeyOn(channel, keys_on[channel] = note, vol);

            }
            file[0].AddDelay(speed);

        }
        if(loops == 0) file.AddLoopEnd();
    }
    file.Finish();

    QString file_name = QString("seed(%1)_generations(%2)_entities(%3).mid").arg(seed).arg(1).arg(entities.size());
    FILE* fp = std::fopen(file_name.toUtf8().constData(), "wb");
    std::fwrite(&file.at(0), 1, file.size(), fp);
    std::fclose(fp);

}
