def get_function_name(file_path):
    file = list(open(file_path).read())
    return "".join(file[:file.index("(")])

def get_inputs(file_path):
    file = list(open(file_path).read())
    return "".join(file[file.index("(")+1:file.index(")")])

def get_expression(file_path):
    file = list(open(file_path).read())
    return "".join(file[file.index("=")+1:-1])

def get_all(file_path):
    file = list(open(file_path).read())
    function_name = "".join(file[:file.index("(")])
    arguments = "".join(file[file.index("(")+1:file.index(")")])
    expression = "".join(file[file.index("=")+1:-1])
    return function_name, arguments, expression