import inspect
import json

def first_function(param1, param2=42, *args, **kwargs):
    """An example function for demonstration."""
    print("This is an example function")

def Second_function(x, y: int, z: str = "default"):
    """Another function with type hints and default values."""
    return x + y

def parse_functions_to_json(module):
    function_definitions = []

    for name, obj in inspect.getmembers(module):
        if inspect.isfunction(obj):
            func_name = obj.__name__

            signature = inspect.signature(obj)

            docstring = inspect.getdoc(obj)

            properties = {}
            required = []

            for param_name, param in signature.parameters.items():

                param_info = {"type": "string"}  # default to string type

                if param.annotation != inspect.Parameter.empty:
                    param_info["type"] = param.annotation.__name__

                param_info["description"] = f"The {param_name} parameter."

                properties[param_name] = param_info

                if param.default == inspect.Parameter.empty:
                    required.append(param_name)

            # Create the JSON definition
            function = {
                "name": func_name,
                "description": docstring or "No description available.",
                "parameters": {
                    "type": "object",
                    "properties": properties,
                    "required": required,
                    "additionalProperties": False
                }
            }

            function_definitions.append(function)

    json_output = json.dumps(function_definitions, indent=4) # this is a string
    return function_definitions

_PYTHON_TYPE_MAP = {
    "str": "string",
    "int": "integer",
    "float": "number",
    "bool": "boolean",
}

def parse_class_to_tools(cls, exclude=None):
    """Generate OpenAI function tool definitions from a class's public methods.

    Skips private methods (starting with '_') and any names in `exclude`.
    Returns a list of dicts in the OpenAI function-tool format.
    """
    skip = set(exclude or [])
    tools = []

    for name, method in inspect.getmembers(cls, predicate=inspect.isfunction):
        if name.startswith("_") or name in skip:
            continue

        sig = inspect.signature(method)
        docstring = inspect.getdoc(method) or "No description available."

        properties = {}
        required = []

        for param_name, param in sig.parameters.items():
            if param_name == "self":
                continue

            type_name = "string"
            if param.annotation != inspect.Parameter.empty:
                raw = getattr(param.annotation, "__name__", str(param.annotation))
                type_name = _PYTHON_TYPE_MAP.get(raw, "string")

            properties[param_name] = {
                "type": type_name,
                "description": f"The {param_name} parameter.",
            }

            if param.default == inspect.Parameter.empty:
                required.append(param_name)

        tools.append({
            "type": "function",
            "name": name,
            "description": docstring,
            "parameters": {
                "type": "object",
                "properties": properties,
                "required": required,
                "additionalProperties": False,
            },
            "strict": True,
        })

    return tools

if __name__ == "__main__":
    import __main__
    parse_functions_to_json(__main__)
