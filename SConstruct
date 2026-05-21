import sys
import os

# Initialize the environment
env = Environment( CFLAGS=["-O2", "-Wall"], CPPPATH=["include"])

if sys.platform == "win32": # Windows Settings
    env.Append(LIBS=["glfw3", "opengl32", "user32", "gdi32", "shell32"])
else: # Linux Settings
    # SCons automatically uses gcc for .c files; no need to force CC or CXX here.
    pass

# --- COMMON BUILD LOGIC ---
VariantDir('build', 'src', duplicate=0)

# Gather source files from both directories
sources = Glob("src/*.c") + Glob("include/*.c")

# Build the runtime executable
env.Program( 
    target="build/runtime", 
    source=sources
)
