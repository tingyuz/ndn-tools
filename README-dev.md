# Notes for IML Dataservice Developers

## Directory Structure and Build Script

All tools are placed in subdirectories of the [`tools`](tools) directory.

A tool can consist of one or more programs.
For instance, a pair of consumer and producer programs that are designed to work together
should be considered a single tool, not two separate tools.

Each tool must have a `wscript` build script in its subdirectory. This script will be
invoked automatically if the corresponding tool is selected for the build. It should
compile the source code and produce one or more binaries in the `build/bin` directory
(e.g., use `target='../../bin/foo'`).

### Shared Modules

Modules shared among multiple tools should be placed in the [`core`](core) directory.
They are available for use in all tools.

A header in `core/` can be included in a tool like `#include "core/foo.hpp"`.

The `wscript` of a tool can link a program with modules in `core/` with `use='core-objects'`.

### Documentation

A file named `README.md` in the subdirectory of each tool should provide a brief
description.

## Code Guidelines

C++ code should conform to the
[ndn-cxx code style](https://named-data.net/doc/ndn-cxx/current/code-style.html).

### Namespace

Types in each tool should be declared in a sub-namespace inside `namespace ndn`.
For example, a tool in `tools/foo` directory has namespace `ndn::foo`.
This allows the tool to reference ndn-cxx types with unqualified name lookup.
This also prevents name conflicts between ndn-cxx and tools.

Types in `core/` should be declared directly inside `namespace ndn`,
or in a sub-namespace if desired.

### main Function

The `main` function of a program should be declared as a static function in
the namespace of the corresponding tool. This allows referencing types in
ndn-cxx and the tool via unqualified name lookup.

Then, another (non-static) `main` function must be defined in the global
namespace, and from there call the `main` function in the tool namespace.

These two functions should appear in a file named `main.cpp` in the tool's
subdirectory.

Example:

    namespace ndn {
    namespace foo {

    class Bar
    {
    public:
      explicit
      Bar(Face& face);

      void
      run();
    };

    static int
    main(int argc, char* argv[])
    {
      Face face;
      Bar program(face);
      program.run();
      return 0;
    }

    } // namespace foo
    } // namespace ndn

    int
    main(int argc, char* argv[])
    {
      return ndn::foo::main(argc, argv);
    }

### Command Line Arguments

[Boost.Program\_options](https://www.boost.org/doc/libs/1_65_1/doc/html/program_options.html)
is strongly preferred over `getopt(3)` for parsing command line arguments.
