from parser import parser
import os 
import subprocess
import ctypes

output_file = "/home/ooh/Projects/MOC/gen/gen.c"

def generate(function_name, arguments, expression):
    with open(output_file, "w") as f:
        f.write(f"""double {function_name}(double {arguments}){{\n    return {expression};\n}}""")
    subprocess.run(["gcc", "gen.c", "-o", (f"{function_name}.so"), "-shared", "-fPIC"])

def get(file_path, function, vale):
    if not os.path.isfile(output_file):
        function_name, arguments, expression = parser.get_all(file_path)
        generate(function_name, arguments, expression)
        
    else:
        lib = ctypes.CDLL(f'/home/ooh/Projects/MOC/gen/square.so')
        lib.square.argtypes = [ctypes.c_double]
        lib.square.restype = ctypes.c_double
        res = lib.square(vale) 
        return res