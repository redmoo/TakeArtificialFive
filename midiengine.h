#ifndef MIDIENGINE_H
#define MIDIENGINE_H

#include <QObject>
#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "Winmm.lib")

class MidiEngine : public QObject
{
    Q_OBJECT

public:
    explicit MidiEngine(QObject *parent = 0);
    ~MidiEngine();

public:
    void setPatch(int patch, int instrument); //nared preverjanje pr igranju ce sploh obstaja patch

    void playNote(int note, int patch, int velocity = 100);
    void stopNote(int note, int patch);

private:
    int initialize();

private:
    HMIDIOUT device;
    int midi_port;
    int event_flag;

    union { unsigned long word; unsigned char data[4]; } message;

};

#endif // MIDIENGINE_H
