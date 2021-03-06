Current Release: 2.11 (9/25/2017)

[![Gitter chat](https://badges.gitter.im/gitterHQ/gitter.png)](https://gitter.im/GOMC_WSU/Lobby?utm_source=share-link&utm_medium=link&utm_campaign=share-link)
[![Build Status](https://travis-ci.org/GOMC-WSU/GOMC.svg?branch=master)](https://travis-ci.org/GOMC-WSU/GOMC)

We recommend the [GOMC Project Website](http://gomc.eng.wayne.edu/ "GOMC Website") and the [user manual](http://gomc.eng.wayne.edu/GOMC_files/GOMC_Manual.pdf "User Manual") for further information and examples.

GOMC - GPU Optimized Monte Carlo
============

BUILDING GOMC ON LINUX:
----------------
   1. Give execution permission using "chmod u+x metamake.sh"
   2. In the base directory type "./metamake.sh"
   3. Step 2 should generate all the executables in "bin" directory
   If you want to compile it in OMP mode:
   4. Open CMakeCache.txt file in "bin" directory and modify
      "CMAKE_CXX_FLAGS_RELEASE:STRING:-O3 -DNDEBUG"
      to "CMAKE_CXX_FLAGS_RELEASE:STRING=-O3 -qopenmp -DNDEBUG"
   5. Type "make" to initiate the build process.

   If you compile the code in OMP mode:
   You can set the number of the threads using the +pN argument, where N is the number of threads.
   For example:
      ./GOMC_Serial_XXXX +p4 in.dat
      Which will run 4 threads and reads input file "in.dat".

   NOTES:
      Building GOMC requires cmake, available at http://www.cmake.org and
      in most Linux package repositories (as cmake).

BUILDING GOMC ON WINDOWS:
-----------------
   1. Open the Windows-compatible CMake GUI.
   2. Set the Source Folder to the GOMC root folder.
   3. Set the build Folder to your Build Folder.
   4. Click configure, select your compiler/environment
   5. Wait for CMake to finish the configuration.
   6. Click configure again and click generate.
   7. Open the CMake-generated project/solution etc. to the desired IDE
      (e.g Visual Studio).
   8. Using the solution in the IDE of choice build GOMC per the IDE's
      standard release compilation/executable generation methods.

   NOTES:
      You can also use CMake from the Windows command line if its directory is
      added to the PATH environment variable.

CONFIGURING CMAKE:
   CMake has a ridiculously expansive set of options, so this document will
   only reproduce the most obviously relevant ones.
   When possible, options should be passed into CMake via command line options
   rather than the CMake.txt file.

   SET BUILD MODE:

      -DCMAKE_BUILD_TYPE=<None|Debug|Release|ReleaseWithDebInfo|MinSizeRel>

      NOTES:
         Typically it is advised to build in "Release" mode.

      	 If bugs are encountered or if a modification is being tested, please
      	 compile in "Debug" mode, which includes the debug symbols which can
      	 be used with gdb and valgrind to track errors.

	 The debugging code is MUCH slower than the release code (as debugger
      	 compiled codes typically are).  Be advised to check that you're
      	 compiling in "Release" build mode if the code seems slow.

   SET ENSEMBLES:

      -DENSEMBLE_GEMC=<On|Off>
      -DENSEMBLE_NVT=<On|Off>
      -DENSEMBLE_GCMC=<On|Off>
      -DENSEMBLE_NPT=<On|Off>

      NOTES:
         CMake will prepare build rules for all ensembles that are On.

      	 To build all ensembles simply leave them at their default
      	 value ("On").

	 GEMC includes both GEMC-NVT and GEMC-NPT implementations, which
      	 are switchable via the configuration file.

   SET COMPILER:

      -DCMAKE_CXX_COMPILER=Compiler

      where Compiler is a valid compiler name or full path. If this is
      omitted, CMake will select your default C++ compiler (usually gcc).

      NOTES:

         It is recommended to use the Intel Compiler and linking
      	 tools (icc/icpc/etc.).  They significantly outperform the default
      	 GNU compiler tools and are available for free for academic use
      	 with registration.
