import sys
import os

# 1. Initialize ONE environment with all your settings and the compilation tool
env = Environment(
    CFLAGS=["-O2", "-Wall"],
    CPPPATH=["include"],
    COMPILATIONDB_USE_ABSPATH=True
)
env.Tool('compilation_db')

# 2. Apply Platform Settings
if sys.platform == "win32":
    env.Append(LIBS=["glfw3", "opengl32", "user32", "gdi32", "shell32"])

# 3. Define Build Folders and Sources
VariantDir('build', 'src', duplicate=0)
sources = Glob("src/*.c") + Glob("include/*.c")

# 4. Build the Executable
runtime_bin = env.Program(
    target="build/runtime",
    source=sources
)

# 5. Generate the JSON database file for Zed (tied to your build target)
env.CompilationDatabase('compile_commands.json')
