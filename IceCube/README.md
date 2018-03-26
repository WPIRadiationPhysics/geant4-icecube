Geant4 Simulation
=================

Table of Contents:
------------------
1. GEOMETRY DEFINITION
2. PHYSICS LIST
3. ACTION INITALIZATION
4. PRIMARY GENERATOR
5. RUNS and EVENTS
6. HOW TO BUILD/RUN
7. VISUALIZATION
8. USER INTERFACES


1 GEOMETRY DEFINITION
---------------------
DetectorConstruction.cc defines geometry and logical volumes. 

1 km^3 cube of ice. T = 273.15 K, rho = 1 g/cm^3, etc


2 PHYSICS LIST
--------------
The particle's type and the physics processes which will be available are set in the FTFP\_BERT2.0 physics list. This physics list requires data files for electromagnetic and hadronic processes. See more on installation of the datasets in Geant4 Installation Guide,

Fritioff String Model with Bertini Cascade, validated for charged particles between 1 MeV and 100 GeV
 

3 ACTION INITALIZATION
----------------------
User action classes are defined in ActionInitialization.cc.  While in sequential mode the action classes are instatiated just once, via invoking the method `ActionInitialization::Build()` in multi-threading mode the same method is invoked for each thread worker and so all user action classes are defined thread-local.  A run action class is instantiated both thread-local and global; its instance is also created in the method `ActionInitialization::BuildForMaster()`, which is invoked only in multi-threading mode.

To be determined...


4 PRIMARY GENERATOR
------------------
The type of the particle and its energy are set in the main function and can be changed via the G4 built-in commands of the G4ParticleGun class.

Tau neutrino, 1 TeV incident from all directions (does full diameter of Earth play a role of comparable shielding?)
 	

5 RUNS and EVENTS
-----------------
A run is a set of events. The number of events set will define the statistical limitations to the results, but physical system definitions may be changed between runs.

1 run, 1 million events


6 HOW TO BUILD/RUN
------------------
Generate a Makefile, then compile the project into an executable:

    % mkdir build
    % cd build
    % cmake ..
    % make -jn
   
The program can be run with the following optional arguments:

    % ./IceCube [-n nEvents ] [-u UIsession] [-t nThreads]
   
The -t option is available only in multi-threading mode and it allows the user to override the Geant4 default number of threads. The number of threads can be also set via G4FORCENUMBEROFTHREADS environment variable which has the top priority.

Execute IceCube in the 'interactive mode' with visualization

    % ./IceCube

and type in the commands from run1.mac line by line:

    Idle> /tracking/verbose 1 
    Idle> /run/beamOn 1
    Idle> ...
    Idle> exit

or

    Idle> /control/execute myRun.mac
    ....
    Idle> exit

Execute G4Application in the 'interactive mode' with a selected UI session, e.g. tcsh

    % ./IceCube -u tcsh 


7 VISUALIZATION
---------------

The visualization manager is set via the G4VisExecutive class in the `main()` function in IceCube.cc. The initialisation of the drawing is done via a set of `/vis/` commands in the macro vis.mac. This macro is automatically read from the main function when used in interactive running mode.

By default, vis.mac opens an OpenGL viewer (`/vis/open OGL`). The user can change the initial viewer by commenting out this line and instead uncommenting one of the other `/vis/open` statements, such as HepRepFile or DAWNFILE (which produce files that can be viewed with the HepRApp and DAWN viewers, respectively).  Note that one can always open new viewers at any time from the command line.  For example, if you already have a view in, say, an OpenGL window with a name "viewer-0", then

    /vis/open DAWNFILE

then to get the same view

    /vis/viewer/copyView viewer-0

or to get the same view _plus_ scene-modifications

    /vis/viewer/set/all viewer-0

then to see the result

    /vis/viewer/flush

The DAWNFILE, HepRepFile drivers are always available (since they require no external libraries), but the OGL driver requires that the Geant4 libraries have been built with the OpenGL option.  For more information on visualization, including information on how to install and run DAWN, OpenGL and HepRApp, see the visualization tutorials:

[http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4DAWNTutorial.html](http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4DAWNTutorial.html)  
[http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4OpenGLTutorial.html](http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4OpenGLTutorial.html)  
[http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4HepRAppTutorial.html](http://geant4.slac.stanford.edu/Presentations/vis/G4[VIS]Tutorial/G4HepRAppTutorial.html)

The tracks are automatically drawn at the end of each event, accumulated for all events and erased at the beginning of the next run.


8 USER INTERFACES
------------------

The user command interface is set via the G4UIExecutive class in the `main()` function in IceCube.cc. The selection of the user command interface is then done automatically according to the Geant4 configuration or it can be done explicitly via the third argument of the G4UIExecutive constructor (see IceCube.cc).
