### Windows API wrapper library

A c++ library that is supposed to wrap most of the common winapi functions.
The library exports header folders that include all the declarations of the library.

# How to use
In order to use the library, you need to compile it, take the .lib file, and include the headers you want from the header folders.
If you don't want to compile it you can just use the compiled version in git.

# For the moment
The api that the library implemented for the moment are:
-	IO api
-	Event api
-	Pipe api
-	Signal api
-	Wrapper for overlapped
-	Wrapper for named pipe server
# In the future 
In the future, the library will continue to add api for the already implemented function and will add api for new windows api.
