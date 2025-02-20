.. title:: Quick Start

Quick Start
===========

The library is header only and consists of two main components: the frontend and the backend.

The **frontend** captures a copy of the log arguments and metadata from each ``LOG_*`` statement and places them in a thread-local SPSC queue buffer.

The **backend** runs in a separate thread, spawned by the library, asynchronously consuming messages from the frontend, formatting them, and writing them to the configured sinks.

To use the library, you need to start the backend thread in your application. Then, set up one or more output ``Sinks`` and a ``Logger``.

Once the initialization is complete, you only need to include two header files to issue log statements:

- ``#include "quill/LogMacros.h"``
- ``#include "quill/Logger.h"``

These headers have minimal dependencies, keeping compilation times low.

For even faster compilation, consider building the backend initialization as a static library, as shown in:
`Recommended Usage Example <https://github.com/odygrd/quill/tree/master/examples/recommended_usage>`_.

For a quick reference on usage see :doc:`Cheat Sheet <cheat_sheet>`.

Logging to Console
------------------

.. literalinclude:: examples/quill_docs_example_console.cpp
   :language: cpp
   :linenos:

Logging to File
---------------

.. literalinclude:: examples/quill_docs_example_file.cpp
   :language: cpp
   :linenos:
