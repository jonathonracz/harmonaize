# harmonAIze

# GoatMix

Welcome to the GoatMix repository! Please keep in mind that audio plugins are extremely fickle and complex things when it comes to building, due to the massive number of dependencies, SDKs, and build gymnastics needed to compile them and make them run. That said, this document will attempt to explain as clearly and concisely as possible how to get started.

## Step 1: Have the correct build tools.
MobileMix is tested to build using Xcode 9.1 under macOS High Sierra (10.13). 

## Step 2: Clone the repository recursively.

If you do a simple `git clone`, the code **_will not work!_** You must clone recursively to also pull in the many dependencies. Note that downloading a `.zip` of `master` from the repository will not download any of the submodules. To clone recursively, use `git clone --recursive https://github.com/jonathonracz/harmonaize.git`

It will take a while to clone everything. Once complete, open `harmonaize/app/Builds/<platform of choice>/<IDE Project>`.

## Step 3: Build.

This is where things will either go without a hitch or crash and burn, mostly due to paths to SDKs (which are included as submodules) are not referenced correctly for whatever reason. In the case of simply building and side-loading the app onto a device, there shouldn't be any issues so long as you're on one of the supported platforms.

## Appendix

The "actual" source code, i.e. all the app's code, can be found in `/harmonaize/app/Source` and `/harmonaize/app/harmonaize`. The program is split into the app portion which contains all the code for the GUI frontend of the application and the harmonaize portion which contains all the code for the machine learning and backend. Statically linked dependencies are in `/harmonaize/app/Source/External` and `/harmonaize/harmonaize/Source/External` respectively.
