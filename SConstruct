import os
import sys

environment = Environment(ENV = os.environ)
environment.Append(LIBPATH=[os.path.join(os.getcwd(), "lib")], RPATH=[os.path.join(os.getcwd(), "lib")], CPPPATH=[os.path.join(os.getcwd(), "include")])

if os.name=="posix":
    environment.Append(
        CCFLAGS = " -Wall -Wextra -Werror -Wno-unused-parameter -Os -g ",
        CXXFLAGS = " -std=c++11 -Wno-invalid-offsetof -fno-rtti -fno-exceptions",
        CFLAGS = " -ansi ",
        LINKFLAGS = " -g ",
        PLUGINPATH = os.path.join(os.getcwd(), "build/plugins"),
        ENGINEPATH = os.path.join(os.getcwd(), "build"),
        OURLIBPATH = os.path.join(os.getcwd(), "lib")
    )

turbo = SConscript(dirs=["src"], exports=["environment"])
