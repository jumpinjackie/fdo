This version of the CPPUNIT code was built from the CPPUNIT 1.8.0 distribution 
(also in this folder).  It can be rebuilt by following these instructions:

1) Un-archive the source (WinZip can do this)
2) Open the examples\examples.dsw project, and allow it to be automatically
   converted to the 7.0 SLN format.
3) Remove the DSPPlugIn project (it only applies to VC++ 6.0)
4) Select all other projects and right-click for Properties
   a. Go to the C++ category and the General sub-category
   b. Change "Debug Information Format" to "Disabled"
   c. Hit OK
5) Pick Build > Batch Build... from the menu
   a. Click on the Select All button
   b. Click on the Build button

The contents of the "lib" folder came from code built by this process.

  - Stephen Penrod (December 2003)