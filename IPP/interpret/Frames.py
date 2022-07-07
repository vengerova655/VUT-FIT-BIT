# @author: Veronika Vengerova
# login: xvenge01
# date: 19.4.2022

import sys
from collections import deque

# ----------------------------------------------------------------------------------------------------------------------
# ----------------------------------------Symbol-Class------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

# Suplementary class to represent variable/int/bool/string/float/nil.
# Consists of type and value of given type
class Symbol:
    def __init__(self, value, type):
        self.type = type
        self.value = value

    # returns the type of symbol instance
    def get_type(self):
        return self.type

    # sets the type of symbol instance
    def set_type(self, typ):
        self.type = typ

    # sets the value of symbol instance
    def set_value(self, value):
        self.value = value

    # returns the value of symbol instance
    def get_value(self):
        return self.value


# ----------------------------------------------------------------------------------------------------------------------
# ----------------------------------------Frame-Class-------------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

# Suplementary class to represent Frame
# consists of defined and declared variables of instance of this class
class Frame:
    def __init__(self):
        self.variables = {}

    # function defines variable with given name
    def define_var(self, name):
        self.variables[name] = None


    # function returns true if variable with given name is already defined in this frame, otherwise false
    def is_defined(self, name):
        if name in self.variables:
            return True
        return False

    # additional auxiliary function to write all defined and declared frame variables, for quick debug
    def write_frame_variables(self, fr):
        print("*"*20 + f"{fr}" + "*"*20)
        for key, value in self.variables.items():
            print(f"{key}:")
            print(f"{value.get_type()} -> {value.get_value()}")
        print("*" * 42)

    # function that return the value saved in variable with var name
    # function does not check if variable is already defined
    def get_value(self, var):
        return self.variables[var]

    # function to save variable with name -var of type -typ with value -value to frame
    def save_variable(self, var, value, typ):
        self.variables[var] = Symbol(value, typ)


# ----------------------------------------------------------------------------------------------------------------------
# ----------------------------------------Frame_stack-Class-------------------------------------------------------------
# ----------------------------------------------------------------------------------------------------------------------

# class to represent stack of frames
class Frame_stack:
    def __init__(self):
        self.stack = deque()

    # function to return the least added Frame, which is LF frame or None if frame stack is empty
    # the frame will not be removed from stack
    def get_LF(self):
        if self.stack:
            return self.stack[-1]
        else:
            return None

    # function to return the least added frame and remove this frame from stack
    def pop_Frame(self):
        try:
            result = self.stack.pop()
        except: # TODO probably give IndexError
            print("NO LF found in stack", file=sys.stderr)
            exit(55)
        return result

    # function to push given frame -fr onto the frame stack
    def push_Frame(self, fr: Frame):
        self.stack.append(fr)

