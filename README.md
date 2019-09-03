# A QuadTree visualization
This is a simple QuadTree algo implementation and visualization.  
Algorithm itself made in abstract way, with using any custom checker (function, that tells for each square does it need to divide).  
Then, I added some functions that gives you more convinient (but less exact) ways to check: by coords, for example.  
And then there is a visualization of the algorithm

## Build 
Download/build sfml for your OS, place it under ./SFML-2.5.1 (or change version/path in makefile)  
Then, make should build it  
Because it isn't static linked you need to make your OS "see" the lib files (google for your OS)  
Or you can create Visual Studio project, link SFML and just add all .cpp and .h files in it  
