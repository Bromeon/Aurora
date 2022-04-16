# Aurora C++ Library

Aurora is an open-source C++ library providing various rather uncommon C++ utilities. Features include:

* Dynamic dispatch
* Named tuples
* Optional types
* Smart pointers with deep-copy semantics
* Basic template and preprocessor metaprogramming
* ...

For a full list of features as well as tutorials and API documentation, visit the [project homepage](http://www.bromeon.ch/libraries/aurora).

Aurora is a header-only library that has no external dependencies, therefore it can be directly used in any project, without the need for a build or installation step. It requires partial C++11 support.

The Aurora library is used in my other project [Thor](https://github.com/Bromeon/Thor), an extension of the multimedia library SFML.


## Development status

I am no longer actively developing Aurora. The library has reached a state where I'm relatively happy with it.
Some of Aurora's features are now available in Modern C++ in a more idiomatic way, however others (like `CopiedPtr`) are still very handy.
Due to its modularity and header-only approach, it should be easy to cherry-pick whatever you consider useful.


## License

Aurora is licensed under [zlib/libpng](http://opensource.org/licenses/zlib-license.php), which is very permissive. You can use the code in free and commercial products, open- or closed-source.
I would appreciate if you left a short note that you used one my libraries, but it's not required.


## Author and contact

Aurora has been developed by Jan Haller since 2012.

* E-mail: bromeon@gmail.com
* Project homepage: http://www.bromeon.ch/libraries/aurora
* GitHub repository: https://github.com/Bromeon/Aurora

