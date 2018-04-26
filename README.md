# HarmonAIze

**HarmonAIze** is a MIDI sequencer application built for the Winter 2018 senior course *Accessible Software Design* (EECS 498) at the University of Michigan. Features of the application include:

- MIDI recording via on-screen keyboard or hardware MIDI input
- MIDI playback via SFZ instruments
- Seconds/beats/time signature mappings, allowing music-based (i.e. BPM) manipulations, such as tempo adjustment
- Arrangement view for viewing the contents of the current project
- Save/load, undo/redo, font size adjustment.

In addition to these, HarmonAIze's "headliner" feature (and namesake) is an *accompaniment generator* which can intelligently generate accompaniments based on the user-created melody in the first track.

## Screenshot

<p align="center">
<img src="https://github.com/jonathonracz/harmonaize/blob/master/app/Design/screenshot.png?raw=true" width = 912 alt="Screenshot, as described above.">
</p>

## Technical Details

### Application

The application portion of HarmonAIze is built on the [JUCE](https://juce.com) framework, a C++ framework suited for building interactive multimedia applications for macOS, Windows, Linux, iOS, and Android. HarmonAIze is tested to work on macOS High Sierra 10.12.4 compiled with Xcode 9.3. The C++ style strictly follows the [JUCE style guide](https://juce.com/discover/stories/coding-standards) with US spelling conventions.

To work on the application, use JUCE's Projucer IDE project generator to open and edit [`/app/harmonaize.jucer`](https://github.com/jonathonracz/harmonaize/blob/master/app/harmonaize.jucer). Information about how to use the Projucer [can be found on the JUCE website](https://docs.juce.com/master/tutorial_new_projucer_project.html).

The application is structured with a strong model-view-controller paradigm; *every* object instance has its data backed by an abstract node in a [`ValueTree` object](https://github.com/jonathonracz/harmonaize/blob/master/app/Source/hmnz_ValueTreeObject.h). This greatly simplifies data management, particularly when it comes to undo/redo, save/reload, inter-object communication, event handling, and app-wide debugging situations. The implementation is based loosely on concepts delivered by developer David Rowland during his technical talk [*Using JUCE value trees and modern C++ to build large scale applications*](https://youtu.be/3IaMjH5lBEY) at the 2017 Audio Developer Conference in London.

In addition to the UI/main thread's strong MVC representation, there is a runtime-only representation reflected to the real-time audio thread whenever the main thread's model changes. This is thread-safe for reading by the audio thread, and can be seen in objects such as [`MidiMessage`](https://github.com/WeAreROLI/JUCE/blob/master/modules/juce_audio_basics/midi/juce_MidiMessage.h) (real-time) vs. [`MidiMessageModel`](https://github.com/jonathonracz/harmonaize/blob/master/app/Source/hmnz_MidiMessageModel.h) (model-backed), and [`MidiMessageSequence`](https://github.com/WeAreROLI/JUCE/blob/master/modules/juce_audio_basics/midi/juce_MidiMessageSequence.h) (real-time) vs. [`MidiMessageSequenceModel`](https://github.com/jonathonracz/harmonaize/blob/master/app/Source/hmnz_MidiMessageSequenceModel.h) (model-backed). These real-time objects are recreated in the main thread any time their model-backed counterparts are changed, and swapped out via a lock (as quickly as possible) as soon as they're ready ([example](https://github.com/jonathonracz/harmonaize/blob/85a1560e551819292168135404261e961942cb51/app/Source/hmnz_Track.cpp#L141)).

### Accompaniment Generation

HarmonAIze's accompaniment generation uses macOS's embedded Python 2.7 along with the [`mido`](https://github.com/olemb/mido) and [`mma`](https://www.mellowood.ca/mma/) modules. Communication between the two languages is accomplished with [`pybind11`](https://github.com/pybind/pybind11). Glue/initialization code can be found in [`harmonaize/harmonaize/midiinterchange/hmnz_MidiInterchange.h`](https://github.com/jonathonracz/harmonaize/blob/master/harmonaize/midiinterchange/hmnz_MidiInterchange.h).

## For Future Student Teams

Due to time and resource constraints, HarmonAIze is still largely incomplete and unsuitable for general distribution. The following is a list of features we'd like to have completed but didn't have time for:

- Metronome count in
- Arrangement view editing
- MIDI note editor
- Snapping/quantization
- Default instrument variation
- Asynchronous accompaniment generation (i.e. as-is now, but without blocking the UI thread)
- Audio file export
- Port to additional platforms

Good luck. All team members are still available via email if guidance is required, but please be specific when contacting us.

### Audio Programming Resources

Audio programming is a type of soft (some, like myself, would argue hard) [real-time programming](https://en.wikipedia.org/wiki/Real-time_computing#Real-time_in_digital_signal_processing) which is incredibly challenging, requiring deep knowledge of systems programming including concepts of multithreading, lock-free coding, strategic memory management and object lifetime. It is not for the faint of heart or those with severe senioritis. If you are interested in reading about concepts reflected in the design of HarmonAIze, here are a few resources reflected in its code, primarily in [`/app/Source/hmnz_PlaybackEngine.cpp`](https://github.com/jonathonracz/harmonaize/blob/master/app/Source/hmnz_PlaybackEngine.cpp):

[Real-time audio programming 101: time waits for nothing](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing) by Ross Bencia, AudioMulch

[C++ in the Audio Industry](https://youtu.be/boPEO2auJj4) by Timur Doumler, ROLI

[C++ in the Audio Industry, Episode II: Floating Atomics](https://youtu.be/2vmXy7znEzs) by Timur Doumler, ROLI

[The JUCE forum](https://forum.juce.com): many huge names in the audio software industry hang out here and are happy to exchange help and advice for non-trivial questions.

## Staff Feedback

> *"Excellent in all regards.  Really an excellent presentation and demo.  As well, the project is technically sophisticated and graphically stunning."\
\
\- Dr. David C., Course Professor*

> *"Your application is virtually flawless with a massive array of features, truly complex software and a really attractive UI. One of the best projects that this class has seen!"\
\
\- Adithya R., Teaching Assistant*

> *"Really successful application in terms of having a wide set of features packaged together in an attractive and complete manner. The accompaniment creation is really quite impressive, and the ability to modify instruments allows for additional creativity. Tools such as the metronome help provide additional practicality to the application. Working with Brad to confirm accessability and comfort for him across the board would lead to a tremendously useful offering for Brad and overall."\
\
\- Kayla F., Teaching Assistant*
